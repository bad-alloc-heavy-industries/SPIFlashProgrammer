// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_TYPES__HXX
#define USB_TYPES__HXX

#include <cstdint>
#include <tuple>

namespace usbTypes
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

	enum class ctrlState_t
	{
		idle,
		wait,
		tx,
		rx
	};

	enum class deviceState_t
	{
		detached,
		attached,
		powered,
		waiting,
		addressing,
		addressed,
		configured
	};

	enum class response_t
	{
		data,
		zeroLengthResponse,
		unhandled,
		stall
	};

	enum class endpointDir_t : uint8_t
	{
		controllerOut = 0x00U,
		controllerIn = 0x80U
	};

	using answer_t = std::tuple<response_t, void *, std::size_t>;
} // namespace usbTypes

#endif /*USB_TYPES__HXX*/
