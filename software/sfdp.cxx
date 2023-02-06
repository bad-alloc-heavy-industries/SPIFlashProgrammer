// SPDX-License-Identifier: BSD-3-Clause
#include <string_view>
#include <substrate/console>
#include "sfdp.hxx"
#include "sfdpInternal.hxx"
#include "usbProtocol.hxx"

using namespace std::literals::string_view_literals;
using substrate::console;
using substrate::asHex_t;
using namespace flashProto;

namespace sfdp
{
	constexpr static uint32_t sfdpHeaderAddress{0U};
	constexpr static uint32_t tableHeaderAddress{sizeof(sfdpHeader_t)};

	constexpr static std::array<char, 4> sfdpMagic{{'S', 'F', 'D', 'P'}};
	constexpr static uint16_t basicSPIParameterTable{0xFF00};

	[[nodiscard]] static bool sfdpRead(const usbDeviceHandle_t &device, const usbDataSource_t &dataSource,
		const uint32_t address, void *const buffer, const size_t bufferLen)
	{
		return
			requests::sfdp_t{}.write(device, dataSource.interface, bufferLen, address) &&
			device.readBulk(dataSource.endpoint, buffer, bufferLen);
	}

	template<typename T> [[nodiscard]] static bool sfdpRead(const usbDeviceHandle_t &device,
		const usbDataSource_t &dataSource, const uint32_t address, T &buffer)
			{ return sfdpRead(device, dataSource, address, &buffer, sizeof(T)); }

	static void displayHeader(const sfdpHeader_t &header)
	{
		console.info("SFDP Header:"sv);
		console.writeln("-> magic '"sv, header.magic, "'"sv);
		console.writeln("-> version "sv, header.versionMajor, '.', header.versionMinor);
		console.writeln("-> "sv, header.parameterHeadersCount(), " parameter headers"sv);
		console.writeln("-> access protocol "sv, asHex_t<2, '0'>{uint8_t(header.accessProtocol)});
	}

	bool readAndDisplay(const usbDeviceHandle_t &device, const usbDataSource_t dataSource)
	{
		console.info("Reading SFDP data for device"sv);
		sfdpHeader_t header{};
		if (sfdpRead(device, dataSource, sfdpHeaderAddress, header))
			return false;
		if (header.magic != sfdpMagic)
		{
			console.error("Device does not have a valid SFDP block"sv);
			console.error(" -> Read signature '"sv, header.magic, "'");
			console.error(" -> Expected signature '"sv, sfdpMagic, "'");
			return true;
		}
		displayHeader(header);

		return true;
	}
} // namespace sfdp
