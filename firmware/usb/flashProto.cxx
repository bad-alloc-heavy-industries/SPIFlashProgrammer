// SPDX-License-Identifier: BSD-3-Clause
#include <cstring>
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "usb/core.hxx"
#include "usbProtocol.hxx"

using namespace usb::types;
using namespace usb::core;
using namespace flashProto;

namespace usb::flashProto
{
	std::array<std::byte, epBufferSize> request{};
	std::array<std::byte, epBufferSize> response{};

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
		memcpy(response.data(), &data, sizeof(T));
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
		requests::listDevice_t listRequest{};
		memcpy(&listRequest, request.data(), sizeof(listRequest));
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
		requests::targetDevice_t targetRequest{};
		memcpy(&targetRequest, request.data(), sizeof(targetRequest));
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

		sendResponse(messages_t::targetDevice);
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
