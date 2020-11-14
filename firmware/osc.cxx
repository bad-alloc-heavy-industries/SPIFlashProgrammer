// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "osc.hxx"

extern void oscInit() noexcept
{
	// Clock bring-up on this chip has to be divided into three distinct phases, thanks to shenanigans.
	// In the first phase, MOsc has to be enabled
	// In the second, the CPU clock source has to be switched to MOsc and the PLL powered up
	// In the third, once the PLL reads as stable, the bypass removed and SysClockDiv configured
	// to provide a suitable operating clock.
	// Once all this is done, we finally end up running on the PLL generated clock and can
	// do bring-up on the USB PLL.
	// Additionally, the third phase has to switch clock config registers from the primary
	// to the secondary so we can select 80MHz as our operating frequency

	sysCtrl.mainOscCtrl = vals::sysCtrl::mainOscCtrlOscFailInterrupt | vals::sysCtrl::mainOscCtrlClockMonitorEnable;
	sysCtrl.runClockConfig1 &= vals::sysCtrl::runClockCfg1MainOscEnableMask;

	// TI have provided no way to check that the main oscillator came up.. instead
	// even their own platform library busy loops for 524288 iterations, then hopes
	// and prays when they enable the source that things don't go sideways in a hurry.
	for (volatile uint32_t loops = 524288U; loops; --loops)
		loops;

	sysCtrl.runClockConfig1 = (sysCtrl.runClockConfig1 & vals::sysCtrl::runClockCfg1Mask) |
		vals::sysCtrl::runClockCfg1MainOscEnable | vals::sysCtrl::runClockCfg1MainOscXtal25MHz |
		vals::sysCtrl::runClockCfg1PLLPowerUp | vals::sysCtrl::runClockCfg1PLLBypass |
		vals::sysCtrl::runClockCfg1NoPWMClkDiv | vals::sysCtrl::runClockCfg1NoSysClkDiv |
		vals::sysCtrl::runClockCfg1OscSourceMainOsc | vals::sysCtrl::runClockCfg1SysClockDiv(0);
	while (!(sysCtrl.pllStatus & vals::sysCtrl::pllStatusLocked))
		continue;

	sysCtrl.runClockConfig2 = (sysCtrl.runClockConfig2 & vals::sysCtrl::runClockCfg2Mask) |
		vals::sysCtrl::runClockCfg2UseRCC2 | vals::sysCtrl::runClockCfg2PLLPreDivDisable |
		vals::sysCtrl::runClockCfg2PLLUSBPowerUp | vals::sysCtrl::runClockCfg2PLLPowerUp |
		vals::sysCtrl::runClockCfg2PLLNoBypass | vals::sysCtrl::runClockCfg2OscSourceMainOsc |
		vals::sysCtrl::runClockCfg2SysClockDiv(2) | vals::sysCtrl::runClockCfgSysClockDivLSBClr;
}
