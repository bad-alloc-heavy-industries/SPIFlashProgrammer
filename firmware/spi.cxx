// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "spi.hxx"
#include "led.hxx"

/*!
 * Onboard SPI bus pinout:
 * PE0 - CS2
 * PE1 - CS1
 *
 * PF0 - CIPO
 * PF1 - COPI
 * PF2 - CLK
 *
 * Target SPI bus pinout:
 * PA2 - CLK
 * PA3 - CS
 * PA4 - CIPO
 * PA5 - COPI
 *
 * The SPI peripheral on this device works in a write-to-read manner,
 * so the code in this file attempts to always keep the read and write
 * FIFOs equally fed and consumed.
 */

namespace spi
{
	// These store what the local flash that's installed's manufacturer, type and capacity read as.
	std::array<uint8_t, internalChips> localMFR{};
	std::array<uint8_t, internalChips> localType{};
	std::array<uint8_t, internalChips> localCapacity{};
}

using namespace spi;

bool checkDeviceID(uint8_t index) noexcept;

void spiInit() noexcept
{
	// Switch over to port A, E & F's AHB apeture
	sysCtrl.gpioAHBCtrl |= vals::sysCtrl::gpioAHBCtrlPortA | vals::sysCtrl::gpioAHBCtrlPortE |
		vals::sysCtrl::gpioAHBCtrlPortF;
	// Enable port A, E & F
	sysCtrl.runClockGateCtrlGPIO |= vals::sysCtrl::runClockGateCtrlGPIOA | vals::sysCtrl::runClockGateCtrlGPIOE |
		vals::sysCtrl::runClockGateCtrlGPIOF;

	// Wait for the ports to come online
	while (!(sysCtrl.periphReadyGPIO & (vals::sysCtrl::periphReadyGPIOA | vals::sysCtrl::periphReadyGPIOE |
			vals::sysCtrl::periphReadyGPIOF)))
		continue;

	// Enable SSI (SPI) port 0 (PA2-5), and 1 (PF0-2)
	sysCtrl.runClockGateCtrlSSI |= vals::sysCtrl::runClockGateCtrlSSI0 | vals::sysCtrl::runClockGateCtrlSSI1;
	// Wait for it to come online
	while (!(sysCtrl.periphReadySSI & (vals::sysCtrl::periphReadySSI0 | vals::sysCtrl::periphReadySSI1)))
		continue;

	// Set the chip select pins as high digital mode outputs
	gpioA.dataBits[0x08U] = 0x08U;
	gpioA.den |= 0x08U;
	gpioA.afSel &= ~0x08U;
	gpioA.dir = 0x08U;
	gpioE.dataBits[0x03U] = 0x03U;
	gpioE.den |= 0x03U;
	gpioE.afSel &= ~0x03U;
	gpioE.dir = 0x03U;

	// PF0 is an NMI, so unlock the port for reconfig
	gpioF.lock = vals::gpio::lockKey;
	gpioF.commit |= 1;
	gpioF.lock = 0;

	// Set the SPI bus pins as digital mode IO
	gpioF.den |= 0x07U;
	gpioF.portCtrl |= vals::gpio::portF::portCtrlPin0SSI1Rx |
		vals::gpio::portF::portCtrlPin1SSI1Tx | vals::gpio::portF::portCtrlPin2SSI1Clk;
	gpioF.afSel |= 0x07U;
	gpioF.dir = (gpioF.dir & 0xF8U) | 0x06U;

	// We want to use 8-bit SPI (Motorola) in mode 0
	ssi1.ctrl0 = vals::ssi::ctrl0FormatMotorola | vals::ssi::ctrl0ClockPolarityLow |
		vals::ssi::ctrl0ClockPhaseLeading | vals::ssi::ctrl0Data8Bit;
	ssi1.clockConfig = vals::ssi::clockConfigSysClk;
	// We have a 25MHz clock PLL'd to 80MHz, which we want divided down as little as possible
	// Scale the clock by 2 to make it 1/2 the system clock
	ssi1.cpsr = 2;
	// Enable the interface
	ssi1.ctrl1 = vals::ssi::control1ModeController | vals::ssi::control1EnableOperations;

	spiSelect(spiChip_t::local1);
	spiIntWrite(spiOpcodes::jedecID);
	localMFR[0] = spiIntRead();
	localType[0] = spiIntRead();
	localCapacity[0] = spiIntRead();

	spiSelect(spiChip_t::local2);
	spiIntWrite(spiOpcodes::jedecID);
	localMFR[1] = spiIntRead();
	localType[1] = spiIntRead();
	localCapacity[1] = spiIntRead();

	spiSelect(spiChip_t::none);
	if (!checkDeviceID(0) || !checkDeviceID(1))
		ledSetColour(true, false, false);
	else
		ledSetColour(false, true, false);
}

void spiSelect(const spiChip_t chip) noexcept
{
	switch (chip)
	{
		case spiChip_t::local1:
			gpioA.dataBits[0x08U] = 0x08U;
			gpioE.dataBits[0x03U] = 0x02U;
			break;
		case spiChip_t::local2:
			gpioA.dataBits[0x08U] = 0x08U;
			gpioE.dataBits[0x03U] = 0x01U;
			break;
		case spiChip_t::target:
			gpioE.dataBits[0x03U] = 0x03U;
			gpioA.dataBits[0x08U] = 0x00U;
			break;
		case spiChip_t::none:
			gpioE.dataBits[0x03U] = 0x03U;
			gpioA.dataBits[0x08U] = 0x08U;
			break;
	}
}

void spiIntResync() noexcept
{
	[[maybe_unused]] uint8_t _;
	// TxFIFOEmpty should always be true on entry as otherwise read-to-write desynced,
	// but lets check all the same just in case.
	while (!(ssi1.status & vals::ssi::statusTxFIFOEmpty))
		continue;
	// RxFIFONotEmpty should always be false on entry as otherwise read-to-write desynced,
	// but lets check all the same just in case.
	while (ssi1.status & vals::ssi::statusRxFIFONotEmpty)
		_ = uint8_t(ssi1.data);
}

uint8_t spiIntRead() noexcept
{
	spiIntResync();
	ssi1.data = 0;
	// Wait for the dummy data to be shifted out and the reply read in
	while (!(ssi1.status & vals::ssi::statusRxFIFONotEmpty))
		continue;
	return uint8_t(ssi1.data);
}

void spiIntWrite(const uint8_t value) noexcept
{
	spiIntResync();
	ssi1.data = value;
	while (!(ssi1.status & vals::ssi::statusTxFIFOEmpty))
		continue;
	while (!(ssi1.status & vals::ssi::statusRxFIFONotEmpty))
		continue;
	[[maybe_unused]] const uint8_t _ = uint8_t(ssi1.data);
}

uint8_t spiExtRead() noexcept
{
	return 0;
}

void spiExtWrite(const uint8_t value) noexcept
{
}

bool checkDeviceID(const uint8_t index) noexcept
{
	return localMFR[index] == 0x1FU &&
		localType[index] == 0x32U &&
		localCapacity[index] == 0x17U;
}
