#ifndef USB_PROTOCOL__HXX
#define USB_PROTOCOL__HXX

#include <stdint>

namespace flashProto
{
	enum class messages_t final : uint8_t
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
};

#endif /*USB_PROTOCOL__HXX*/
