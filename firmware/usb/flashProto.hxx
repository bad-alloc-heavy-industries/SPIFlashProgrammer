#ifndef USB_FLASH_PROTO___HXX
#define USB_FLASH_PROTO___HXX

#include <usb/types.hxx>

namespace usb::flashProto
{
	extern void registerHandlers(uint8_t inEP, uint8_t outEP, uint8_t config) noexcept;
} // namespace usb::flashProto

#endif /*USB_FLASH_PROTO___HXX*/
