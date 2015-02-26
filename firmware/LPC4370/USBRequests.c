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

#include <LPC4370.h>
#include <stdint.h>
#include "USB.h"
#include "USBTypes.h"
#include "USBRequests.h"
#include "USBCDC.h"

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
	USB_CDC_CM_SELF_MANAGE,
	2
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

void usbRequestGetDescriptor(volatile usbSetupPacket_t *packet)
{
	if (packet->requestType.direction == USB_DIR_IN)
	{
		usbStatusInEP[0].buffSrc = USB_BUFFER_SRC_FLASH;
		usbStatusInEP[0].needsArming = 1;

		switch (packet->value.descriptor.type)
		{
			case USB_DESCRIPTOR_DEVICE:
				usbStatusInEP[0].buffer.flashPtr = &usbDeviceDesc;
				usbStatusInEP[0].xferCount = usbDeviceDesc.length;
				break;
			case USB_DESCRIPTOR_CONFIGURATION:
				if (packet->value.descriptor.index < USB_NUM_CONFIG_DESC)
				{
					uint8_t i;
					const usbMultiPartTable_t *configDesc = &usbConfigDescs[packet->value.descriptor.index];
					usbStatusInEP[0].buffer.flashPtr = configDesc->descriptors[0].descriptor;
					usbStatusInEP[0].xferCount = 0;
					for (i = 0; i < configDesc->numDesc; i++)
						usbStatusInEP[0].xferCount += configDesc->descriptors[i].length;
					usbStatusInEP[0].multiPart = 1;
					usbStatusInEP[0].part = 1;
					usbStatusInEP[0].partDesc = configDesc;
					usbStatusInEP[0].partCount = configDesc->descriptors[0].length;
				}
				else
					usbStatusInEP[0].value = 0;
				break;
			case USB_DESCRIPTOR_INTERFACE:
				if (packet->value.descriptor.index < USB_NUM_IFACE_DESC)
				{
					const usbInterfaceDescriptor_t *ifaceDesc = &usbInterfaceDesc[packet->value.descriptor.index];
					usbStatusInEP[0].buffer.flashPtr = ifaceDesc;
					usbStatusInEP[0].xferCount = ifaceDesc->length;
				}
				else
					usbStatusInEP[0].value = 0;
				break;
			case USB_DESCRIPTOR_ENDPOINT:
				if (packet->value.descriptor.index < USB_NUM_ENDPOINT_DESC)
				{
					const usbEndpointDescriptor_t *epDesc = &usbEndpointDesc[packet->value.descriptor.index];
					usbStatusInEP[0].buffer.flashPtr = epDesc;
					usbStatusInEP[0].xferCount = epDesc->length;
				}
				else
					usbStatusInEP[0].value = 0;
				break;
			case USB_DESCRIPTOR_STRING:
				if (packet->value.descriptor.index < USB_NUM_STRING_DESC)
				{
					const usbStringDescBase_t *strDesc = usbStrings[packet->value.descriptor.index];
					usbStatusInEP[0].buffer.flashPtr = strDesc;
					usbStatusInEP[0].xferCount = strDesc->length;
				}
				else
					usbStatusInEP[0].value = 0;
				break;
			default:
				usbStatusInEP[0].value = 0;
		}
	}
}

void usbRequestSetConfiguration(volatile usbSetupPacket_t *packet)
{
	uint8_t i;

	/* Generate a 0 length ack for this */
	usbStatusInEP[0].needsArming = 1;
	/* And reset all non-EP0 endpoints */
	/* Reset all BDT entiries to pristine state */

	/* Reset the ping-pong buffers, and their states */

	/* Reset alternate interface setting and set active config */
	usbActiveConfig = packet->value.config.value;

	if (usbActiveConfig == 0)
		usbState = USB_STATE_ADDRESSED;
	else if (usbActiveConfig <= USB_NUM_CONFIG_DESC)
	{
		uint8_t configIdx = usbActiveConfig - 1;
		uint8_t j, ifaceIdx = 0, endpointIdx = 0;

		usbState = USB_STATE_CONFIGURED;
		/* Count interfaces to arrive at the first to deref */
		for (i = 0; i < configIdx; i++)
		{
			j = ifaceIdx;
			ifaceIdx += usbConfigDesc[i].numInterfaces;
			for (; j < ifaceIdx; j++)
				endpointIdx += usbInterfaceDesc[j].numEndpoints;
		}

		for (i = 0; i < usbConfigDesc[configIdx].numInterfaces; i++)
		{
			for (j = 0; j < usbInterfaceDesc[i].numEndpoints; j++)
			{
				const usbEndpointDescriptor_t *endpoint = &usbEndpointDesc[endpointIdx + j];
				volatile uint32_t *ep = &USB0->epCtrl[endpoint->endpointAddress & 0x7F];
				uint8_t epType = endpoint->attributes & 0x03;

				if ((endpoint->endpointAddress & 0x80) == USB_EPDIR_IN)
				{
					if (epType == USB_EPTYPE_BULK)
						*ep |= USB_EP_TX_TYPE_BULK;
					else if (epType == USB_EPTYPE_INTR)
						*ep |= USB_EP_TX_TYPE_INTR;
					else
						*ep |= USB_EP_TX_TYPE_ISO;
					*ep |= USB_EP_TXE;
				}
				else
				{
					if (epType == USB_EPTYPE_BULK)
						*ep |= USB_EP_RX_TYPE_BULK;
					else if (epType == USB_EPTYPE_INTR)
						*ep |= USB_EP_RX_TYPE_INTR;
					else
						*ep |= USB_EP_RX_TYPE_ISO;
					*ep |= USB_EP_RXE;
				}
			}
			endpointIdx += j;
		}
	}
}

void usbRequestGetStatus(volatile usbSetupPacket_t *packet)
{
	usbEP0Data[0] = usbEP0Data[1] = 0;

	switch (packet->requestType.recipient)
	{
		case USB_RECIPIENT_DEVICE:
			usbStatusInEP[0].needsArming = 1;
			/* We are never remotely woken up, and nor we are self-powered. */
			usbEP0Data[0] |= 0x00;
			break;
		case USB_RECIPIENT_INTERFACE:
			usbStatusInEP[0].needsArming = 1;
			/* The spec mandates this has to return 0 for both bytes */
			break;
		case USB_RECIPIENT_ENDPOINT:
		{
			volatile uint32_t epStat;
			usbStatusInEP[0].needsArming = 1;
			/* Look the endpoint up */
			epStat = USB0->epCtrl[packet->index.epNum];
			/* Check if the endpoint is halted */
			if (packet->index.epDir == USB_DIR_OUT && epStat & USB_EP_RX_STALLED)
				usbEP0Data[0] = 0x01;
			else if (packet->index.epDir == USB_DIR_IN && epStat & USB_EP_TX_STALLED)
				usbEP0Data[0] = 0x01;
			break;
		}
	}

	/* If something generated something to report, set up the endpoint state for it */
	if (usbStatusInEP[0].needsArming)
	{
		usbStatusInEP[0].buffer.memPtr = usbEP0Data;
		usbStatusInEP[0].buffSrc = USB_BUFFER_SRC_MEM;
		usbStatusInEP[0].xferCount = 2;
	}
}

void usbRequestDoFeature(volatile usbSetupPacket_t *packet)
{
	/* Process any eventual Remote Wakeup requests to apease Windows. */
	if (packet->value.feature.value == USB_FEATURE_REMOTE_WAKEUP &&
		packet->requestType.recipient == USB_RECIPIENT_DEVICE)
	{
		usbStatusInEP[0].needsArming = 1;
		return;
	}
}

