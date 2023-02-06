// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/utility>
#include <substrate/units>
#include "flash.hxx"
#include "sfdp.hxx"

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

	static std::optional<flashChip_t> readSFDP(const flashID_t chipID, const spiChip_t targetDevice) noexcept
	{
		const auto parameters{sfdp::parameters(targetDevice)};
		if (!parameters)
			return std::nullopt;
		flashChip_t device{};
		device.type = chipID.type;
		device.reportedCapacity = chipID.capacity;
		device.actualCapacity = parameters->capacity;
		device.eraseInstruction = parameters->sectorEraseOpcode;
		device.erasePageSize = parameters->sectorSize;
		device.flashPageSize = parameters->pageSize;
		return device;
	}

	flashChip_t findChip(const flashID_t chipID, const spiChip_t targetDevice) noexcept
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
		// If we didn't find the chip in the database, no worries - read the SFDP data if available
		const auto parameters{readSFDP(chipID, targetDevice)};
		if (parameters)
			return *parameters;
		// If we could not read the SFDP data then fabricate something based on some sensible fallbacks
		return {chipID.type, chipID.capacity, chipID.capacity, 0xD8, 64_KiB, 256};
	}
} // namespace flash
