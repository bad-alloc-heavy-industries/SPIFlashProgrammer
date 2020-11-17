// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_TYPES__HXX
#define USB_TYPES__HXX

#include <cstdint>
#include <tuple>

namespace usbTypes
{
	// Reserve space for EP0 In + Out, and EP1 In + Out.
	constexpr static const uint8_t endpointCount{2};
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

	enum class buffer_t : uint8_t
	{
		first = 0x00U,
		second = 0x40U
	};

	struct usbEP_t final
	{
	private:
		uint8_t value{};

	public:
		usbEP_t() = default;

		void endpoint(const uint8_t num) noexcept { value |= num & 0x0FU; }
		uint8_t endpoint() const noexcept { return value & 0x0FU; }

		void dir(const endpointDir_t dir) noexcept
		{
			value &= 0x7FU;
			value |= uint8_t(dir);
		}

		endpointDir_t dir() const noexcept
		{
			return value & 0x80U ?
				endpointDir_t::controllerIn :
				endpointDir_t::controllerOut;
		}

		void buffer(const buffer_t choise) noexcept
		{
			value &= 0xBF;
			value |= uint8_t(choise);
		}

		buffer_t buffer() const noexcept
		{
			return value & 0x40U ?
				buffer_t::first :
				buffer_t::second;
		}
	};

	using answer_t = std::tuple<response_t, void *, std::size_t>;
} // namespace usbTypes

extern usbTypes::usbEP_t usbPacket;
extern usbTypes::ctrlState_t usbCtrlState;

#endif /*USB_TYPES__HXX*/
