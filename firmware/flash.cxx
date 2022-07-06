// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/utility>
#include <substrate/units>
#include "flash.hxx"

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
			{0x32, 0x17, 0x17, 0x20, 4_KiB, 256}
		})
	};

	constexpr static auto numonyxChips
	{
		substrate::make_array<flashChip_t>
		({
			{0x20, 0x14, 0x14, 0xD8, 64_KiB, 256},
			{0x20, 0x15, 0x15, 0xD8, 64_KiB, 256}
		})
	};

	constexpr static auto gigaDeviceChips
	{
		substrate::make_array<flashChip_t>
		({
			{0x40, 0x13, 0x13, 0x20, 4_KiB, 256},
			{0x40, 0x17, 0x17, 0x20, 4_KiB, 256}
		})
	};

	constexpr static auto winbondChips
	{
		substrate::make_array<flashChip_t>
		({
			{0x40, 0x14, 0x14, 0x20, 4_KiB, 256},
			{0x40, 0x16, 0x16, 0x20, 4_KiB, 256},
			{0x40, 0x18, 0x18, 0x20, 4_KiB, 256},
			{0xAA, 0x21, 0x1B, 0xD8, 128_KiB, 2_KiB},
			{0x70, 0x18, 0x18, 0x20, 4_KiB, 256}
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

	flashChip_t findChip(const flashID_t chipID) noexcept
	{
		for (const auto &mfr : manufacturers)
		{
			if (mfr.manufacturerID == chipID.manufacturer)
			{
				for (const auto &chip : mfr)
				{
					if (chip == chipID)
						return chip;
				}
				break;
			}
		}
		// If we didn't find the chip in the database, fabricate something with common defaults
		return {chipID.type, chipID.capacity, chipID.capacity, 0xD8, 64_KiB, 256};
	}
} // namespace flash
