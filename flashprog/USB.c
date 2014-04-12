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

/*
 * VID = 0x1CBE
 * PID = 0x00FD
 * REV = 0x0100
 * MI = 0x00
 */
static const int interface = 1;
libusb_context *usbContext;
libusb_device_handle *usbDevice;

uint8_t ctrlEndpoint, inEndpoint, outEndpoint;

void usbInitCleanup()
{
	libusb_close(usbDevice);
	libusb_exit(usbContext);
}

void usbInit()
{
	libusb_device *usbRawDevice;
	libusb_device_descriptor usbDevDesc;
	libusb_config_descriptor *usbConfigDesc;
	usbIfaceAssoc *usbInterfaceAssoc;

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
		usbConfigDesc->bNumInterfaces != 4 || usbConfigDesc->extra_length != 8)
	{
		libusb_free_config_descriptor(usbConfigDesc);
		usbInitCleanup();
		die("Error: The descriptor returned by the device claiming to be a Tiva C Launchpad is invalid\n");
	}

	usbInterfaceAssoc = (usbIfaceAssoc *)usbConfigDesc->extra;
	if (usbInterfaceAssoc->bLength != 8 || usbInterfaceAssoc->bDescriptorType != 11 ||
		usbInterfaceAssoc->bInterfaceCount != 2)
	{
		libusb_free_config_descriptor(usbConfigDesc);
		usbInitCleanup();
		die("Error: The interface association returned by the device claiming to be the Tiva C Launchpad is invalid\n");
	}

	libusb_free_config_descriptor(usbConfigDesc);

	libusb_set_auto_detach_kernel_driver(usbDevice, true);
	if (libusb_claim_interface(usbDevice, interface) != 0)
	{
		usbInitCleanup();
		die("Error: Could not claim the Tiva C Launchpad UART interface\n");
	}
}

void usbDeinit()
{
	libusb_release_interface(usbDevice, interface);
	libusb_close(usbDevice);
	libusb_exit(usbContext);
}

/* TODO: check for errors in libusb_bulk_transfer() */
int32_t usbWrite(void *data, int32_t dataLen)
{
	int32_t actualLen, error;
	error = libusb_bulk_transfer(usbDevice, 1 | LIBUSB_ENDPOINT_OUT, data, dataLen, &actualLen, 10);
	if (error != 0)
	{
		printf("Error: libusb_bulk_transfer(%d) failed\n", error);
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
	int32_t actualLen, error;
	error = libusb_bulk_transfer(usbDevice, 2 | LIBUSB_ENDPOINT_IN, data, dataLen, &actualLen, 10);
	if (error != 0)
	{
		printf("Error: libusb_bulk_transfer(%d) failed\n", error);
		return 0;
	}
	return actualLen;
}

int32_t usbReadByte(uint8_t *data)
{
	return usbRead(data, 1);
}

