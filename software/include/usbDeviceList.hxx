// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_DEVICE_LIST__HXX
#define USB_DEVICE_LIST__HXX

#include <string_view>
#include <libusb.h>
#include <substrate/console>
#include "usbDevice.hxx"

using namespace std::literals::string_view_literals;
using substrate::console;

struct usbDeviceIter_t final
{
private:
	libusb_device **current;

public:
	usbDeviceIter_t(libusb_device **current_) noexcept : current{current_} { }
	usbDevice_t operator *() const noexcept { return {*current}; }

	auto operator ++() noexcept
	{
		++current;
		return *this;
	}

	bool operator ==(const usbDeviceIter_t &other) const noexcept
		{ return current == other.current; }
	bool operator !=(const usbDeviceIter_t &other) const noexcept
		{ return current != other.current; }
};

struct usbDeviceList_t final
{
private:
	libusb_device **deviceList{nullptr};
	size_t count{0};

public:
	usbDeviceList_t() noexcept = default;
	usbDeviceList_t(libusb_device **const deviceList_, ssize_t count_) noexcept :
		deviceList{deviceList_}, count{static_cast<size_t>(count_)} { }

	~usbDeviceList_t() noexcept
	{
		if (count)
			libusb_free_device_list(deviceList, false);
	}

	usbDeviceIter_t begin() const noexcept { return {deviceList}; }
	usbDeviceIter_t end() const noexcept { return {deviceList + count}; }
};

#endif /*USB_DEVICE_LIST__HXX*/
