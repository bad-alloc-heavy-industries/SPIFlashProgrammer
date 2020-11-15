// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_TYPES__HXX
#define USB_TYPES__HXX

#include <cstdint>

namespace usb
{
	// Reserve space for EP0 In + Out, and EP1 In + Out.
	constexpr static const uint8_t endpointCount{4};
	constexpr static const uint8_t epBufferSize{64};

	constexpr static const uint16_t vid{0x1209};
	constexpr static const uint16_t pid{0xAB0C};

	constexpr static const uint8_t configDescriptorCount{1};
	constexpr static const uint8_t interfaceDescriptorCount{1};
	constexpr static const uint8_t endpointDescriptorCount{2};
	constexpr static const uint8_t stringCount{4};

	extern bool usbHandleStandardRequest() noexcept;
}

#endif /*USB_TYPES__HXX*/
