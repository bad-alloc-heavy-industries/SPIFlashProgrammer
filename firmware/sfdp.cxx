// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/index_sequence>
#include "sfdp.hxx"

namespace sfdp
{
	constexpr static uint32_t sfdpHeaderAddress{0U};
	constexpr static uint32_t tableHeaderAddress{sizeof(sfdpHeader_t)};

	constexpr static std::array<char, 4> sfdpMagic{{'S', 'F', 'D', 'P'}};
	constexpr static uint16_t basicSPIParameterTable{0xFF00};

	static void sfdpRead(const spiChip_t targetDevice, const uint32_t address, void *const buffer, const size_t bufferLen)
	{
		auto &device{*spiDevice(targetDevice)};
		spiSelect(targetDevice);
		spiWrite(device, spiOpcodes::readSFDP);
		spiWrite(device, uint8_t(address >> 16U));
		spiWrite(device, uint8_t(address >> 8U));
		spiWrite(device, uint8_t(address));
		spiWrite(device, 0);

		auto *const data{static_cast<uint8_t *>(buffer)};
		for (const auto i : substrate::indexSequence_t{bufferLen})
			data[i] = spiRead(device);
		spiSelect(spiChip_t::none);
	}

	template<typename T> static void sfdpRead(const spiChip_t targetDevice, const uint32_t address, T &buffer)
		{ return sfdpRead(targetDevice, address, &buffer, sizeof(T)); }

	spiParameters_t readBasicParameterTable(const spiChip_t device, uint32_t address, size_t length)
	{
		basicParameterTable_t parameterTable{};
		sfdpRead(device, address, &parameterTable, std::min(sizeof(basicParameterTable_t), length));

		spiParameters_t result{};
		result.capacity = parameterTable.flashMemoryDensity.capacity();
		for (const auto &eraseType : parameterTable.eraseTypes)
		{
			if (eraseType.opcode == parameterTable.sectorEraseOpcode)
			{
				result.sectorEraseOpcode = eraseType.opcode;
				result.sectorSize = eraseType.eraseSize();
				break;
			}
		}
		result.pageSize = parameterTable.programmingAndChipEraseTiming.pageSize();
		return result;
	}

	std::optional<spiParameters_t> parameters(const spiChip_t device)
	{
		sfdpHeader_t header{};
		sfdpRead(device, sfdpHeaderAddress, header);
		if (header.magic != sfdpMagic)
			return std::nullopt;

		for (const auto i : substrate::indexSequence_t{header.parameterHeadersCount()})
		{
			parameterTableHeader_t tableHeader{};
			sfdpRead(device, tableHeaderAddress + (sizeof(parameterTableHeader_t) * i), tableHeader);
			if (tableHeader.jedecParameterID() == basicSPIParameterTable)
				return readBasicParameterTable(device, tableHeader.tableAddress, tableHeader.tableLength());
		}

		return std::nullopt;
	}
} // namespace sfdp
