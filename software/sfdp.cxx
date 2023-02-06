// SPDX-License-Identifier: BSD-3-Clause
#include <string_view>
#include <substrate/console>
#include <substrate/index_sequence>
#include "sfdp.hxx"
#include "sfdpInternal.hxx"
#include "usbProtocol.hxx"

using namespace std::literals::string_view_literals;
using substrate::console;
using substrate::asHex_t;
using substrate::indexSequence_t;
using namespace flashProto;

namespace sfdp
{
	constexpr static uint32_t sfdpHeaderAddress{0U};
	constexpr static uint32_t tableHeaderAddress{sizeof(sfdpHeader_t)};

	constexpr static std::array<char, 4> sfdpMagic{{'S', 'F', 'D', 'P'}};
	constexpr static uint16_t basicSPIParameterTable{0xFF00U};

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
		console.info("-> magic '"sv, header.magic, "'"sv);
		console.info("-> version "sv, header.versionMajor, '.', header.versionMinor);
		console.info("-> "sv, header.parameterHeadersCount(), " parameter headers"sv);
		console.info("-> access protocol "sv, asHex_t<2, '0'>{uint8_t(header.accessProtocol)});
	}

	static void displayTableHeader(const parameterTableHeader_t &header, const size_t index)
	{
		console.info("Parameter table header "sv, index, ":"sv);
		console.info("-> type "sv, asHex_t<4, '0'>{header.jedecParameterID()});
		console.info("-> version "sv, header.versionMajor, '.', header.versionMinor);
		console.info("-> table is "sv, header.tableLength(), " bytes long"sv);
		console.info("-> table SFDP address: "sv, uint32_t{header.tableAddress});
	}

	bool readAndDisplay(const usbDeviceHandle_t &device, const usbDataSource_t dataSource)
	{
		console.info("Reading SFDP data for device"sv);
		sfdpHeader_t header{};
		if (!sfdpRead(device, dataSource, sfdpHeaderAddress, header))
			return false;
		if (header.magic != sfdpMagic)
		{
			console.error("Device does not have a valid SFDP block"sv);
			console.error(" -> Read signature '"sv, header.magic, "'");
			console.error(" -> Expected signature '"sv, sfdpMagic, "'");
			return true;
		}
		displayHeader(header);

		for (const auto idx : indexSequence_t{header.parameterHeadersCount()})
		{
			parameterTableHeader_t tableHeader{};
			if (!sfdpRead(device, dataSource, tableHeaderAddress + (sizeof(parameterTableHeader_t) * idx), tableHeader))
				return false;
			displayTableHeader(tableHeader, idx + 1U);
		}
		return true;
	}
} // namespace sfdp
