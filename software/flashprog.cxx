#include <vector>
#include "usbContext.hxx"

int32_t interact(const usbDevice_t &rawDevice)
{
	const auto device{rawDevice.open()};
	if (!device.valid() ||
		!device.claimInterface(0))
		return 1;

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
			devices.push_back(device);
		}
	}
	console.info("Found "sv, devices.size(), " devices");
	if (devices.size() == 1)
		return interact(devices[0]);

	return 0;
}
