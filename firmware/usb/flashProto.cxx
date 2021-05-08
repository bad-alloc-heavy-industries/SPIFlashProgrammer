// SPDX-License-Identifier: BSD-3-Clause
#include <cstring>
#include <cstdint>
#include <substrate/units>
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "usb/core.hxx"
#include "usbProtocol.hxx"
#include "spi.hxx"

using namespace substrate;
using namespace usb::types;
using namespace usb::core;
using namespace flashProto;

namespace usb::flashProto
{
	std::array<std::byte, epBufferSize> request{};
	std::array<std::byte, epBufferSize> response{};
	spiChip_t targetDevice{spiChip_t::none};

	void init() noexcept
	{
		epStatusControllerIn[1].isMultiPart(false);
		epStatusControllerIn[1].needsArming(false);
		epStatusControllerIn[1].stall(false);
	}

	template<typename T> void sendResponse(const T &data)
	{
		auto &epStatus{epStatusControllerIn[1]};
		epStatus.transferCount = sizeof(T);
		memcpy(response.data(), &data.type, sizeof(T));
		while (writeEPBusy(1))
			continue;
		writeEP(1);
	}

	void handleDeviceCount() noexcept
	{
		responses::deviceCount_t deviceCount{};
		deviceCount.internalCount = 2;
		deviceCount.externalCount = 0;

		sendResponse(deviceCount);
	}

	[[nodiscard]] constexpr size_t power2(const size_t power) noexcept
		{ return power ? power2(power - 1) * 2 : 1; }

	void handleListDevice() noexcept
	{
		requests::listDevice_t listRequest{request};
		responses::listDevice_t device{};
		const auto deviceNumber{listRequest.deviceNumber};

		if (listRequest.deviceType == deviceType_t::internal)
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

		sendResponse(device);
	}

	void handleTargetDevice() noexcept
	{
		requests::targetDevice_t targetRequest{request};
		const auto deviceNumber{targetRequest.deviceNumber};

		if (targetRequest.deviceType == deviceType_t::internal)
		{
			if (deviceNumber == 0)
				targetDevice = spiChip_t::local1;
			else if (deviceNumber == 1)
				targetDevice = spiChip_t::local2;
			else
				targetDevice = spiChip_t::none;
		}
		else
		{
			targetDevice = spiChip_t::none;
		}

		sendResponse(responses::targetDevice_t{});
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

	void handleRead() noexcept
	{
		requests::read_t readRequest{request};

#if 0
		if (targetDevice == spiChip_t::none)
		{
			// TODO: Handle..
			return;
		}
#endif

		sendResponse(responses::read_t{});
		const uint32_t page{readRequest.page};
		auto &epStatus{epStatusControllerIn[1]};

		spiSelect(targetDevice);
		spiIntWrite(spiOpcodes::pageRead);
		// Translate the page number into a byte address
		spiIntWrite(uint8_t(page >> 8U));
		spiIntWrite(uint8_t(page));
		spiIntWrite(0);

		for (uint16_t byteCount{}; byteCount < 256U; byteCount += uint16_t(response.size()))
		{
			for (auto &byte : response)
				byte = std::byte{spiIntRead()};
			epStatus.transferCount = response.size();
			while (writeEPBusy(1))
				continue;
			writeEP(1);
		}

		spiSelect(spiChip_t::none);
	}

	void handleRequest() noexcept
	{
		epStatusControllerOut[1].memBuffer = request.data();
		epStatusControllerOut[1].transferCount = usbCtrl.epCtrls[0].rxCount;
		if (!readEP(1))
		{
			usbCtrl.epCtrls[0].rxStatusCtrlL |= vals::usb::epStatusCtrlLStall;
			return;
		}

		epStatusControllerIn[1].memBuffer = response.data();
		// We have now have valid data in the request buffer above. Needs reinterpreting according to the
		// usbProtocol header structures.
		auto type{messages_t(request[0])};
		switch (type)
		{
			case messages_t::deviceCount:
				return handleDeviceCount();
			case messages_t::listDevice:
				return handleListDevice();
			case messages_t::targetDevice:
				return handleTargetDevice();
			case messages_t::erase:
				return handleErase();
			case messages_t::read:
				return handleRead();
		}
	}

	void handleResponse() noexcept
	{
	}

	void handlePacket() noexcept
	{
		if (usbPacket.dir() == endpointDir_t::controllerOut)
			handleRequest();
		else
			handleResponse();
	}
} // namespace usb::flashProto
