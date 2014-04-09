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
#include "strUtils.h"
#include "USBInterface.h"
/*
 * USB transfer protocol:
 *
 * CMD_START + 4 bytes => uint32_t length of data total
 * CMD_PAGE + 1 bytes + up to 256 bytes => uint8_t page length (0 == 256), page data
 * CMD_ABORT => Sent to indicate user requested to abort
 * CMD_STOP => Sent at the end of transfering all the data to indicate we think we've finished.
 *   Device replies with some data indicating the status of the flash device and if there are any remaining expected bytes.
 *
 * After sending each command, including CMD_STOP, the device must respond with the command code and a byte indicating whether
 * it could execute it correctly - 1 for OK, 0 for error.
 */

/*
 * VID = 0x1CBE
 * PID = 0x00FD
 * REV = 0x0100
 * MI = 0x00
 */
static const int interface = 0;
libusb_context *usbContext;
libusb_device_handle *usbDevice;

int usage(char *prog)
{
	printf("Usage:\n"
		"\t%s binfile.bin\n", prog);
	return 1;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return usage(argv[0]);
	else if (libusb_init(&usbContext) != 0)
		die("Error: Could not initialise libusb-1.0\n");

	usbDevice = libusb_open_device_with_vid_pid(usbContext, 0x1CBE, 0x00FD);
	if (usbDevice == NULL)
	{
		libusb_exit(usbContext);
		die("Error: Could not find a Tiva C Launchpad to connect to\n");
	}

	libusb_set_auto_detach_kernel_driver(usbDevice, true);
	libusb_claim_interface(usbDevice, interface);

	libusb_bulk_transfer(usbDevice, LIBUSB_ENDPOINT_OUT, );

	libusb_release_interface(usbDevice, interface);
	libusb_close(usbDevice);
	libusb_exit(usbContext);
	return 0;
}
