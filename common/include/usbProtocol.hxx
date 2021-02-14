// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_PROTOCOL__HXX
#define USB_PROTOCOL__HXX

#include <cstdint>

namespace flashProto
{
	enum class messages_t : uint8_t
	{
		deviceCount,
		listDevice,
		targetDevice,
	};

	struct deviceCount_t final
	{
		messages_t type{messages_t::deviceCount};
		uint8_t count{0};
	};
} // namespace flashProto

#endif /*USB_PROTOCOL__HXX*/
