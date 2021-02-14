// SPDX-License-Identifier: BSD-3-Clause
#include "usb/core.hxx"
#include "usbProtocol.hxx"

using namespace usb::types;
using namespace usb::core;

namespace usb::flashProto
{
	std::array<std::byte, epBufferSize> request{};
	std::array<std::byte, epBufferSize> response{};

	void init() noexcept
	{
		epStatusControllerOut[1].memBuffer = request.data();
		epStatusControllerIn[1].memBuffer = request.data();
	}

	void handleRequest() noexcept
	{
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
