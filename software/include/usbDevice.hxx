// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_DEVICE_HXX
#define USB_DEVICE_HXX

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

enum class request_t : uint8_t
{
	typeStandard = 0x00,
	typeClass = 0x20U,
	typeVendor = 0x40U
};

enum class recipient_t : uint8_t
{
	device = 0,
	interface = 1,
	endpoint = 2,
	other = 3
};

struct requestType_t final
{
private:
	uint8_t value{};

public:
	constexpr requestType_t() noexcept = default;
	constexpr requestType_t(const recipient_t recipient, const request_t type) noexcept :
		requestType_t{recipient, type, endpointDir_t::controllerOut} { }
	constexpr requestType_t(const recipient_t recipient, const request_t type, const endpointDir_t direction) noexcept :
		value(static_cast<uint8_t>(recipient) | static_cast<uint8_t>(type) | static_cast<uint8_t>(direction)) { }

	void recipient(const recipient_t recipient) noexcept
	{
		value &= 0xE0U;
		value |= static_cast<uint8_t>(recipient);
	}

	void type(const request_t type) noexcept
	{
		value &= 0x9FU;
		value |= static_cast<uint8_t>(type);
	}

	void dir(const endpointDir_t direction) noexcept
	{
		value &= 0x7FU;
		value |= static_cast<uint8_t>(direction);
	}

	[[nodiscard]] recipient_t recipient() const noexcept
		{ return static_cast<recipient_t>(value & 0x1FU); }
	[[nodiscard]] request_t type() const noexcept
		{ return static_cast<request_t>(value & 0x60U); }
	[[nodiscard]] endpointDir_t dir() const noexcept
		{ return static_cast<endpointDir_t>(value & 0x80U); }
	[[nodiscard]] operator uint8_t() const noexcept { return value; }
};

struct usbDeviceHandle_t final
{
private:
	libusb_device_handle *device{nullptr};

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	[[nodiscard]] bool interruptTransfer(const uint8_t endpoint, void *const bufferPtr, const int32_t bufferLen) const noexcept
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

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	[[nodiscard]] bool bulkTransfer(const uint8_t endpoint, void *const bufferPtr, const int32_t bufferLen) const noexcept
	{
		const auto result{libusb_bulk_transfer(device, endpoint, static_cast<uint8_t *>(bufferPtr),
			bufferLen, nullptr, 0)};
		if (result)
		{
			const auto endpointNumber{uint8_t(endpoint & 0x7FU)};
			const auto direction{endpointDir_t(endpoint & 0x80U)};
			console.error("Failed to complete bulk transfer of "sv, bufferLen,
				" byte(s) to endpoint "sv, endpointNumber, ' ',
				direction == endpointDir_t::controllerIn ? "IN"sv : "OUT"sv,
				", reason:"sv, libusb_error_name(result));
		}
		return !result;
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	[[nodiscard]] bool controlTransfer(const requestType_t requestType, const uint8_t request, const uint16_t value,
		const uint16_t index, void *const bufferPtr, const uint16_t bufferLen) const noexcept
	{
		const auto result{libusb_control_transfer(device, requestType, request, value, index,
			static_cast<uint8_t *>(bufferPtr), bufferLen, 0)};
		if (result < 0)
		{
			console.error("Failed to complete control transfer of "sv, bufferLen,
				" bytes(s), reason:"sv, libusb_error_name(result));
		}
		else if (result != bufferLen)
		{
			console.error("Control transfer incomplete, got "sv, result,
				", expected "sv, bufferLen);
		}
		return result == bufferLen;
	}

public:
	usbDeviceHandle_t() noexcept = default;
	usbDeviceHandle_t(libusb_device_handle *const device_) noexcept : device{device_}
		{ autoDetachKernelDriver(true); }
	[[nodiscard]] bool valid() const noexcept { return device; }

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	void autoDetachKernelDriver(bool autoDetach) const noexcept
	{
		if (const auto result{libusb_set_auto_detach_kernel_driver(device, autoDetach)}; result)
			console.warning("Automatic detach of kernel driver not supported on this platform"sv);
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	[[nodiscard]] bool claimInterface(const int32_t interfaceNumber) const noexcept
	{
		const auto result{libusb_claim_interface(device, interfaceNumber)};
		if (result)
			console.error("Failed to claim interface "sv, interfaceNumber, ": "sv, libusb_error_name(result));
		return !result;
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	[[nodiscard]] bool releaseInterface(const int32_t interfaceNumber) const noexcept
	{
		const auto result{libusb_release_interface(device, interfaceNumber)};
		if (result)
			console.error("Failed to release interface "sv, interfaceNumber, ": "sv, libusb_error_name(result));
		return !result;
	}

	[[nodiscard]] bool writeInterrupt(const uint8_t endpoint, const void *const bufferPtr, const int32_t bufferLen) const noexcept
		{ return interruptTransfer(endpointAddress(endpointDir_t::controllerOut, endpoint),
			const_cast<void *>(bufferPtr), bufferLen); }

	[[nodiscard]] bool readInterrupt(const uint8_t endpoint, void *const bufferPtr, const int32_t bufferLen) const noexcept
		{ return interruptTransfer(endpointAddress(endpointDir_t::controllerIn, endpoint), bufferPtr, bufferLen); }

	[[nodiscard]] bool writeBulk(const uint8_t endpoint, const void *const bufferPtr, const int32_t bufferLen) const noexcept
		{ return bulkTransfer(endpointAddress(endpointDir_t::controllerOut, endpoint),
			const_cast<void *>(bufferPtr), bufferLen); }

	[[nodiscard]] bool readBulk(const uint8_t endpoint, void *const bufferPtr, const int32_t bufferLen) const noexcept
		{ return bulkTransfer(endpointAddress(endpointDir_t::controllerIn, endpoint), bufferPtr, bufferLen); }

	template<typename T> bool writeControl(requestType_t requestType, const uint8_t request,
		const uint16_t value, const uint16_t index, const T &data) const noexcept
	{
		requestType.dir(endpointDir_t::controllerOut);
		static_assert(sizeof(T) <= UINT16_MAX);
		return controlTransfer(requestType, request, value, index, const_cast<T *>(&data), sizeof(T));
	}

	[[nodiscard]] bool writeControl(requestType_t requestType, const uint8_t request,
		const uint16_t value, const uint16_t index, std::nullptr_t) const noexcept
	{
		requestType.dir(endpointDir_t::controllerOut);
		return controlTransfer(requestType, request, value, index, nullptr, 0);
	}

	template<typename T> bool readControl(requestType_t requestType, const uint8_t request,
		const uint16_t value, const uint16_t index, T &data) const noexcept
	{
		requestType.dir(endpointDir_t::controllerIn);
		static_assert(sizeof(T) <= UINT16_MAX);
		return controlTransfer(requestType, request, value, index, &data, sizeof(T));
	}

	template<typename T, size_t length> bool readControl(requestType_t requestType, const uint8_t request,
		const uint16_t value, const uint16_t index, std::array<T, length> &data) const noexcept
	{
		requestType.dir(endpointDir_t::controllerIn);
		static_assert(length * sizeof(T) <= UINT16_MAX);
		return controlTransfer(requestType, request, value, index, data.data(), length * sizeof(T));
	}
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

#endif /*USB_DEVICE_HXX*/
