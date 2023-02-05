// SPDX-License-Identifier: BSD-3-Clause
#include <tm4c123gh6pm/platform.hxx>
#include <tm4c123gh6pm/constants.hxx>
#include "spi.hxx"
#include "led.hxx"
#include "timer.hxx"

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
	// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
	// These store what the local flash that's installed's manufacturer, type and capacity read as.
	std::array<flashID_t, internalChips> localChip{};

	static spiChip_t targetDevice{spiChip_t::none};
	// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
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

	// Set the target reset pin as a high digital mode output
	gpioA.dataBits[0x80U] = 0x80U;
	gpioA.den |= 0x80U;
	gpioA.afSel &= ~0x80U;
	gpioA.dir |= 0x80U;

	// PF0 is an NMI, so unlock the port for reconfig
	gpioF.lock = vals::gpio::lockKey;
	gpioF.commit |= 1;
	gpioF.lock = 0;

	// Set the internal SPI bus pins as digital mode IO
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

	// Set the external SPI bus pins as digital mode IO
	gpioA.den |= 0x34U;
	gpioA.portCtrl |= vals::gpio::portA::portCtrlPin2SSI0Clk |
		vals::gpio::portA::portCtrlPin4SSI0Rx | vals::gpio::portA::portCtrlPin5SSI0Tx;
	gpioA.afSel |= 0x34U;
	gpioA.dir = (gpioA.dir & 0xCBU) | 0x24U;

	// For the target interface it's much the same but start out with a much slower clock (500kHz)
	// to guarantee comms then spin it up once we know the device can handle the speed
	ssi0.ctrl0 = vals::ssi::ctrl0FormatMotorola | vals::ssi::ctrl0ClockPolarityLow |
		vals::ssi::ctrl0ClockPhaseLeading | vals::ssi::ctrl0Data8Bit;
	ssi0.clockConfig = vals::ssi::clockConfigSysClk;
	// 80MHz -> 500kHz = 160
	//ssi0.cpsr = 160;
	ssi0.cpsr = 8;
	ssi0.ctrl1 = vals::ssi::control1ModeController | vals::ssi::control1EnableOperations;

	localChip[0] = identDevice(spiChip_t::local1);
	localChip[1] = identDevice(spiChip_t::local2);
	if (!checkDeviceID(0) || !checkDeviceID(1))
		ledSetColour(true, false, false);
	else
		ledSetColour(false, true, false);
}

void spiSelect(const spiChip_t chip) noexcept
{
	// NOLINTBEGIN(bugprone-branch-clone)
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
	// NOLINTEND(bugprone-branch-clone)
	targetDevice = chip;
}

tivaC::ssi_t *spiDevice(const spiChip_t chip) noexcept
{
	if (chip == spiChip_t::local1 || chip == spiChip_t::local2)
		return &ssi1;
	else if (chip == spiChip_t::target)
		return &ssi0;
	return nullptr;
}

tivaC::ssi_t *spiDevice() noexcept { return spiDevice(targetDevice); }

void spiResync(tivaC::ssi_t &device)
{
	// TxFIFOEmpty should always be true on entry as otherwise read-to-write desynced,
	// but lets check all the same just in case.
	while (!(device.status & vals::ssi::statusTxFIFOEmpty))
		continue;
	// RxFIFONotEmpty should always be false on entry as otherwise read-to-write desynced,
	// but lets check all the same just in case.
	while (device.status & vals::ssi::statusRxFIFONotEmpty)
		// NOLINTNEXTLINE(readability-identifier-length)
		[[maybe_unused]] const volatile auto _{device.data};
}

uint8_t spiRead(tivaC::ssi_t &device) noexcept
{
	spiResync(device);
	device.data = 0;
	// Wait for the dummy data to be shifted out and the reply read in
	while (!(device.status & vals::ssi::statusTxFIFOEmpty))
		continue;
	while (!(device.status & vals::ssi::statusRxFIFONotEmpty))
		continue;
	return uint8_t(device.data);
}

uint8_t spiRead() noexcept
{
	auto *device{spiDevice()};
	if (device)
		return spiRead(*device);
	return {};
}

void spiWrite(tivaC::ssi_t &device, const uint8_t value) noexcept
{
	spiResync(device);
	device.data = value;
	while (!(device.status & vals::ssi::statusTxFIFOEmpty))
		continue;
	while (!(device.status & vals::ssi::statusRxFIFONotEmpty))
		continue;
	// NOLINTNEXTLINE(readability-identifier-length)
	[[maybe_unused]] const volatile auto _{device.data};
}

void spiWrite(const uint8_t value) noexcept
{
	auto *device{spiDevice()};
	if (device)
		spiWrite(*device, value);
}

flashID_t readID(const spiChip_t chip) noexcept
{
	spiSelect(chip);
	auto &device{*spiDevice()};
	spiWrite(device, spiOpcodes::jedecID);
	const auto mfr
	{
		[&]()
		{
			if (chip == spiChip_t::target)
			{
				volatile auto value = spiRead(device);
				if (value != 255U)
					return value;
			}
			return spiRead(device);
		}()
	};
	const auto type{spiRead(device)};
	const auto capacity{spiRead(device)};
	spiSelect(spiChip_t::none);
	return {mfr, type, capacity};
}

void setDeviceReset(const bool resetState) noexcept
	{ gpioA.dataBits[0x80U] = resetState ? 0x80U : 0x00U; }
bool isDeviceReset() noexcept { return gpioA.dataBits[0x80U]; }

flashID_t identDevice(const spiChip_t chip, const bool releaseReset) noexcept
{
	if (chip == spiChip_t::target)
		setDeviceReset(true);
	auto chipID{readID(chip)};
	if (chipID.manufacturer == 0xFFU && chipID.type == 0xFFU)
	{
		spiSelect(spiChip_t::target);
		spiWrite(spiOpcodes::wakeUp);
		spiSelect(spiChip_t::none);
		waitFor(20); // 20us
		chipID = readID(chip);
	}
	if (chip == spiChip_t::target && releaseReset)
		setDeviceReset(false);
	return chipID;
}

bool checkDeviceID(const uint8_t index) noexcept
{
	return
		localChip[index].manufacturer == 0x1FU &&
		localChip[index].type == 0x32U &&
		localChip[index].capacity == 0x17U;
}
