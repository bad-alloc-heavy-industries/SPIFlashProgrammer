// SPDX-License-Identifier: BSD-3-Clause
#ifndef SPI___HXX
#define SPI___HXX

#include <cstdint>
#include <array>
#include <tuple>
#include <tm4c123gh6pm/platform.hxx>

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
extern std::tuple<uint8_t, uint8_t, uint8_t> identDevice(spiChip_t chip) noexcept;

namespace spi
{
	constexpr static const uint8_t internalChips{2};
	extern std::array<uint8_t, internalChips> localMFR;
	extern std::array<uint8_t, internalChips> localType;
	extern std::array<uint8_t, internalChips> localCapacity;
}

namespace spiOpcodes
{
	constexpr static const uint8_t jedecID{0x9FU};
	constexpr static const uint8_t chipErase{0xC7U};
	constexpr static const uint8_t blockErase{0xD8U};
	constexpr static const uint8_t pageRead{0x03U};
	constexpr static const uint8_t pageWrite{0x02U};
	constexpr static const uint8_t statusRead{0x05U};
	constexpr static const uint8_t writeEnable{0x06U};
	constexpr static const uint8_t writeDisable{0x04U};
} // namespace spiOpcodes

#endif /*SPI___HXX*/
