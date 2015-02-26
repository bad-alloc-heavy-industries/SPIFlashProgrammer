/*
 * This file is part of SPI Flash Programmer (SPIFP)
 * Copyright © 2014 Rachel Mant (dx-mon@users.sourceforge.net)
 *
 * SPIFP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SPIFP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include "USB.h"
#include "USBTypes.h"
#include "USBRequests.h"

#define USB_VID					0x0
#define USB_PID					0x0

#define USB_NUM_STRING_DESC		4

#define USB_EPDIR_IN			0x80
#define USB_EPDIR_OUT			0x00

const usbDeviceDescriptor_t usbDeviceDesc =
{
	sizeof(usbDeviceDescriptor_t),
	USB_DESCRIPTOR_DEVICE,
	0x0200, /* this is 2.00 in USB's BCD format */
	USB_CLASS_COMMS,
	USB_SUBCLASS_NONE,
	USB_PROTOCOL_NONE,
	USB_EP0_SETUP_LEN,
	USB_VID,
	USB_PID,
	0x0001, /* BCD encoded device version */
	0x01, /* Manufacturer string index */
	0x02, /* Product string index */
	0x00, /* Temporarily do not support a serial number string */
	USB_NUM_CONFIG_DESC /* One configuration only */
};

const struct
{
	usbStringDescBase_t header;
	uint16_t ids[1];
} usbStringLangIDs =
{
	{
		sizeof(usbStringLangIDs),
		USB_DESCRIPTOR_STRING
	},
	{ 0x0409 } /* This encodes the ID code for US English so we work nicely with windows */
};

const struct
{
	usbStringDescBase_t header;
	uint16_t strMfr[11];
} usbStringMfr =
{
	{
		sizeof(usbStringMfr),
		USB_DESCRIPTOR_STRING
	},
	{
		/* Manufacturer string is "Rachel Mant" */
		'R', 'a', 'c', 'h', 'e', 'l', ' ', 'M', 'a', 'n', 't'
	}
};

const struct
{
	usbStringDescBase_t header;
	uint16_t strProduct[18];
} usbStringProduct =
{
	{
		sizeof(usbStringProduct),
		USB_DESCRIPTOR_STRING
	},
	{
		/* Product string is "SPIFlashProgrammer" */
		'S', 'P', 'I', 'F', 'l', 'a', 's', 'h', 'P', 'r', 'o', 'g',
		'r', 'a', 'm', 'm', 'e', 'r'
	}
};

const struct
{
	usbStringDescBase_t header;
	uint16_t strUART[8];
} usbStringUART =
{
	{
		sizeof(usbStringUART),
		USB_DESCRIPTOR_STRING
	},
	{
		/* Primary configuration string is "USB UART" */
		'U', 'S', 'B', ' ', 'U', 'A', 'R', 'T'
	}
};

const usbStringDescBase_t *usbStrings[USB_NUM_STRING_DESC] =
{
	&usbStringLangIDs.header,
	&usbStringMfr.header,
	&usbStringProduct.header,
	&usbStringUART.header
};

void usbRequestGetDescriptor()
{
}

void usbRequestSetConfiguration()
{
}

void usbRequestGetStatus()
{
}

void usbRequestDoFeature()
{
}

