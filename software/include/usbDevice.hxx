// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_DEVICE__HXX
#define USB_DEVICE__HXX

#include <string_view>
#include <utility>
#include <libusb.h>
#include <substrate/console>

using namespace std::literals::string_view_literals;
using substrate::console;

enum class endpointDir_t : uint8_t
{
	controllerOut = 0x00U,
	controllerIn = 0x80U
};

constexpr static const uint8_t endpointDirMask{0x7F};
constexpr inline uint8_t endpointAddress(const endpointDir_t dir, const uint8_t number) noexcept
	{ return uint8_t(dir) | (number & endpointDirMask); }

struct usbDeviceHandle_t final
{
private:
	libusb_device_handle *device{nullptr};

	bool interruptTransfer(const uint8_t endpoint, void *const bufferPtr, const int32_t bufferLen) const noexcept
	{
		const auto result{libusb_interrupt_transfer(device, endpoint, static_cast<uint8_t *>(bufferPtr),
			bufferLen, nullptr, 0)};
		if (result)
		{
			const auto endpointNumber{uint8_t(endpoint & 0x7FU)};
			const auto direction{endpointDir_t(endpoint & 0x80U)};
			console.error("Failed to complete interrupt transfer of "sv, bufferLen,
				" byte(s) to endpoint "sv, endpointNumber, ' ',
				direction == endpointDir_t::controllerIn ? "IN"sv : "OUT"sv,
				", reason:"sv, libusb_error_name(result));
		}
		return !result;
	}

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

	bool claimInterface(const int32_t interfaceNumber) const noexcept
	{
		const auto result{libusb_claim_interface(device, interfaceNumber)};
		if (result)
			console.error("Failed to claim interface "sv, interfaceNumber, ": "sv, libusb_error_name(result));
		return !result;
	}

	bool releaseInterface(const int32_t interfaceNumber) const noexcept
	{
		const auto result{libusb_release_interface(device, interfaceNumber)};
		if (result)
			console.error("Failed to release interface "sv, interfaceNumber, ": "sv, libusb_error_name(result));
		return !result;
	}

	bool writeInterrupt(const uint8_t endpoint, const void *const bufferPtr, const int32_t bufferLen) const noexcept
		{ return interruptTransfer(endpointAddress(endpointDir_t::controllerOut, endpoint),
			const_cast<void *>(bufferPtr), bufferLen); }

	bool readInterrupt(const uint8_t endpoint, void *const bufferPtr, const int32_t bufferLen) const noexcept
		{ return interruptTransfer(endpointAddress(endpointDir_t::controllerIn, endpoint), bufferPtr, bufferLen); }
};

struct usbDevice_t final
{
private:
	libusb_device *device{nullptr};
	libusb_device_descriptor descriptor{};

	usbDevice_t() noexcept = default;

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

	usbDevice_t(usbDevice_t &&other) noexcept : usbDevice_t{} { swap(other); }

	~usbDevice_t() noexcept
	{
		if (device)
			libusb_unref_device(device);
	}

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

	void swap(usbDevice_t &other) noexcept
	{
		std::swap(device, other.device);
		std::swap(descriptor, other.descriptor);
	}
};

#endif /*USB_DEVICE__HXX*/
