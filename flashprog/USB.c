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
static const int interface = 0;
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

void usbWrite()
{
//	libusb_bulk_transfer(usbDevice, LIBUSB_ENDPOINT_OUT, );
}

void usbRead()
{
//	libusb_bulk_transfer(usbDevice, LIBUSB_ENTPOINT_IN, );
}

