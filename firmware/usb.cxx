// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "platform.hxx"
#include "usb.hxx"
#include "usb/types.hxx"
#include "usb/device.hxx"
#include "indexedIterator.hxx"

/*!
 * USB pinout:
 * PD4 - D-
 * PD5 - D+
 *
 * PB0 - ID
 * PB1 - VBus
 */

usbTypes::deviceState_t usbState;
usbTypes::usbEP_t usbPacket;
bool usbSuspended;
usbTypes::ctrlState_t usbCtrlState;
uint8_t usbDeferalFlags;

std::array<usbTypes::usbEPStatus_t<const void>, usbTypes::endpointCount> epStatusControllerIn;
std::array<usbTypes::usbEPStatus_t<void>, usbTypes::endpointCount> epStatusControllerOut;

void usbReset() noexcept;

/*!
 * Transmitting packets:
 * Write data to endpoint TXFIFO register up to 4 bytes at a time,
 * but this must be kept consistent up to the last transfer.
 * Write TXRDY bit for the end point once ready for arming transmit.
 * On completion of transmission, TXRDY and FIFONE (FIFO Not Empty) get cleared.
 * When double-buffered, TXRDY gets immediately cleared when the second buffer
 * is also empty.
 * When either kind of transmission complete fires, a TX interrupt is triggered.
 *
 * Receiving packets:
 * Once data reception (automatic) is complete, RXRDY bit is set.
 * If double-buffered and the second buffer is now also full or if single-buffered,
 * endpoint FULL is set.
 * Read data from RXFIFO up to 4 bytes at a time, but this must also be kept
 * consistent up to the last transfer.
 * Once complete, RXDRY must be cleared to re-arm buffer. This also generates host ACK.
 * If single-buffered or this freed the second buffer in double-buffering,
 * this clears the FULL bit.
 *
 * DATAEND is the end-of-data-phase start-of-status-phase indicator
 */

void usbInit() noexcept
{
	// Enable the USB peripheral
	sysCtrl.runClockGateCtrlUSB |= vals::sysCtrl::runClockGateCtrlUSB;
	// and wait for it to become ready
	while (!(sysCtrl.periphReadyUSB & vals::sysCtrl::periphReadyUSB))
		continue;

	// Switch over to port B & D's AHB apeture
	sysCtrl.gpioAHBCtrl |= vals::sysCtrl::gpioAHBCtrlPortB | vals::sysCtrl::gpioAHBCtrlPortD;
	// Enable port B & D
	sysCtrl.runClockGateCtrlGPIO |= vals::sysCtrl::runClockGateCtrlGPIOB | vals::sysCtrl::runClockGateCtrlGPIOD;
	// Wait for the ports to come online
	while (!(sysCtrl.periphReadyGPIO & (vals::sysCtrl::periphReadyGPIOB | vals::sysCtrl::periphReadyGPIOD)))
		continue;

	// Configure the D+/D- pins as USB peripheral IO
	gpioD.amSel |= 0x30U;
	gpioD.portCtrl |= vals::gpio::portD::portCtrlPin4USBDataMinus | vals::gpio::portD::portCtrlPin5USBDataPlus;
	gpioD.afSel |= 0x30U;
	//gpioD.dir |= 0x30U;

	// Configure the ID and VBus pins as USB peripheral IO
	gpioB.amSel |= 0x03U;
	gpioB.portCtrl |= vals::gpio::portB::portCtrlPin0USBID | vals::gpio::portB::portCtrlPin1USBVBus;
	gpioD.afSel |= 0x03U;
	gpioD.dir &= ~0x01U;

	// Put the controller in device mode and reset the power control register completely.
	usb.gpCtrlStatus = vals::usb::gpCtrlStatusDeviceMode | vals::usb::gpCtrlStatusOTGModeDevice;
	usb.power &= vals::usb::powerMask;
	usbReset();
	usbState = usbTypes::deviceState_t::detached;
	usb.power |= vals::usb::powerSoftConnect;
	usbCtrlState = usbTypes::ctrlState_t::idle;
	usbDeferalFlags = 0;
}

void usbReset() noexcept
{
	// Reset all USB interrupts
	usb.intEnable &= vals::usb::itrEnableDeviceMask;
	usb.txIntEnable &= vals::usb::txItrEnableMask;
	usb.rxIntEnable &= vals::usb::rxItrEnableMask;
	// And their flags
	vals::readDiscard(usb.intStatus);
	vals::readDiscard(usb.txIntStatus);
	vals::readDiscard(usb.rxIntStatus);

	for (uint8_t i{}; i < usbTypes::endpointCount; ++i)
	{
		usb.epIndex = i;
		usb.txFIFOSize = vals::usb::txFIFOSize64 | vals::usb::txFIFOSizeDoubleBuffered;
		usb.txFIFOAddr;
		usb.rxFIFOSize = vals::usb::rxFIFOSize64 | vals::usb::rxFIFOSizeDoubleBuffered;
		usb.rxFIFOAddr;
	}
	// Really enable the double-buffers as apparently this isn't done just by the above.
	usb.txPacketDoubleBuffEnable |= vals::usb::txPacketDoubleBuffEnableEP1;
	usb.rxPacketDoubleBuffEnable |= vals::usb::rxPacketDoubleBuffEnableEP1;

	// Configure for FS USB operation
	//usb.ep0Ctrl.type = ;

	for (auto &[i, epStatus] : utility::indexedIterator_t{epStatusControllerIn})
	{
		epStatus->resetStatus();
		epStatus->transferCount = 0;
		epStatus->ctrl.endpoint(i);
		epStatus->ctrl.dir(usbTypes::endpointDir_t::controllerIn);
	}

	for (auto &[i, epStatus] : utility::indexedIterator_t{epStatusControllerOut})
	{
		epStatus->resetStatus();
		epStatus->transferCount = 0;
		epStatus->ctrl.endpoint(i);
		epStatus->ctrl.dir(usbTypes::endpointDir_t::controllerOut);
	}

	// Once we get done, idle the peripheral
	usb.address = 0;
	usbState = usbTypes::deviceState_t::attached;
}

void usbDetach()
{
	if (usbState == usbTypes::deviceState_t::detached)
		return;
	usb.power &= vals::usb::powerSoftDisconnectMask;
	usb.intEnable &= vals::usb::itrEnableDeviceMask;
	usbState = usbTypes::deviceState_t::detached;
}

void usbWakeup()
{
	usbSuspended = false;
	usb.power |= vals::usb::powerResume;
	while ((usb.power & vals::usb::powerSuspend) == vals::usb::powerSuspend)
		continue;
	usb.power &= ~vals::usb::powerResume;
	usb.intEnable &= ~vals::usb::itrEnableResume;
	usb.intEnable |= vals::usb::itrEnableSuspend;
}

void usbSuspend()
{
	usb.intEnable &= ~vals::usb::itrEnableSuspend;
	usb.intEnable |= vals::usb::itrEnableResume;
	usb.power |= vals::usb::powerSuspend;
	usbSuspended = true;
}

void irqUSB() noexcept
{
	const auto status{usb.intStatus & usb.intEnable};
	const auto rxStatus{usb.rxIntStatus & usb.rxIntEnable};
	const auto txStatus{usb.txIntStatus & usb.txIntEnable};

	if (status & vals::usb::itrStatusDisconnect)
		return usbDetach();
	else if (usbState == usbTypes::deviceState_t::attached)
		usb.intEnable |= vals::usb::itrEnableSuspend;

	if (status & vals::usb::itrStatusResume)
		usbWakeup();
	else if (usbSuspended)
		return;

	if (status & vals::usb::itrStatusDeviceReset)
	{
		usbReset();
		return;
	}

	if (status & vals::usb::itrStatusSuspend)
		usbSuspend();

	if (usbState == usbTypes::deviceState_t::detached ||
		usbState == usbTypes::deviceState_t::attached ||
		usbState == usbTypes::deviceState_t::powered ||
		(!rxStatus && !txStatus))
		return;

	for (uint8_t endpoint; endpoint < usbTypes::endpointCount; ++endpoint)
	{
		const uint16_t endpointMask = 1U << endpoint;
		if (rxStatus & endpointMask || txStatus & endpointMask)
		{
			usbPacket.endpoint(endpoint);
			if (rxStatus & endpointMask ||
					(endpoint == 0 && usb.ep0Ctrl.statusCtrlL & vals::usb::epStatusCtrlLRxReady))
				usbPacket.dir(usbTypes::endpointDir_t::controllerOut);
			else
				usbPacket.dir(usbTypes::endpointDir_t::controllerIn);

			if (endpoint == 0)
				usbDevice::handleControlPacket();
		}
	}
}
