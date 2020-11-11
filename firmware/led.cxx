// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "led.hxx"

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
	gpioD.dataBits[0x07] = 0x06U;
}
