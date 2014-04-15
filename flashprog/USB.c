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

#include <stdio.h>
#include <stdbool.h>
#include <libusb.h>
#include "USB.h"

typedef struct libusb_device_descriptor libusb_device_descriptor;
typedef struct libusb_config_descriptor libusb_config_descriptor;
typedef struct libusb_interface_descriptor libusb_interface_descriptor;
typedef struct libusb_endpoint_descriptor libusb_endpoint_descriptor;

#pragma pack(push, 1)
typedef struct usbIfaceAssoc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bFirstInterface;
	uint8_t bInterfaceCount;
	uint8_t bFunctionClass;
	uint8_t bFunctionSubClass;
	uint8_t bFunctionProtocol;
	uint8_t iInterface;
} usbIfaceAssoc;

typedef struct usbCDCConfig
{
	/* Header */
	uint8_t bHeaderLen;
	uint8_t bHeaderDesc;
	uint8_t bHeaderType;
	uint16_t bcdCDC;

	/* ACM */
	uint8_t bACMLen;
	uint8_t bACMDesc;
	uint8_t bACMType;
	uint8_t bmACMCapabilities;

	/* Unions */
	uint8_t bUnionLen;
	uint8_t bUnionDesc;
	uint8_t bUnionType;
	uint8_t iMasterInterface;
	uint8_t iSlaveInterface;

	/* Call Management */
	uint8_t bCallLen;
	uint8_t bCallDesc;
	uint8_t bCallType;
	uint8_t bmCallCapabilities;
	uint8_t iDataInterface;
} usbCDCConfig;
#pragma pack(pop)

/*
 * VID = 0x1CBE
 * PID = 0x00FD
 * REV = 0x0100
 * MI = 0x00
 */
libusb_context *usbContext;
libusb_device_handle *usbDevice;

int ctrlInterface, dataInterface;
uint8_t ctrlEndpoint, inEndpoint, outEndpoint;

#define CDC_SET_LINE_CODING 0x20
#define CDC_GET_LINE_CODING 0x21
#define CDC_SET_CONTROL_LINE_STATE 0x22
#define CTRL_LEN 32
uint8_t ctrlData[CTRL_LEN];

void usbInitCleanup()
{
	libusb_close(usbDevice);
	libusb_exit(usbContext);
}

void usbInit()
{
	int res, actualLen;
	libusb_device *usbRawDevice;
	libusb_device_descriptor usbDevDesc;
	libusb_config_descriptor *usbConfigDesc;
	const libusb_interface_descriptor *usbIface;
	const libusb_endpoint_descriptor *usbEndpointDesc;
	const usbIfaceAssoc *usbInterfaceAssoc;
	const usbCDCConfig *usbCDCDesc;

	if (libusb_init(&usbContext) != 0)
		die("Error: Could not initialise libusb-1.0\n");

	usbDevice = libusb_open_device_with_vid_pid(usbContext, 0x1CBE, 0x00FD);
	if (usbDevice == NULL)
	{
		libusb_exit(usbContext);
		die("Error: Could not find a Tiva C Launchpad to connect to\n");
	}
	usbRawDevice = libusb_get_device(usbDevice);

	if (libusb_get_device_descriptor(usbRawDevice, &usbDevDesc) != 0 || usbDevDesc.bNumConfigurations != 1)
	{
		usbInitCleanup();
		die("Error: libusb could not get the device descriptor for the Tiva C Launchpad\n");
	}

	if (libusb_get_config_descriptor(usbRawDevice, 0, &usbConfigDesc) != 0)
	{
		usbInitCleanup();
		die("Error: libusb could not get the configuration descriptor for the Tiva C Launchpad\n");
	}
	else if (usbConfigDesc->bLength != 9 || usbConfigDesc->bDescriptorType != 2 ||
		usbConfigDesc->bNumInterfaces != 4 || usbConfigDesc->extra_length != sizeof(usbIfaceAssoc))
	{
		libusb_free_config_descriptor(usbConfigDesc);
		usbInitCleanup();
		die("Error: The descriptor returned by the device claiming to be a Tiva C Launchpad is invalid\n");
	}

	usbInterfaceAssoc = (const usbIfaceAssoc *)usbConfigDesc->extra;
	if (usbInterfaceAssoc->bLength != 8 || usbInterfaceAssoc->bDescriptorType != 11 ||
		usbInterfaceAssoc->bInterfaceCount != 2 || usbInterfaceAssoc->bFirstInterface >= usbConfigDesc->bNumInterfaces)
	{
		libusb_free_config_descriptor(usbConfigDesc);
		usbInitCleanup();
		die("Error: The interface association returned by the device claiming to be the Tiva C Launchpad is invalid\n");
	}

	usbIface = usbConfigDesc->interface[usbInterfaceAssoc->bFirstInterface].altsetting;
	if (usbIface->bNumEndpoints != 1 || usbIface->extra_length != sizeof(usbCDCConfig))
	{
		libusb_free_config_descriptor(usbConfigDesc);
		usbInitCleanup();
		die("Error: The interface descriptor that is supposed to be for the control interface is invalid\n");
	}
	usbCDCDesc = (const usbCDCConfig *)usbIface->extra;
	if (usbCDCDesc->bHeaderLen != 5 || usbCDCDesc->bcdCDC != 0x0110 ||
		usbCDCDesc->bACMLen != 4 || usbCDCDesc->bUnionLen != 5 ||
		usbCDCDesc->bCallLen != 5)
	{
		libusb_free_config_descriptor(usbConfigDesc);
		usbInitCleanup();
		die("Error: The CDC descriptor that is provided by the control interface is invalid\n");
	}
	usbEndpointDesc = &usbIface->endpoint[0];
	ctrlEndpoint = usbEndpointDesc->bEndpointAddress;

	usbIface = usbConfigDesc->interface[usbCDCDesc->iDataInterface].altsetting;
	if (usbIface->bNumEndpoints != 2 || usbIface->extra_length != 0)
	{
		libusb_free_config_descriptor(usbConfigDesc);
		usbInitCleanup();
		die("Error: The interface descriptor that is supposed to be for the data interface is invalid\n");
	}
	usbEndpointDesc = &usbIface->endpoint[0];
	inEndpoint = usbEndpointDesc->bEndpointAddress;
	usbEndpointDesc = &usbIface->endpoint[1];
	outEndpoint = usbEndpointDesc->bEndpointAddress;

	ctrlInterface = usbInterfaceAssoc->bFirstInterface;
	dataInterface = usbCDCDesc->iDataInterface;
	libusb_set_configuration(usbDevice, usbConfigDesc->bConfigurationValue);
	libusb_free_config_descriptor(usbConfigDesc);

	libusb_set_auto_detach_kernel_driver(usbDevice, true);
	if (libusb_claim_interface(usbDevice, ctrlInterface) != 0 ||
		libusb_claim_interface(usbDevice, dataInterface) != 0)
	{
		usbDeinit();
		die("Error: Could not claim the Tiva C Launchpad virtual serial port interface\n");
	}

	/* Set the port baud rate */
	*((uint32_t *)ctrlData) = 115200;
	/* 1 stop bit, no parity, 8-bit */
	ctrlData[4] = 0;
	ctrlData[5] = 0;
	ctrlData[6] = 8;
	res = libusb_control_transfer(usbDevice, 0x21, CDC_SET_LINE_CODING, 0, ctrlInterface, ctrlData, 7, 10);
	if (res != 7)
	{
		usbDeinit();
		die("libusb returned %d: %s\n", res, libusb_strerror(res));
	}

	res = libusb_control_transfer(usbDevice, 0xA1, CDC_GET_LINE_CODING, 0, ctrlInterface, ctrlData, 7, 10);
	if (res != 7)
	{
		usbDeinit();
		die("libusb returned %d: %s\n", res, libusb_strerror(res));
	}

	res = libusb_control_transfer(usbDevice, 0x21, CDC_SET_CONTROL_LINE_STATE, 0, ctrlInterface, NULL, 0, 10);
	if (res != 0)
	{
		usbDeinit();
		die("libusb returned %d: %s\n", res, libusb_strerror(res));
	}
}

void usbDeinit()
{
	libusb_release_interface(usbDevice, dataInterface);
	libusb_release_interface(usbDevice, ctrlInterface);
	libusb_close(usbDevice);
	libusb_exit(usbContext);
}

/* TODO: check for errors in libusb_bulk_transfer() */
int32_t usbWrite(void *data, int32_t dataLen)
{
	int32_t actualLen, error;
	error = libusb_bulk_transfer(usbDevice, outEndpoint, data, dataLen, &actualLen, 10);
	if (error != 0)
	{
		printf("Error: libusb_bulk_transfer(%d => %s) write failed\n", error, libusb_strerror(error));
		return 0;
	}
	return actualLen;
}

int32_t usbWriteByte(uint8_t data)
{
	return usbWrite(&data, 1);
}

/* TODO: check for errors in libusb_bulk_transfer() */
int32_t usbRead(void *data, int32_t dataLen)
{
	int32_t actualLen, recvLen, error;
	recvLen = 0;
	while (recvLen < dataLen)
	{
		error = libusb_bulk_transfer(usbDevice, inEndpoint, data + recvLen, dataLen - recvLen, &actualLen, 100);
		if (error != 0)
		{
			printf("Error: libusb_bulk_transfer(%d => %s) read failed\n", error, libusb_strerror(error));
			return 0;
		}
		recvLen += actualLen;
	}
	return recvLen;
}

int32_t usbReadByte(uint8_t *data)
{
	return usbRead(data, 1);
}

