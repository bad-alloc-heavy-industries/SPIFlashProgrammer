// SPDX-License-Identifier: BSD-3-Clause
#ifndef HELP_HXX
#define HELP_HXX

#include <string_view>

using namespace std::literals::string_view_literals;

namespace flashprog
{
	constexpr static auto helpString{R"(SPIFlashProgrammer - SPI Flash programming utility

Usage:
	flashprog [options]
	flashprog {operation} <[--device N] --chip bus:N file>

Options:
	--version       Print the version information for flashprog
	-h, --help      Prints this help message

Operations:
	listDevices     Lists the available SPIFlashProgrammers attached to your system
	                (This is the default if no operation is given)
	list            Lists the available Flash chips a given SPIFlashProgrammer can see
	read            Reads the contents of a specific Flash chip into the requested file
	write           Writes the contents of the requested file into a specific Flash chip
	verifiedWrite   Does the same as write, but verifies the contents of the Flash chip after writing
	erase           Performs a full chip erases on the requested Flash chip
	sfdp            Reads and dumps the SFDP data from the requested Flash chip

Options for list, read, write, verifiedWrite, erase and sfdp:
	--device        The SPIFlashProgrammer to use for the operation

Options for read, write, verifiedWrite, erase and sfdp:
	--chip bus:N    Specifies what Flash chip on which bus you want to target.
	                The chip specification works as follows:
	                'bus' can be one of 'int' or 'ext' representing the internal (on-chip)
	                and external (8-pin Flash connector) SPI busses.
	                N is a number from 0 to 255 which specifies a detected Flash chip as given by the
	                listDevices operation

Options for read, write and verifiedWrite:
	file            The local file to use for the operation

This utility is licensed under BSD-3-Clase
Report bugs using https://github.com/bad-alloc-heavy-industries/flashprog/issues)"sv
	};
} // namespace flashprog

#endif /*HELP_HXX*/
