// SPDX-License-Identifier: BSD-3-Clause
#ifndef FLASH___HXX
#define FLASH___HXX

#include <cstdint>
#include <array>
#include "usbProtocol.hxx"

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

	flashChip_t() = delete;
	constexpr bool operator ==(const flashID_t id) const noexcept
		{ return type == id.type && reportedCapacity == id.capacity; }
};

namespace flash
{
	flashChip_t findChip(flashID_t chipID) noexcept;
};

#endif /*FLASH___HXX*/
