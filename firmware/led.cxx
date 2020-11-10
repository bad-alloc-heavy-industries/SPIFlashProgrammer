// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "led.hxx"

void ledInit() noexcept
{
	// Dwitch over to port D's AHB apeture
	sysCtrl.gpioAHBCtrl |= vals::sysCtrl::gpioAHBCtrlPortD;
	// Enable port D
	sysCtrl.runClockGateCtrlGPIO |= vals::sysCtrl::runClockGateCtrlGPIOD;

	// Wait for the port to come online
	while (!(sysCtrl.periphReadyGPIO & vals::sysCtrl::periphReadyGPIOD))
		continue;

	// Port D is protected, so enable changing it to digital GPIO
	gpioD.lock = vals::gpio::lockKey;
	gpioD.commit |= 0x01;
	gpioD.lock = 0;
	// Set the RGB LED pins as digital mode outputs
	gpioD.den |= 0x07;
	gpioD.afSel &= ~0x07;
	gpioD.dir = 0x07;

	// Turn the LED purple.
	gpioD.dataBits[0x07] = 0x06;
}
