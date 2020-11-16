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

//usbDeviceState_t usbState;
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
	usb.intEn &= vals::usb::itrEnableDeviceMask;
	usb.txIntEn &= vals::usb::txItrEnableMask;
	usb.rxIntEn &= vals::usb::rxItrEnableMask;
}

void irqUSB() noexcept
{
	//
}
