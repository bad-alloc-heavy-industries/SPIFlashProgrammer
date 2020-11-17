// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_DEVICE__HXX
#define USB_DEVICE__HXX

#include <cstdint>
#include "types.hxx"

extern usbTypes::usbEP_t usbPacket;
extern usbTypes::ctrlState_t usbCtrlState;

extern std::array<usbTypes::usbEPStatus_t, usbTypes::endpointCount> usbStatusInEP;
extern std::array<usbTypes::usbEPStatus_t, usbTypes::endpointCount> usbStatusOutEP;

extern void usbServiceCtrlEP() noexcept;

namespace usbTypes
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

	struct setupPacket_t
	{
	private:
		struct requestType_t
		{
		private:
			uint8_t value{};

		public:
			requestType_t() = default;

			[[nodiscard]] recipient_t recipient() const noexcept
				{ return static_cast<recipient_t>(value & 0x1FU; }
			[[nodiscard]] request_t type() const noexcept
				{ return static_cast<request_t>(value & 0x60U); }
			[[nodiscard]] endpointDir_t dir() const noexcept
				{ return static_cast<endpointDir_t>(value & 0x80U); }
		};

		setupPacket_t() = default;
	};
} // namespace usb

#endif /*USB_DEVICE__HXX*/
