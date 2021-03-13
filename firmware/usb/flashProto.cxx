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

	void handleDeviceCount() noexcept
	{
		responses::deviceCount_t deviceCount{};
		deviceCount.internalCount = 2;
		deviceCount.externalCount = 0;

		auto &epStatus{epStatusControllerIn[1]};
		epStatus.transferCount = sizeof(deviceCount);
		memcpy(response.data(), &deviceCount, epStatus.transferCount);
		writeEP(1);
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
