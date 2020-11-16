// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "platform.hxx"
#include "usb.hxx"
#include "usb/types.hxx"

/*!
 * USB pinout:
 * PD4 - D-
 * PD5 - D+
 *
 * PB0 - ID
 * PB1 - VBus
 */

usbTypes::deviceState_t usbState;
volatile bool usbSuspended;
usbTypes::ctrlState_t usbCtrlState;
uint8_t usbDeferalFlags;

void usbReset() noexcept;

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

	for (uint8_t i{}; i < (usbTypes::endpointCount + 1) >> 1; ++i)
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

	// Once we get done, idle the peripheral
	usbState = usbTypes::deviceState_t::detached;
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
		usbState = usbTypes::deviceState_t::waiting;
	}

	if (status & vals::usb::itrStatusSuspend)
		usbSuspend();

}
