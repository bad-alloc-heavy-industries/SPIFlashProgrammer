// SPDX-License-Identifier: BSD-3-Clause
#ifndef FLASH_HXX
#define FLASH_HXX

#include <cstdint>
#include <array>
#include "usbProtocol.hxx"

enum class spiChip_t
{
	local1,
	local2,
	target,
	none
};

struct flashID_t
{
	uint8_t manufacturer;
	uint8_t type;
	uint8_t capacity;
};

struct flashChip_t
{
	uint8_t type;
	uint8_t reportedCapacity;
	uint8_t actualCapacity;
	uint8_t eraseInstruction;
	flashProto::page_t erasePageSize;
	flashProto::page_t flashPageSize;
	uint8_t chipSpeedMHz;

	flashChip_t() = delete;
	constexpr bool operator ==(const flashID_t id) const noexcept
		{ return type == id.type && reportedCapacity == id.capacity; }
};

namespace flash
{
	flashChip_t findChip(flashID_t chipID, spiChip_t targetDevice) noexcept;
}

#endif /*FLASH_HXX*/
