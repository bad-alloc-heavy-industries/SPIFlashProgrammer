#ifndef USB_DEVICE__HXX
#define USB_DEVICE__HXX

#include <string_view>
#include <libusb.h>
#include <substrate/console>

using namespace std::literals::string_view_literals;
using substrate::console;

struct usbDeviceHandle_t final
{
private:
	libusb_device_handle *device{nullptr};

public:
	usbDeviceHandle_t() noexcept = default;
	usbDeviceHandle_t(libusb_device_handle *const device_) noexcept : device{device_}
		{ autoDetachKernelDriver(true); }
	bool valid() const noexcept { return device; }

	void autoDetachKernelDriver(bool autoDetach) const noexcept
	{
		if (const auto result{libusb_set_auto_detach_kernel_driver(device, autoDetach)}; result)
			console.warning("Automatic detach of kernel driver not supported on this platform"sv);
	}

	bool claimInterface(int32_t interfaceNumber) const noexcept
	{
		const auto result{libusb_claim_interface(device, interfaceNumber)};
		if (result)
			console.error("Failed to claim interface "sv, interfaceNumber, ": "sv, libusb_error_name(result));
		return !result;
	}

	bool releaseInterface(int32_t interfaceNumber) const noexcept
	{
		const auto result{libusb_release_interface(device, interfaceNumber)};
		if (result)
			console.error("Failed to release interface "sv, interfaceNumber, ": "sv, libusb_error_name(result));
		return !result;
	}

	bool writeInterrupt(uint8_t endpoint, void *bufferPtr, int32_t bufferLen) const noexcept
	{
		const auto result{libusb_interrupt_transfer(device, endpoint, static_cast<uint8_t *>(bufferPtr),
			bufferLen, nullptr, 0)};
		if (result)
			console.error("Failed to complete interrupt transfer of "sv, bufferLen,
				" byte(s) to endpoint "sv, endpoint, ", reason:"sv, libusb_error_name(result));
		return !result;
	}
};

struct usbDevice_t final
{
private:
	libusb_device *device;
	libusb_device_descriptor descriptor{};

public:
	usbDevice_t(libusb_device *const device_) noexcept : device{device_}
	{
		libusb_ref_device(device);
		if (const auto result{libusb_get_device_descriptor(device, &descriptor)}; result)
		{
			console.warning("Failed to get descriptor for device: ", libusb_error_name(result));
			descriptor = {};
		}
	}

	~usbDevice_t() noexcept { libusb_unref_device(device); }

	auto vid() const noexcept { return descriptor.idVendor; }
	auto pid() const noexcept { return descriptor.idProduct; }

	auto busNumber() const noexcept { return libusb_get_bus_number(device); }
	auto portNumber() const noexcept { return libusb_get_port_number(device); }

	usbDeviceHandle_t open() const noexcept
	{
		libusb_device_handle *handle{nullptr};
		if (const auto result{libusb_open(device, &handle)}; result)
		{
			console.error("Failed to open requested device: "sv, libusb_error_name(result));
			return {};
		}
		return {handle};
	}
};

#endif /*USB_DEVICE__HXX*/
