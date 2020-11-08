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

#ifndef USBCDC_H
#define	USBCDC_H

#define USB_DESCRIPTOR_CDC		0x24

#define USB_CDC_HEADER			0x00
#define USB_CDC_CM				0x01
#define USB_CDC_ACM				0x02
#define USB_CDC_DLM 			0x03
#define USB_CDC_TR				0x04
#define USB_CDC_TCLSR			0x05
#define USB_CDC_UNION			0x06
#define USB_CDC_CS				0x07
#define USB_CDC_TOM				0x08
#define USB_CDC_TERM			0x09
#define USB_CDC_NCT				0x0A
#define USB_CDC_PU				0x0B
#define USB_CDC_EU				0x0C
#define USB_CDC_MCM				0x0D
#define USB_CDC_CAPI			0x0E
#define USB_CDC_ETH				0x0F
#define USB_CDC_ATM				0x10

#define USB_CDC_CM_DATA_CHANNEL	0x02
#define USB_CDC_CM_SELF_MANAGE	0x01

#define USB_ACM_NETWORK_CON		0x08
#define USB_ACM_SEND_BREAK		0x04
#define USB_ACM_LINE_CODING		0x02
#define USB_ACM_COMM_FEAT		0x01

typedef struct
{
	uint8_t length;
	uint8_t descriptorType;
	uint8_t descriptorSubtype;
	uint16_t cdcVersion;
} usbCDCHeader_t;

typedef struct
{
	uint8_t length;
	uint8_t descriptorType;
	uint8_t descriptorSubtype;
	uint8_t capabilities;
	uint8_t dataInterface;
} usbCDCCallMgmt_t;

typedef struct
{
	uint8_t length;
	uint8_t descriptorType;
	uint8_t descriptorSubtype;
	uint8_t capabilities;
} usbCDCHeaderACM_t;

typedef struct
{
	uint8_t length;
	uint8_t descriptorType;
	uint8_t descriptorSubtype;
	uint8_t masterInterface;
	uint8_t slaveInterface0;
} usbCDCUnion2_t;

#endif	/* USBCDC_H */

