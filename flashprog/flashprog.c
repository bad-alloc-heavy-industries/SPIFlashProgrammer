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
#include "strUtils.h"
#include "USB.h"
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
	usbInit();

	writeUSB();
	readUSB();

	usbDeinit();
	return 0;
}
