// SPDX-License-Identifier: BSD-3-Clause
#ifndef SPI___HXX
#define SPI___HXX

#include <cstdint>
#include <array>
#include <tuple>
#include <tm4c123gh6pm/platform.hxx>
#include "flash.hxx"

enum class spiChip_t
{
	local1,
	local2,
	target,
	none
};

extern void spiInit() noexcept;
extern void spiSelect(spiChip_t chip) noexcept;
extern tivaC::ssi_t *spiDevice() noexcept;
extern tivaC::ssi_t *spiDevice(spiChip_t chip) noexcept;
extern uint8_t spiRead(tivaC::ssi_t &device) noexcept;
extern void spiWrite(tivaC::ssi_t &device, uint8_t value) noexcept;
extern uint8_t spiRead() noexcept;
extern void spiWrite(uint8_t value) noexcept;
extern flashID_t identDevice(spiChip_t chip, bool releaseReset = true) noexcept;
extern void setDeviceReset(bool resetState) noexcept;

namespace spi
{
	constexpr static const uint8_t internalChips{2};
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
	constexpr static uint8_t writeEnable{0x06U};
	constexpr static uint8_t writeDisable{0x04U};
	constexpr static uint8_t releasePowerDown{0xABU};
} // namespace spiOpcodes

#endif /*SPI___HXX*/
