#ifndef USB_INTERFACE_H
#define USB_INTERFACE_H

typedef enum usbCommand
{
	CMD_START,
	CMD_PAGE,
	CMD_STOP,
	CMD_ABORT,
	CMD_INVALID = 0xFF
} usbCommand;

typedef enum usbReplys
{
	RPL_FAIL = 0,
	RPL_OK = 1
} usbReplys;

#endif /*USB_INTERFACE_H*/
