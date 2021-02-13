#ifndef USB_CONTEXT__HXX
#define USB_CONTEXT__HXX

#include <string_view>
#include <libusb.h>
#include <substrate/console>

#include "usbDeviceList.hxx"

using namespace std::literals::string_view_literals;
using substrate::console;

struct usbContext_t final
{
private:
	libusb_context *context{nullptr};

public:
	usbContext_t() noexcept
	{
		if (auto result = libusb_init(&context); result)
		{
			console.error("Failed to initialise libusb-1.0: "sv, libusb_error_name(result));
			context = nullptr;
		}
	}

	~usbContext_t() noexcept
	{
		if (context)
			libusb_exit(context);
	}

	bool valid() const noexcept { return context; }

	usbDeviceList_t deviceList() const noexcept
	{
		libusb_device **list{nullptr};
		auto result = libusb_get_device_list(context, &list);
		if (result < 0)
		{
			console.error("Failed to get device list: "sv, libusb_error_name(result));
			return {};
		}
		return {list, result};
	}
};

#endif /*USB_CONSTEXT__HXX*/
