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

#define USB_NUM_CONFIG_DESC		1
#define USB_NUM_IFACE_DESC		2
#define USB_NUM_ENDPOINT_DESC	3
#define USB_NUM_CONFIG_SECS		11
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

const usbConfigDescriptor_t usbConfigDesc[USB_NUM_CONFIG_DESC] =
{
	{
		sizeof(usbConfigDescriptor_t),
		USB_DESCRIPTOR_CONFIGURATION,
		sizeof(usbConfigDescriptor_t) + sizeof(usbInterfaceAssocDescriptor_t) +
		sizeof(usbInterfaceDescriptor_t) + sizeof(usbCDCHeader_t) +
		sizeof(usbCDCHeaderACM_t) + sizeof(usbCDCUnion2_t) + sizeof(usbCDCCallMgmt_t) +
		sizeof(usbEndpointDescriptor_t) + sizeof(usbInterfaceDescriptor_t) +
		sizeof(usbEndpointDescriptor_t) + sizeof(usbEndpointDescriptor_t),
		0x02, /* One interface */
		0x01, /* This is the first configuration */
		0x03, /* Configuration string index */
		USB_CONF_ATTR_DEFAULT | USB_CONF_ATTR_SELFPWR,
		50 /* 100mA max */
	}
};

const usbInterfaceDescriptor_t usbInterfaceDesc[USB_NUM_IFACE_DESC] =
{
	{
		sizeof(usbInterfaceDescriptor_t),
		USB_DESCRIPTOR_INTERFACE,
		0x00, /* 0th interface, by which we mean 1st */
		0x00, /* Alternate 0 */
		0x01, /* Two endpoints to the interface */
		USB_CLASS_COMMS,
		USB_SUBCLASS_ACM,
		USB_PROTOCOL_NONE,
		0x00 /* No string to describe this interface */
	},
	{
		sizeof(usbInterfaceDescriptor_t),
		USB_DESCRIPTOR_INTERFACE,
		0x01, /* 1st interface, by which we mean 2nd */
		0x00, /* Alternate 0 */
		0x02, /* One endpoint to the interface */
		USB_CLASS_DATA,
		USB_SUBCLASS_NONE,
		USB_PROTOCOL_NONE,
		0x00 /* No string to describe this interface */
	}
};

const usbEndpointDescriptor_t usbEndpointDesc[USB_NUM_ENDPOINT_DESC] =
{
	{
		sizeof(usbEndpointDescriptor_t),
		USB_DESCRIPTOR_ENDPOINT,
		USB_EPDIR_IN | 2,
		USB_EPTYPE_INTR,
		USB_EP2_IN_LEN,
		0x01 /* Poll once per frame */
	},
	{
		sizeof(usbEndpointDescriptor_t),
		USB_DESCRIPTOR_ENDPOINT,
		USB_EPDIR_IN | 1,
		USB_EPTYPE_BULK,
		USB_EP1_IN_LEN,
		0x01 /* Poll once per frame */
	},
	{
		sizeof(usbEndpointDescriptor_t),
		USB_DESCRIPTOR_ENDPOINT,
		USB_EPDIR_OUT | 1,
		USB_EPTYPE_BULK,
		USB_EP1_OUT_LEN,
		0x01 /* Poll once per frame */
	}
};

const usbInterfaceAssocDescriptor_t usbInterfaceAssocDesc =
{
	sizeof(usbInterfaceAssocDescriptor_t),
	USB_DESCRIPTOR_INTERFACE_ASSOCIATION,
	0, /* First interface is #1 */
	2, /* Two interfaces involved contiguously */
	USB_CLASS_COMMS,
	USB_SUBCLASS_ACM,
	USB_PROTOCOL_NONE,
	0x03 /* Configuration string index */
};

const usbCDCHeader_t usbCDCHeader =
{
	sizeof(usbCDCHeader_t),
	USB_DESCRIPTOR_CDC,
	USB_CDC_HEADER,
	0x0110
};

const usbCDCHeaderACM_t usbCDCHeaderACM =
{
	sizeof(usbCDCHeaderACM_t),
	USB_DESCRIPTOR_CDC,
	USB_CDC_ACM,
	USB_ACM_LINE_CODING /* Set break here does not make any sense */
};

const usbCDCUnion2_t usbCDCUnion =
{
	sizeof(usbCDCUnion2_t),
	USB_DESCRIPTOR_CDC,
	USB_CDC_UNION,
	0,
	1
};

const usbCDCCallMgmt_t usbCDCCallMgmt =
{
	sizeof(usbCDCCallMgmt_t),
	USB_DESCRIPTOR_CDC,
	USB_CDC_CM,
	USB_CDC_CM_SELF_MANAGE
};

const usbMultiPartDesc_t usbConfigSecs[USB_NUM_CONFIG_SECS] =
{
	{
		sizeof(usbConfigDescriptor_t),
		&usbConfigDesc[0]
	},
	{
		sizeof(usbInterfaceAssocDescriptor_t),
		&usbInterfaceAssocDesc
	},
	{
		sizeof(usbInterfaceDescriptor_t),
		&usbInterfaceDesc[0]
	},
	{
		sizeof(usbCDCHeader_t),
		&usbCDCHeader
	},
	{
		sizeof(usbCDCHeaderACM_t),
		&usbCDCHeaderACM
	},
	{
		sizeof(usbCDCUnion2_t),
		&usbCDCUnion
	},
	{
		sizeof(usbCDCCallMgmt_t),
		&usbCDCCallMgmt
	},
	{
		sizeof(usbEndpointDescriptor_t),
		&usbEndpointDesc[0]
	},
	{
		sizeof(usbInterfaceDescriptor_t),
		&usbInterfaceDesc[1]
	},
	{
		sizeof(usbEndpointDescriptor_t),
		&usbEndpointDesc[1]
	},
	{
		sizeof(usbEndpointDescriptor_t),
		&usbEndpointDesc[2]
	}
};

const usbMultiPartTable_t usbConfigDescs[USB_NUM_CONFIG_DESC] =
{
	{
		USB_NUM_CONFIG_SECS,
		usbConfigSecs
	}
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

