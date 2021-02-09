// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_CORE__HXX
#define USB_CORE__HXX

#include "usb/types.hxx"

namespace usbCore
{
	extern usbTypes::deviceState_t usbState;
	extern usbTypes::usbEP_t usbPacket;
	extern usbTypes::ctrlState_t usbCtrlState;

	extern std::array<usbTypes::usbEPStatus_t<const void>, usbTypes::endpointCount> epStatusControllerIn;
	extern std::array<usbTypes::usbEPStatus_t<void>, usbTypes::endpointCount> epStatusControllerOut;

	extern const uint8_t *sendData(const uint8_t ep, const uint8_t *const buffer, const uint8_t length) noexcept;
	extern uint8_t *recvData(const uint8_t ep, uint8_t *const buffer, const uint8_t length) noexcept;

	extern void usbResetEPs(bool includeEP0 = false) noexcept;
} // namespace usbCore

#endif /*USB_CORE__HXX*/
