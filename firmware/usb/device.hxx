// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_DEVICE__HXX
#define USB_DEVICE__HXX

#include <cstdint>
#include <cstring>
#include "types.hxx"

extern usbTypes::deviceState_t usbState;
extern usbTypes::usbEP_t usbPacket;
extern usbTypes::ctrlState_t usbCtrlState;

extern std::array<usbTypes::usbEPStatus_t, usbTypes::endpointCount> usbStatusInEP;
extern std::array<usbTypes::usbEPStatus_t, usbTypes::endpointCount> usbStatusOutEP;

extern void usbServiceCtrlEP() noexcept;

namespace usbTypes
{
	enum class request_t : uint8_t
	{
		getStatus = 0,
		clearFeature = 1,
		setFeature = 3,
		setAddress = 5,
		getDescriptor = 6,
		setDescriptor = 7,
		getConfiguration = 8,
		setConfiguration = 9,
		getInterface = 10,
		setInterface = 11,
		syncFrame = 12
	};

	namespace setupPacket
	{
		enum class request_t : uint8_t
		{
			typeStandard = 0x00,
			typeClass = 0x20U,
			typeVendor = 0x40U
		};

		enum class recipient_t : uint8_t
		{
			device = 0,
			interface = 1,
			endpoint = 2,
			other = 3
		};

		struct requestType_t final
		{
		private:
			uint8_t value{};

		public:
			requestType_t() = default;

			[[nodiscard]] recipient_t recipient() const noexcept
				{ return static_cast<recipient_t>(value & 0x1FU); }
			[[nodiscard]] request_t type() const noexcept
				{ return static_cast<request_t>(value & 0x60U); }
			[[nodiscard]] endpointDir_t dir() const noexcept
				{ return static_cast<endpointDir_t>(value & 0x80U); }
		};

		struct descriptor_t final
		{
			uint8_t index;
			uint8_t type;
		};

		struct address_t final
		{
			uint8_t addrL;
			uint8_t addrH;
		};

		struct value_t final
		{
		private:
			uint16_t value{};

		public:
			value_t() = default;
			[[nodiscard]] operator uint16_t() const noexcept { return value; }

			[[nodiscard]] descriptor_t asDescriptor() const noexcept
			{
				descriptor_t result{};
				std::memcpy(&result, &value, sizeof(value));
				return result;
			}

			[[nodiscard]] address_t asAddress() const noexcept
			{
				address_t result{};
				std::memcpy(&result, &value, sizeof(value));
				return result;
			}
		};

		struct index_t final
		{
		private:
			uint8_t valueL{};
			uint8_t valueH{};

		public:
			index_t() = default;
			[[nodiscard]] operator uint16_t() const noexcept { return (uint16_t{valueH} << 8) | valueL; }

			[[nodiscard]] endpointDir_t dir() const noexcept { return static_cast<endpointDir_t>(valueL & 0x80U); }
			[[nodiscard]] uint8_t endpoint() const noexcept { return valueL & 0x0FU; }
		};
	} // namespace setupPacket

	struct setupPacket_t final
	{
		setupPacket::requestType_t requestType{};
		request_t request{};
		setupPacket::value_t value;
		setupPacket::index_t index;
		uint16_t length;

		setupPacket_t() = default;
	};
} // namespace usb

#endif /*USB_DEVICE__HXX*/
