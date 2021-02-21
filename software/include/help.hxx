// SPDX-License-Identifier: BSD-3-Clause
#ifndef HELP__HXX
#define HELP__HXX

#include <string_view>

using namespace std::literals::string_view_literals;

namespace flashprog
{
	constexpr static auto helpString{R"(SPIFlashProgrammer - SPI Flash programming utility

Usage:
	flashprog [options]
	flashprog {operation} <[--device N] N file>

Options:
	--version       Print the version information for flashprog
	-h, --help      Prints this help message

Operations:
	listDevices     Lists the available SPIFlashProgrammers attached to your system
	list            Lists the available Flash chips a given SPIFlashProgrammer can see
	read            Reads the contents of a specific Flash chip into the requested file
	write           Writes the contents of the requested file into a specific Flash chip
	verifiedWrite   Does the same as write, but verifies the contents of the Flash chip after writing

Options for list, read, write and verifiedWrite:
	--device        The SPIFlashProgrammer to use for the operation

Options for read, write and verifiedWrite:
	N               The flash chip to target the operation to
	file            The local file to use for the operation

This utility is licensed under BSD-3-Clase
Report bugs using https://github.com/bad-alloc-heavy-industries/flashprog/issues)"sv
	};
}

#endif /*HELP__HXX*/
