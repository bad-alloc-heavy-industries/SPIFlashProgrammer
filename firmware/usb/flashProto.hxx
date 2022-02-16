// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_FLASH_PROTO_HXX
#define USB_FLASH_PROTO_HXX

#include <usb/types.hxx>

namespace usb::flashProto
{
	void registerHandlers(uint8_t inEP, uint8_t outEP, uint8_t interface, uint8_t config) noexcept;
} // namespace usb::flashProto

#endif /*USB_FLASH_PROTO_HXX*/
