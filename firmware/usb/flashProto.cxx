// SPDX-License-Identifier: BSD-3-Clause
#include <cstring>
#include <cstdint>
#include <array>
#include <substrate/units>
#include <substrate/indexed_iterator>
#include <substrate/index_sequence>
#include <tm4c123gh6pm/platform.hxx>
#include <tm4c123gh6pm/constants.hxx>
#include <usb/core.hxx>
#include <usb/device.hxx>
#include "usbProtocol.hxx"
#include "flashProto.hxx"
#include "spi.hxx"

using namespace substrate;
using namespace usb::constants;
using namespace usb::types;
using namespace usb::core;
using namespace usb::device;
using namespace flashProto;
using usb::device::packet;

namespace usb::flashProto
{
	static std::array<uint8_t, epBufferSize> response{};
	static std::array<uint8_t, 4096> flashBuffer{};
	static spiChip_t targetDevice{spiChip_t::none};

	static uint8_t readEndpoint{};
	static page_t readPage{};
	static uint16_t readCount{};

	static requests::erase_t eraseConfig{};
	static eraseOperation_t eraseOperation{eraseOperation_t::idle};
	static bool eraseActive{false};

	static uint8_t writeEndpoint{};
	static page_t writePage{};
	static uint32_t writeCount{};
	static uint32_t writeTotal{};

	static bool verifyWrite{};
	static page_t verifyPage{};

	static responses::status_t status{};

	template<typename T> void sendResponse(const T &data)
	{
		auto &epStatus{epStatusControllerIn[1]};
		epStatus.memBuffer = response.data();
		epStatus.transferCount = sizeof(T);
		memcpy(response.data(), &data.type, sizeof(T));
		while (writeEPBusy(1))
			continue;
		writeEP(1);
	}

	template<typename T> uint16_t writeResponse(const T &data)
	{
		std::memcpy(response.data(), &data, sizeof(T));
		return sizeof(T);
	}

	static uint16_t fetchDeviceCount() noexcept
	{
		responses::deviceCount_t deviceCount{};
		deviceCount.internalCount = 2;
		const auto [mfr, type, capacity] = identDevice(spiChip_t::target);
		if (mfr || type || capacity)
			deviceCount.externalCount = 1;
		else
			deviceCount.externalCount = 0;
		return writeResponse(deviceCount);
	}

	[[nodiscard]] constexpr size_t power2(const size_t power) noexcept
		{ return power ? power2(power - 1) * 2 : 1; }

	static uint16_t fetchDeviceListing(const setupPacket::address_t address)
	{
		if (address.addrH >= static_cast<uint8_t>(deviceType_t::none))
			return 0;
		const auto deviceType{static_cast<deviceType_t>(address.addrH)};
		const auto deviceNumber{address.addrL};
		responses::listDevice_t device{};

		if (deviceType == deviceType_t::internal)
		{
			if (deviceNumber <= spi::internalChips)
			{
				device.manufacturer = spi::localMFR[deviceNumber];
				device.deviceType = spi::localType[deviceNumber];
				device.deviceSize = power2(spi::localCapacity[deviceNumber]);
				device.eraseSize = 64_KiB;
				device.pageSize = 256;
			}
		}
		else
		{
			if (deviceNumber == 0)
			{
				const auto [mfr, type, capacity] = identDevice(spiChip_t::target);
				device.manufacturer = mfr;
				device.deviceType = type;
				device.deviceSize = power2(capacity);
				// These are wrong but need a device LUT to fix.
				device.eraseSize = 64_KiB;
				device.pageSize = 256;
			}
		}
		return writeResponse(device);
	}

	static bool handleTargetDevice(const setupPacket::address_t address) noexcept
	{
		if (address.addrH > static_cast<uint8_t>(deviceType_t::none))
			return false;
		const auto deviceType{static_cast<deviceType_t>(address.addrH)};
		const auto deviceNumber{address.addrL};

		if (deviceType == deviceType_t::internal)
		{
			if (deviceNumber == 0)
				targetDevice = spiChip_t::local1;
			else if (deviceNumber == 1)
				targetDevice = spiChip_t::local2;
			else
				return false;
		}
		else if (deviceType == deviceType_t::external)
		{
			if (deviceNumber == 0)
				targetDevice = spiChip_t::target;
			else
				return false;
		}
		else
			targetDevice = spiChip_t::none;
		return true;
	}

	static bool isBusy() noexcept
	{
		auto &device{*spiDevice(targetDevice)};
		spiSelect(targetDevice);
		spiWrite(device, spiOpcodes::statusRead);
		const auto status{spiRead(device)};
		spiSelect(spiChip_t::none);
		return (status & 1);
	}

	static void handleErase() noexcept
	{
		status.eraseComplete = 0;
		switch (eraseOperation)
		{
			case eraseOperation_t::all:
			{
				spiSelect(targetDevice);
				spiWrite(spiOpcodes::writeEnable);
				spiSelect(spiChip_t::none);

				spiSelect(targetDevice);
				spiWrite(spiOpcodes::chipErase);
				spiSelect(spiChip_t::none);
				break;
			}
			case eraseOperation_t::page:
				eraseConfig.endPage = eraseConfig.beginPage + 1;
				[[fallthrough]];
			case eraseOperation_t::pageRange:
				eraseActive = true;
				break;
			default:
				status.eraseComplete = 3;
		}
	}

	static bool setupErase(const uint8_t opcode)
	{
		if (opcode >= static_cast<uint8_t>(eraseOperation_t::idle))
			return false;
		else if (targetDevice == spiChip_t::none)
		{
			eraseOperation = eraseOperation_t::idle;
			status.eraseComplete = 2;
			return false;
		}
		auto &epStatus{epStatusControllerOut[0]};
		eraseOperation = static_cast<eraseOperation_t>(opcode);
		epStatus.memBuffer = &eraseConfig;
		epStatus.transferCount = sizeof(eraseConfig);
		epStatus.needsArming(true);
		setupCallback = handleErase;
		return true;
	}

	static void checkEraseStatus()
	{
		status.eraseComplete = eraseActive || isBusy() ? 0 : 1;
		if (status.eraseComplete)
			eraseOperation = eraseOperation_t::idle;
	}

	static void performRead(const uint8_t endpoint)
	{
		auto &device{*spiDevice()};
		if (readCount == 0)
			return;
		auto &epStatus{epStatusControllerIn[endpoint]};
		// For each byte in the response buffer, read a byte from Flash and store it
		for (auto &byte : response)
			byte = spiRead(device);
		// Reset the transfer buffer pointer and amount
		epStatus.memBuffer = response.data();
		epStatus.transferCount = response.size();
		// Transfer the data to the USB controller and tell it that we're ready for it to transmit
		writeEP(endpoint);
		readCount -= static_cast<uint16_t>(response.size());
		if (readCount == 0)
			spiSelect(spiChip_t::none);
	}

	static void beginPageRead(const page_t page) noexcept
	{
		auto &device{*spiDevice(targetDevice)};
		spiSelect(targetDevice);
		spiWrite(device, spiOpcodes::pageRead);
		// Translate the page number into a byte address
		spiWrite(device, uint8_t(page >> 8U));
		spiWrite(device, uint8_t(page));
		spiWrite(device, 0);
	}

	static void handleRead() noexcept
	{
		if (targetDevice == spiChip_t::none)
		{
			// TODO: Handle.. - use the status area to indicate we were asked to do something silly
			return;
		}

		beginPageRead(readPage);
		performRead(readEndpoint);
	}

	static bool setupRead(const uint16_t count) noexcept
	{
		if (count > flashBuffer.size())
			return false;
		else if (!count)
			readCount = 256U;
		else
			readCount = count;
		auto &epStatus{epStatusControllerOut[0]};
		epStatus.memBuffer = &readPage;
		epStatus.transferCount = sizeof(readPage);
		epStatus.needsArming(true);
		setupCallback = handleRead;
		return true;
	}

	static void writeAddress()
	{
		auto &device{*spiDevice(targetDevice)};
		// Enable writes to the device (must be done for every page, so..)
		spiSelect(targetDevice);
		spiWrite(device, spiOpcodes::writeEnable);
		spiSelect(spiChip_t::none);

		spiSelect(targetDevice);
		spiWrite(device, spiOpcodes::pageWrite);
		// Translate the page number into a byte address
		spiWrite(device, uint8_t(writePage >> 8U));
		spiWrite(device, uint8_t(writePage));
		spiWrite(device, 0);

		++writePage;
	}

	static void performWrite(const uint8_t endpoint)
	{
		auto &device{*spiDevice()};
		if (writeCount == 0)
			return;
		readEP(endpoint);
		auto &epStatus{epStatusControllerOut[endpoint]};
		// Compute where we are in the buffer
		const auto begin{writeTotal - writeCount};
		const auto end{writeTotal - epStatus.transferCount};
		// For each new byte in the write buffer, write the byte to Flash
		for (const auto i : substrate::indexSequence_t{begin, end})
			spiWrite(device, flashBuffer[i]);
		// Decrease the number of bytes left to write by the amount written
		writeCount -= end - begin;
		// If we finished writing a page or we finished recieving data
		if (!(end & 0xFFU) || !writeCount)
		{
			spiSelect(spiChip_t::none);
			while (isBusy())
				continue;
			if (writeCount)
				writeAddress();
		}
		// If we completed writing the buffer and we need to verify, perform verification
		if (writeCount == 0 && verifyWrite)
		{
			beginPageRead(verifyPage);
			for (const auto i : substrate::indexSequence_t{writeTotal})
			{
				if (flashBuffer[i] != spiRead(device))
					status.writeOK = false;
			}
			spiSelect(spiChip_t::none);
		}
	}

	static void handleWrite()
	{
#if 0
		if (targetDevice == spiChip_t::none)
		{
			// TODO: Handle..
			return;
		}
#endif

		writeAddress();

		auto &epStatus{epStatusControllerOut[writeEndpoint]};
		// Reset the transfer buffer pointer and amount
		epStatus.memBuffer = flashBuffer.data();
		epStatus.transferCount = static_cast<uint16_t>(writeCount);
	}

	static bool setupWrite(const uint16_t count, const bool verify) noexcept
	{
		if (count > flashBuffer.size())
			return false;
		else if (!count)
			writeCount = 256U;
		else
			writeCount = count;
		auto &epStatus{epStatusControllerOut[0]};
		epStatus.memBuffer = &writePage;
		epStatus.transferCount = sizeof(writePage);
		epStatus.needsArming(true);
		setupCallback = handleWrite;

		writeTotal = writeCount;
		verifyWrite = verify;
		verifyPage = writePage;
		status.writeOK = true;
		return true;
	}

	static void tick() noexcept
	{
		if (eraseActive && !isBusy())
		{
			auto *device{spiDevice(targetDevice)};
			status.erasePage = eraseConfig.beginPage;
			if (eraseConfig.beginPage == eraseConfig.endPage || !device)
			{
				eraseActive = false;
				return;
			}
			const auto page{static_cast<uint32_t>(eraseConfig.beginPage) << 8U};
			++eraseConfig.beginPage;
			spiSelect(targetDevice);
			spiWrite(*device, spiOpcodes::writeEnable);
			spiSelect(spiChip_t::none);

			spiSelect(targetDevice);
			spiWrite(*device, spiOpcodes::blockErase);
			// Translate the page number into a byte address
			spiWrite(*device, uint8_t(page >> 8U));
			spiWrite(*device, uint8_t(page));
			spiWrite(*device, 0);
			spiSelect(spiChip_t::none);
		}
	}

	static answer_t handleCtrlRequest(const std::size_t interface) noexcept
	{
		const auto &requestType{packet.requestType};
		if (requestType.recipient() != setupPacket::recipient_t::interface ||
			requestType.type() != setupPacket::request_t::typeClass ||
			packet.index != interface)
			return {response_t::unhandled, nullptr, 0};

		const auto request{static_cast<messages_t>(packet.request)};
		switch (request)
		{
			case messages_t::deviceCount:
				if (packet.requestType.dir() != endpointDir_t::controllerIn)
					return {response_t::stall, nullptr, 0};
				return {response_t::data, response.data(), fetchDeviceCount()};
			case messages_t::listDevice:
				if (packet.requestType.dir() != endpointDir_t::controllerIn)
					return {response_t::stall, nullptr, 0};
				return {response_t::data, response.data(), fetchDeviceListing(packet.value.asAddress())};
			case messages_t::targetDevice:
				if (packet.requestType.dir() != endpointDir_t::controllerOut)
					return {response_t::stall, nullptr, 0};
				if (handleTargetDevice(packet.value.asAddress()))
					return {response_t::zeroLength, nullptr, 0};
				else
					return {response_t::stall, nullptr, 0};
			case messages_t::read:
				if (packet.requestType.dir() != endpointDir_t::controllerOut)
					return {response_t::stall, nullptr, 0};
				if (setupRead(packet.value))
					return {response_t::zeroLength, nullptr, 0};
				else
					return {response_t::stall, nullptr, 0};
			case messages_t::erase:
				if (packet.requestType.dir() != endpointDir_t::controllerOut)
					return {response_t::stall, nullptr, 0};
				if (setupErase(packet.value.asConfiguration()))
					return {response_t::zeroLength, nullptr, 0};
				else
					return {response_t::stall, nullptr, 0};
			case messages_t::write:
			case messages_t::verifiedWrite:
				if (packet.requestType.dir() != endpointDir_t::controllerOut)
					return {response_t::stall, nullptr, 0};
				if (setupWrite(packet.value, request == messages_t::verifiedWrite))
					return {response_t::zeroLength, nullptr, 0};
				else
					return {response_t::stall, nullptr, 0};
			case messages_t::status:
				if (packet.requestType.dir() != endpointDir_t::controllerIn)
					return {response_t::stall, nullptr, 0};
				if (eraseOperation != eraseOperation_t::idle)
					checkEraseStatus();
				return {response_t::data, &status, sizeof(status)};
		}

		return {response_t::stall, nullptr, 0};
	}

	static const handler_t flashProtoInHandler
	{
		nullptr,
		nullptr,
		performRead
	};

	static const handler_t flashProtoOutHandler
	{
		nullptr,
		nullptr,
		performWrite
	};

	void registerHandlers(const uint8_t inEP, const uint8_t outEP,
		const uint8_t interface, const uint8_t config) noexcept
	{
		readEndpoint = inEP;
		writeEndpoint = outEP;
		registerHandler({inEP, endpointDir_t::controllerIn}, config, flashProtoInHandler);
		registerHandler({outEP, endpointDir_t::controllerOut}, config, flashProtoOutHandler);
		registerSOFHandler(interface, tick);
		usb::device::registerHandler(interface, config, handleCtrlRequest);
	}
} // namespace usb::flashProto
