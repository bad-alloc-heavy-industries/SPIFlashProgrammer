// SPDX-License-Identifier: BSD-3-Clause
#include <cstring>
#include <cstdint>
#include <array>
#include <substrate/units>
#include <substrate/indexed_iterator>
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
	static std::array<uint8_t, epBufferSize> request{};
	static std::array<uint8_t, epBufferSize> response{};
	static std::array<uint8_t, 4096> flashBuffer{};
	static spiChip_t targetDevice{spiChip_t::none};

	static page_t readPage{};
	static uint16_t readCount{};

	void init(const uint8_t endpoint) noexcept
	{
		epStatusControllerIn[1].isMultiPart(false);
		epStatusControllerIn[1].needsArming(false);
		epStatusControllerIn[1].stall(false);
	}

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
			targetDevice = spiChip_t::none;
		}
		else
			targetDevice = spiChip_t::none;
		return true;
	}

	bool isBusy() noexcept
	{
		spiSelect(targetDevice);
		spiIntWrite(spiOpcodes::statusRead);
		const auto status{spiIntRead()};
		spiSelect(spiChip_t::none);
		return (status & 1);
	}

	void handleErase() noexcept
	{
		requests::erase_t eraseRequest{request};
		bool complete{false};
		responses::erase_t response{};
		response.currentPage = 0xFFFFFFFFU;

		if (targetDevice == spiChip_t::none)
		{
			response.complete = 2;
			sendResponse(response);
			return;
		}

		switch (eraseRequest.operation)
		{
			case eraseOperation_t::all:
			{
				spiSelect(targetDevice);
				spiIntWrite(spiOpcodes::writeEnable);
				spiSelect(spiChip_t::none);

				spiSelect(targetDevice);
				spiIntWrite(spiOpcodes::chipErase);
				spiSelect(spiChip_t::none);
				complete = !isBusy();
				break;
			}
			case eraseOperation_t::page:
				break;
			case eraseOperation_t::pageRange:
				break;
			case eraseOperation_t::status:
				complete = !isBusy();
				break;
		}

		response.complete = complete ? 1 : 0;
		sendResponse(response);
	}

	static void handleRead() noexcept
	{
#if 0
		if (targetDevice == spiChip_t::none)
		{
			// TODO: Handle..
			return;
		}
#endif

		auto &epStatus{epStatusControllerIn[1]};

		spiSelect(targetDevice);
		spiIntWrite(spiOpcodes::pageRead);
		// Translate the page number into a byte address
		spiIntWrite(uint8_t(readPage >> 8U));
		spiIntWrite(uint8_t(readPage));
		spiIntWrite(0);

		// Work through reading the page of Flash response.size() bytes at at time
		for (uint16_t byteCount{}; byteCount < readCount; byteCount += uint8_t(response.size()))
		{
			// For each byte in the response buffer, read a byte from Flash and store it
			for (auto &byte : response)
				byte = spiIntRead();
			// Reset the transfer buffer pointer and amount
			epStatus.memBuffer = response.data();
			epStatus.transferCount = response.size();
			// Wait for the endpoint to have free space in the hardware FIFO
			while (writeEPBusy(1))
				continue;
			// Transfer the data to the USB controller and tell it that we're ready for it to transmit
			writeEP(1);
		}

		spiSelect(spiChip_t::none);
		readCount = 0;
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

	void handleWrite()
	{
		requests::write_t writeRequest{request};

#if 0
		if (targetDevice == spiChip_t::none)
		{
			// TODO: Handle..
			return;
		}
#endif

		// if (writeRequest.count > 256U)

		const uint32_t page{writeRequest.page};
		auto &epStatus{epStatusControllerOut[1]};

		// Enable writes to the device (must be done for every page, so..)
		spiSelect(targetDevice);
		spiIntWrite(spiOpcodes::writeEnable);
		spiSelect(spiChip_t::none);

		spiSelect(targetDevice);
		spiIntWrite(spiOpcodes::pageWrite);
		// Translate the page number into a byte address
		spiIntWrite(uint8_t(page >> 8U));
		spiIntWrite(uint8_t(page));
		spiIntWrite(0);

		for (uint16_t byteCount{}; byteCount < writeRequest.count; byteCount += uint16_t(request.size()))
		{
			const auto transferCount{std::min<uint16_t>(uint16_t(writeRequest.count) - byteCount, request.size())};
			epStatus.memBuffer = request.data();
			epStatus.transferCount = transferCount;
			while (!readEPReady(1))
				continue;
			readEP(1);
			for (const auto &[i, byte] : substrate::indexedIterator_t{request})
			{
				if (i >= transferCount)
					break;
				spiIntWrite(byte);
			}
		}

		spiSelect(spiChip_t::none);
		while (isBusy())
			continue;
		sendResponse(responses::write_t{});
	}

	void handleRequest(const uint8_t endpoint) noexcept
	{
		if (!readEPDataAvail(1))
			return;
		epStatusControllerOut[1].memBuffer = request.data();
		epStatusControllerOut[1].transferCount = usbCtrl.epCtrls[0].rxCount;
		if (!readEP(1))
		{
			usbCtrl.epCtrls[0].rxStatusCtrlL |= vals::usb::epStatusCtrlLStall;
			return;
		}

		// We have now have valid data in the request buffer above. Needs reinterpreting according to the
		// usbProtocol header structures.
		auto type{messages_t(request[0])};
		switch (type)
		{
			case messages_t::erase:
				return handleErase();
			case messages_t::write:
				return handleWrite();
			default:
				return;
		}
	}

	void handleResponse(const uint8_t endpoint) noexcept
	{
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
		}

		return {response_t::stall, nullptr, 0};
	}

	static const handler_t flashProtoInHandler
	{
		nullptr,
		nullptr,
		handleResponse
	};

	static const handler_t flashProtoOutHandler
	{
		init,
		nullptr,
		handleRequest
	};

	void registerHandlers(const uint8_t inEP, const uint8_t outEP,
		const uint8_t interface, const uint8_t config) noexcept
	{
		registerHandler({inEP, endpointDir_t::controllerIn}, config, flashProtoInHandler);
		registerHandler({outEP, endpointDir_t::controllerOut}, config, flashProtoOutHandler);
		usb::device::registerHandler(interface, config, handleCtrlRequest);
	}
} // namespace usb::flashProto
