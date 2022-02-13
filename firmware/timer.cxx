// SPDX-License-Identifier: BSD-3-Clause
#include <tm4c123gh6pm/platform.hxx>
#include <tm4c123gh6pm/constants.hxx>
#include "timer.hxx"

void timerInit() noexcept
{
	sysCtrl.runClockGateCtrlTimer |= vals::sysCtrl::runClockGateCtrlTimer0;
	while (!(sysCtrl.periphReadyTimer & vals::sysCtrl::periphReadyTimer0))
		continue;

	timer0.ctrl &= vals::timer::ctrlMask;
	timer0.config = (timer0.config & vals::timer::configMask) | vals::timer::configNarrow;
	timer0.timerAMode = (timer0.timerAMode & vals::timer::modeMask) |
		vals::timer::modeOneShot | vals::timer::modeCountDown;
	timer0.icr |= ~vals::timer::itrMask;
	// This sets the prescaler up so our 80MHz clock (12.5ns/cycle) will cause the counter to count in 1us increments
	timer0.timerAPrescale = 80U;
}

void waitFor(const uint32_t microSeconds) noexcept
{
	timer0.timerAIntervalLoad = microSeconds;
	timer0.ctrl |= vals::timer::ctrlTimerAEnable;
	while (!(timer0.ris & vals::timer::itrTimerATimeOut))
		continue;
	timer0.icr |= vals::timer::itrTimerATimeOut;
}
