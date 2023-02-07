// SPDX-License-Identifier: BSD-3-Clause
#include <limits>
#include <substrate/utility>
#include <substrate/units>
#include "flash.hxx"
#include "sfdp.hxx"

#ifdef __GNUC__
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define unlikely(x) __builtin_expect((x), 0)
#else
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define unlikely(x) (x)
#endif

using namespace substrate;

namespace flash
{
	struct flashManufacturer_t
	{
		uint8_t manufacturerID;
		const flashChip_t *chips;
		size_t chipCount;

		[[nodiscard]] const flashChip_t *begin() const noexcept { return chips; }
		[[nodiscard]] const flashChip_t *end() const noexcept { return chips + chipCount; }
	};

	constexpr static auto adestoChips
	{
		substrate::make_array<flashChip_t>
		({
			{0x32, 0x17, 0x17, 0x20, 4_KiB, 256, 50}
		})
	};

	constexpr static auto numonyxChips
	{
		substrate::make_array<flashChip_t>
		({
			{0x20, 0x14, 0x14, 0xD8, 64_KiB, 256, 33},
			{0x20, 0x15, 0x15, 0xD8, 64_KiB, 256, 33}
		})
	};

	constexpr static auto gigaDeviceChips
	{
		substrate::make_array<flashChip_t>
		({
			{0x40, 0x13, 0x13, 0x20, 4_KiB, 256, 80},
			{0x40, 0x17, 0x17, 0x20, 4_KiB, 256, 80}
		})
	};

	constexpr static auto winbondChips
	{
		substrate::make_array<flashChip_t>
		({
			{0x40, 0x14, 0x14, 0x20, 4_KiB, 256, 50},
			{0x40, 0x15, 0x15, 0x20, 4_KiB, 256, 50},
			{0x40, 0x16, 0x16, 0x20, 4_KiB, 256, 50},
			{0x40, 0x18, 0x18, 0x20, 4_KiB, 256, 50},
			{0xAA, 0x21, 0x1B, 0xD8, 128_KiB, 2_KiB, 104},
			{0x70, 0x18, 0x18, 0x20, 4_KiB, 256, 50}
		})
	};

	constexpr static auto manufacturers
	{
		substrate::make_array<flashManufacturer_t>
		({
			{0x1F, adestoChips.data(), adestoChips.size()},
			{0x20, numonyxChips.data(), numonyxChips.size()},
			{0xC8, gigaDeviceChips.data(), gigaDeviceChips.size()},
			{0xEF, winbondChips.data(), winbondChips.size()},
		})
	};

	static inline uint8_t log2(uint32_t value) noexcept
	{
		if (unlikely(!value))
			return UINT8_MAX;
#if defined(__GNUC__)
		return (sizeof(uint32_t) * 8U) - static_cast<uint8_t>(__builtin_clz(value));
#else
		uint8_t result{};
		if (value <= UINT32_C(0x0000FFFF))
		{
			result += 16U;
			value <<= 16U;
		}
		if (value <= UINT32_C(0x00FFFFFF))
		{
			result += 8U;
			value <<= 8U;
		}
		if (value <= UINT32_C(0x0FFFFFFF))
		{
			result += 4U;
			value <<= 4U;
		}
		if (value <= UINT32_C(0x3FFFFFFF))
		{
			result += 2U;
			value <<= 2U;
		}
		if (value <= UINT32_C(0x7FFFFFFF))
			++result;
		return (sizeof(uint32_t) * 8U) - result;
#endif
	}

	static std::optional<flashChip_t> readSFDP(const flashID_t chipID, const spiChip_t targetDevice) noexcept
	{
		// The SFDP code will reclock the bus for us if the SFDP data can be read
		const auto parameters{sfdp::parameters(targetDevice)};
		if (!parameters)
			return std::nullopt;
		// Build a flashChip_t from the SFDP data gathered
		flashChip_t device{};
		device.type = chipID.type;
		device.reportedCapacity = chipID.capacity;
		device.actualCapacity = log2(parameters->capacity);
		device.eraseInstruction = parameters->sectorEraseOpcode;
		device.erasePageSize = parameters->sectorSize;
		device.flashPageSize = parameters->pageSize;
		// SFDP guarantees 50MHz operation minimum
		device.chipSpeedMHz = 50U;
		return device;
	}

	flashChip_t findChip(const flashID_t chipID, const spiChip_t targetDevice) noexcept
	{
		// Loop through the manufacturers
		for (const auto &mfr : manufacturers)
		{
			if (mfr.manufacturerID == chipID.manufacturer)
			{
				// Once we've found a matching manufacturer, loop through their devices
				for (const auto &chip : mfr)
				{
					if (chip == chipID)
					{
						// If we find the chip in the chipDB, then reclock to its designated speed
						spiSetClock(targetDevice, chip.chipSpeedMHz);
						return chip;
					}
				}
				// Once we've exhausted this manufactuer's chipDB, exit out and check SFDP instead
				break;
			}
		}
		// If we didn't find the chip in the database, no worries - read the SFDP data if available
		const auto parameters{readSFDP(chipID, targetDevice)};
		if (parameters)
			return *parameters;
		// If we could not read the SFDP data then fabricate something based on some sensible fallbacks
		// NB: This leaves the bus set to 500kHz as a safe bet (hence the 0 for chipSpeedMHz)
		return {chipID.type, chipID.capacity, chipID.capacity, 0xD8, 64_KiB, 256, 0};
	}
} // namespace flash
