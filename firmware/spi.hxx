// SPDX-License-Identifier: BSD-3-Clause
#ifndef SPI_HXX
#define SPI_HXX

#include <cstdint>
#include <array>
#include <tuple>
#include <tm4c123gh6pm/platform.hxx>
#include "flash.hxx"

void spiInit() noexcept;
void spiResetClocks() noexcept;
void spiSelect(spiChip_t chip) noexcept;
tivaC::ssi_t *spiDevice() noexcept;
tivaC::ssi_t *spiDevice(spiChip_t chip) noexcept;
uint8_t spiRead(tivaC::ssi_t &device) noexcept;
void spiWrite(tivaC::ssi_t &device, uint8_t value) noexcept;
uint8_t spiRead() noexcept;
void spiWrite(uint8_t value) noexcept;
flashID_t identDevice(spiChip_t chip, bool releaseReset = true) noexcept;
void setDeviceReset(bool resetState) noexcept;
bool isDeviceReset() noexcept;

namespace spi
{
	constexpr static const uint8_t internalChips{2};
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
	extern std::array<flashID_t, internalChips> localChip;
}

namespace spiOpcodes
{
	constexpr static uint8_t jedecID{0x9FU};
	constexpr static uint8_t chipErase{0xC7U};
	constexpr static uint8_t blockErase{0xD8U};
	constexpr static uint8_t pageRead{0x03U};
	constexpr static uint8_t pageAddressRead{0x13U};
	constexpr static uint8_t pageWrite{0x02U};
	constexpr static uint8_t pageAddressWrite{0x10U};
	constexpr static uint8_t statusRead{0x05U};
	constexpr static uint8_t statusWrite{0x01U};
	constexpr static uint8_t writeEnable{0x06U};
	constexpr static uint8_t writeDisable{0x04U};
	constexpr static uint8_t readSFDP{0x5AU};
	constexpr static uint8_t wakeUp{0xABU};
	constexpr static uint8_t reset{0xFFU};
} // namespace spiOpcodes

#endif /*SPI_HXX*/
