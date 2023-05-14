// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/command_line/options>
#include <substrate/conversions>
#include "usbProtocol.hxx"

namespace flashprog
{
	using namespace std::literals::string_view_literals;
	using namespace substrate::commandLine;

	using chipBus_t = flashProto::flashBus_t;

	struct chip_t
	{
		chipBus_t bus{chipBus_t::unknown};
		uint8_t index{UINT8_MAX};
	};

	static inline std::optional<std::any> chipSelectionParser(const std::string_view &value) noexcept
	{
		// Check if we have enough characters to form a valid chip selector
		if (value.length() < 5)
			return std::nullopt;
		// If we do, check that the start of the string is one of "int" or "ext"
		const auto bus{value.substr(0, 3)};
		if (bus != "int"sv && bus != "ext"sv)
			return std::nullopt;
		// Check that the 4th character is a ':'
		if (value[3] != ':')
			return std::nullopt;
		// Now convert the final part as an unsigned integer
		substrate::toInt_t<uint64_t> index{value.substr(4).data()};
		if (!index.isDec())
			return std::nullopt;
		const auto number{index.fromDec()};
		// And check that it's in the range [0,256)
		if (number > 255U)
			return std::nullopt;
		// Construct and return the chip selection state
		return chip_t{bus[0] == 'i' ? chipBus_t::internal : chipBus_t::external, static_cast<uint8_t>(number)};
	}

	constexpr static auto deviceOption
	{
		option_t
		{
			"--device"sv,
			"The SPIFlashProgrammer to use for the operation"sv,
		}.takesParameter(optionValueType_t::unsignedInt)
	};

	constexpr static auto deviceOptions
	{
		options
		(
			deviceOption,
			option_t
			{
				"--chip"sv,
				"Specifies what Flash chip on which bus you want to target.\n"
				"The chip specification works as follows:\n"
				"'bus' can be one of 'int' or 'ext' representing the internal (on-chip)\n"
				"and external (8-pin Flash connector) SPI busses.\n"
				"N is a number from 0 to 255 which specifies a detected Flash chip as given by the\n"
				"listDevices operation"sv,
			}.takesParameter(optionValueType_t::userDefined, chipSelectionParser)
		)
	};

	constexpr static auto fileOptions
	{
		options
		(
			deviceOptions,
			option_t{optionValue_t{"file"sv}, "The local file to use for the operation"sv}
				.valueType(optionValueType_t::path)
		)
	};

	constexpr static auto listOptions{options(deviceOption)};

	constexpr static auto actions
	{
		optionAlternations
		({
			{
				"listDevices"sv,
				"Lists the available SPIFlashProgrammers attached to your system\n"
				"(This is the default if no operation is given)"sv,
			},
			{
				"list"sv,
				"Lists the available Flash chips a given SPIFlashProgrammer can see"sv,
				listOptions,
			},
			{
				"read"sv,
				"Reads the contents of a specific Flash chip into the requested file"sv,
				fileOptions,
			},
			{
				"write"sv,
				"Writes the contents of the requested file into a specific Flash chip"sv,
				fileOptions,
			},
			{
				"verifiedWrite"sv,
				"Does the same as write, but verifies the contents of the Flash chip after writing"sv,
				fileOptions,
			},
			{
				"erase"sv,
				"Performs a full chip erases on the requested Flash chip"sv,
				deviceOptions,
			},
			{
				"sfdp"sv,
				"Read and display the SFDP (Serial Flash Discoverable Parameters) data for a specific Flash chip"sv,
				deviceOptions,
			},
		})
	};

	constexpr static auto programOptions
	{
		options
		(
			option_t{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv},
			option_t{"--version"sv, "Display the version information for flashprog and exit"sv},
			optionSet_t{"action"sv, actions}
		)
	};
} // namespace flashprog
