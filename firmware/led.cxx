// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "led.hxx"

/*!
 * RGB LED pinout:
 * PD0 - G
 * PD1 - R
 * PD2 - B
 */

void ledInit() noexcept
{
	// Switch over to port D's AHB apeture
	sysCtrl.gpioAHBCtrl |= vals::sysCtrl::gpioAHBCtrlPortD;
	// Enable port D
	sysCtrl.runClockGateCtrlGPIO |= vals::sysCtrl::runClockGateCtrlGPIOD;

	// Wait for the port to come online
	while (!(sysCtrl.periphReadyGPIO & vals::sysCtrl::periphReadyGPIOD))
		continue;

	// Set the RGB LED pins as digital mode outputs
	gpioD.den |= 0x07U;
	gpioD.afSel &= ~0x07U;
	gpioD.dir = 0x07U;

	// Turn the LED purple.
	ledSetColour(true, false, true);
}

void ledSetColour(bool r, bool g, bool b) noexcept
{
	gpioD.dataBits[0x07] =
		(r ? 0x02 : 0x00) |
		(g ? 0x01 : 0x00) |
		(b ? 0x04 : 0x00);
}
