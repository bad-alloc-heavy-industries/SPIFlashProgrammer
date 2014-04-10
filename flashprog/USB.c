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

#include <stdbool.h>
#include <libusb.h>
#include "USB.h"

/*
 * VID = 0x1CBE
 * PID = 0x00FD
 * REV = 0x0100
 * MI = 0x00
 */
static const int interface = 1;
libusb_context *usbContext;
libusb_device_handle *usbDevice;

void usbInit()
{
	if (libusb_init(&usbContext) != 0)
		die("Error: Could not initialise libusb-1.0\n");

	usbDevice = libusb_open_device_with_vid_pid(usbContext, 0x1CBE, 0x00FD);
	if (usbDevice == NULL)
	{
		libusb_exit(usbContext);
		die("Error: Could not find a Tiva C Launchpad to connect to\n");
	}

	libusb_set_auto_detach_kernel_driver(usbDevice, true);
	if (libusb_claim_interface(usbDevice, interface) != 0)
	{
		libusb_close(usbDevice);
		libusb_exit(usbContext);
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
int32_t usbWrite(uint8_t *data, int32_t dataLen)
{
	int32_t actualLen;
	libusb_bulk_transfer(usbDevice, 1 | LIBUSB_ENDPOINT_OUT, data, dataLen, &actualLen, 1);
	return actualLen;
}

int32_t usbWriteByte(uint8_t data)
{
	return usbWrite(&data, 1);
}

/* TODO: check for errors in libusb_bulk_transfer() */
int32_t usbRead(uint8_t *data, int32_t dataLen)
{
	int32_t actualLen;
	libusb_bulk_transfer(usbDevice, 2 | LIBUSB_ENDPOINT_IN, data, dataLen, &actualLen, 1);
	return actualLen;
}

int32_t usbReadByte(uint8_t *data)
{
	return usbRead(data, 1);
}

