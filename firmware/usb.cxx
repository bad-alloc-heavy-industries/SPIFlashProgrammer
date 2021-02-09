// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "platform.hxx"
#include "usb.hxx"
#include "usb/core.hxx"
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

using namespace usb::types;

namespace usb::core
{
	deviceState_t usbState;
	usbEP_t usbPacket;
	bool usbSuspended;
	ctrlState_t usbCtrlState;
	uint8_t usbDeferalFlags;

	std::array<usbEPStatus_t<const void>, endpointCount> epStatusControllerIn;
	std::array<usbEPStatus_t<void>, endpointCount> epStatusControllerOut;
} // namespace usb::core

using namespace usb::core;

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
	gpioB.afSel |= 0x03U;
	gpioB.dir &= ~0x01U;

	// Put the controller in device mode and reset the power control register completely.
	usbCtrl.gpCtrlStatus = vals::usb::gpCtrlStatusDeviceMode | vals::usb::gpCtrlStatusOTGModeDevice;
	usbCtrl.power &= vals::usb::powerMask;

	// Reset all USB interrupts
	usbCtrl.intEnable &= vals::usb::itrEnableDeviceMask;
	usbCtrl.txIntEnable &= vals::usb::txItrEnableMask;
	usbCtrl.rxIntEnable &= vals::usb::rxItrEnableMask;
	// And their flags
	vals::readDiscard(usbCtrl.intStatus);
	vals::readDiscard(usbCtrl.txIntStatus);
	vals::readDiscard(usbCtrl.rxIntStatus);
	// Ensure the device address is 0
	usbCtrl.address = 0;

	// Enable the USB NVIC and reset interrupt
	nvic.enableInterrupt(44);
	usbCtrl.intEnable |= vals::usb::itrEnableDeviceReset;

	// Initialise the state machine
	usbState = deviceState_t::detached;
	usbCtrlState = ctrlState_t::idle;
	usbDeferalFlags = 0;
	usbCtrl.power |= vals::usb::powerSoftConnect;
}

namespace usb::core
{
	void usbReset() noexcept
	{
		for (uint8_t i{}; i < endpointCount; ++i)
		{
			usbCtrl.epIndex = i;
			// 128 / 2 = 64, so this gives us 64 bytes per EP.
			usbCtrl.txFIFOSize = vals::usb::txFIFOSize128 | vals::usb::txFIFOSizeDoubleBuffered;
			usbCtrl.txFIFOAddr = vals::usb::fifoAddr(128 * (i * 2));
			usbCtrl.rxFIFOSize = vals::usb::rxFIFOSize128 | vals::usb::rxFIFOSizeDoubleBuffered;
			usbCtrl.rxFIFOAddr = vals::usb::fifoAddr(128 * ((i * 2) + 1));
		}
		// Really enable the double-buffers as apparently this isn't done just by the above.
		usbCtrl.txPacketDoubleBuffEnable |= vals::usb::txPacketDoubleBuffEnableEP1;
		usbCtrl.rxPacketDoubleBuffEnable |= vals::usb::rxPacketDoubleBuffEnableEP1;
		usbResetEPs(true);

		// Once we get done, idle the peripheral
		usbCtrl.address = 0;
		usbState = deviceState_t::attached;
		usbCtrl.intEnable |= vals::usb::itrEnableDisconnect | vals::usb::itrEnableSOF;
		usbCtrl.txIntEnable &= vals::usb::txItrEnableMask;
		usbCtrl.rxIntEnable &= vals::usb::rxItrEnableMask;
		usbCtrl.txIntEnable |= vals::usb::txItrEnableEP0;
	}

	void usbResetEPs(const bool includeEP0) noexcept
	{
		for (auto &[i, epStatus] : utility::indexedIterator_t{epStatusControllerIn})
		{
			epStatus->resetStatus();
			epStatus->transferCount = 0;
			epStatus->ctrl.endpoint(i);
			epStatus->ctrl.dir(endpointDir_t::controllerIn);
		}

		for (auto &[i, epStatus] : utility::indexedIterator_t{epStatusControllerOut})
		{
			epStatus->resetStatus();
			epStatus->transferCount = 0;
			epStatus->ctrl.endpoint(i);
			epStatus->ctrl.dir(endpointDir_t::controllerOut);
		}
	}

	void usbDetach()
	{
		if (usbState == deviceState_t::detached)
			return;
		usbCtrl.power &= vals::usb::powerSoftDisconnectMask;
		usbCtrl.intEnable &= vals::usb::itrEnableDeviceMask;
		usbCtrl.power |= vals::usb::powerSoftConnect;
		usbCtrl.intEnable |= vals::usb::itrEnableDeviceReset;
		usbState = deviceState_t::detached;
	}

	void usbWakeup()
	{
		usbSuspended = false;
		usbCtrl.power |= vals::usb::powerResume;
		while ((usbCtrl.power & vals::usb::powerSuspend) == vals::usb::powerSuspend)
			continue;
		usbCtrl.power &= ~vals::usb::powerResume;
		usbCtrl.intEnable &= ~vals::usb::itrEnableResume;
		usbCtrl.intEnable |= vals::usb::itrEnableSuspend;
	}

	void usbSuspend()
	{
		usbCtrl.intEnable &= ~vals::usb::itrEnableSuspend;
		usbCtrl.intEnable |= vals::usb::itrEnableResume;
		usbCtrl.power |= vals::usb::powerSuspend;
		usbSuspended = true;
	}

	const uint8_t *sendData(const uint8_t ep, const uint8_t *const buffer, const uint8_t length) noexcept
	{
		// Copy the data to tranmit from the user buffer
		for (uint8_t i{}; i < (length & 0xFCU); i += 4)
			writeFIFO_t<uint32_t>{}(usbCtrl.epFIFO[ep], buffer + i);
		if (length & 0x02U)
			writeFIFO_t<uint16_t>{}(usbCtrl.epFIFO[ep], buffer + (length & 0xFEU) - 2);
		if (length & 0x01U)
			writeFIFO_t<uint8_t>{}(usbCtrl.epFIFO[ep], buffer + length - 1);
		return buffer + length;
	}

	uint8_t *recvData(const uint8_t ep, uint8_t *const buffer, const uint8_t length) noexcept
	{
		// Copy the received data to the user buffer
		for (uint8_t i{}; i < (length & 0xFCU); i += 4)
			readFIFO_t<uint32_t>{}(usbCtrl.epFIFO[ep], buffer + i);
		if (length & 0x02U)
			readFIFO_t<uint16_t>{}(usbCtrl.epFIFO[ep], buffer + (length & 0xFEU) - 2);
		if (length & 0x01U)
			readFIFO_t<uint8_t>{}(usbCtrl.epFIFO[ep], buffer + length - 1);
		return buffer + length;
	}
} // namespace usb::core

void irqUSB() noexcept
{
	const auto status{usbCtrl.intStatus & usbCtrl.intEnable};
	const auto rxStatus{usbCtrl.rxIntStatus & usbCtrl.rxIntEnable};
	const auto txStatus{usbCtrl.txIntStatus & usbCtrl.txIntEnable};

	if (status & vals::usb::itrStatusDisconnect)
		return usbDetach();
	else if (usbState == deviceState_t::attached)
	{
		usbCtrl.intEnable |= vals::usb::itrEnableSuspend;
		usbState = deviceState_t::powered;
	}

	if (status & vals::usb::itrStatusResume)
		usbWakeup();
	else if (usbSuspended)
		return;

	if (status & vals::usb::itrStatusDeviceReset)
	{
		usbReset();
		usbState = deviceState_t::waiting;
		return;
	}

	if (status & vals::usb::itrStatusSuspend)
		usbSuspend();

	if (usbState == deviceState_t::detached ||
		usbState == deviceState_t::attached ||
		usbState == deviceState_t::powered ||
		(!rxStatus && !txStatus))
		return;

	for (uint8_t endpoint{}; endpoint < endpointCount; ++endpoint)
	{
		const uint16_t endpointMask = 1U << endpoint;
		if (rxStatus & endpointMask || txStatus & endpointMask)
		{
			usbPacket.endpoint(endpoint);
			if (rxStatus & endpointMask || (endpoint == 0 &&
					(usbCtrl.ep0Ctrl.statusCtrlL & vals::usb::epStatusCtrlLRxReady)))
				usbPacket.dir(endpointDir_t::controllerOut);
			else
				usbPacket.dir(endpointDir_t::controllerIn);

			if (endpoint == 0)
				usb::device::handleControlPacket();
		}
	}
}
