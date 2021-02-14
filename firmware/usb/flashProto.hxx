#ifndef USB_FLASH_PROTO__HXX
#define USB_FLASH_PROTO__HXX

namespace usb::flashProto
{
	void init() noexcept;
	void handlePacket() noexcept;
} // namespace usb::flashProto

#endif /*USB_FLASH_PROTO__HXX*/
