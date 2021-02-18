// SPDX-License-Identifier: BSD-3-Clause
#include <vector>
#include "usbContext.hxx"
#include "usbProtocol.hxx"

using namespace flashProto;

uint8_t requestCount(const usbDeviceHandle_t &device)
{
	const auto type{messages_t::deviceCount};
	device.writeInterrupt(1, &type, 1);

	deviceCount_t deviceCount{};
	device.readInterrupt(1, &deviceCount, sizeof(deviceCount));
	return deviceCount.count;
}

int32_t interact(const usbDevice_t &rawDevice)
{
	const auto device{rawDevice.open()};
	if (!device.valid() ||
		!device.claimInterface(0))
		return 1;

	const auto deviceCount{requestCount(device)};
	// Talk with device here.

	if (!device.releaseInterface(0))
		return 1;
	return 0;
}

int32_t main(int, char **)
{
	console = {stdout, stderr};
	usbContext_t context{};

	if (!context.valid())
		return 2;

	std::vector<usbDevice_t> devices{};

	for (auto device : context.deviceList())
	{
		if (device.vid() == 0x1209 && device.pid() == 0xAB0C)
		{
			console.info("Found device at address "sv, device.busNumber(), ':', device.portNumber());
			devices.emplace_back(std::move(device));
		}
	}
	console.info("Found "sv, devices.size(), " devices");
	if (devices.size() == 1)
		return interact(devices[0]);

	return 0;
}