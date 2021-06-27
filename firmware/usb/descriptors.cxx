// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include <usb/core.hxx>
#include <usb/descriptors.hxx>
#include <usb/device.hxx>
#include "constants.hxx"

using namespace usb::constants;
using namespace usb::types;
using namespace usb::descriptors;
using namespace std::literals::string_view_literals;

static const usbDeviceDescriptor_t usbDeviceDesc
{
	sizeof(usbDeviceDescriptor_t),
	usbDescriptor_t::device,
	0x0200, // This is 2.00 in USB's BCD format
	usbClass_t::none,
	uint8_t(subclasses::device_t::none),
	uint8_t(protocols::device_t::none),
	epBufferSize,
	vid,
	pid,
	0x0001, // BCD encoded device version
	1, // Manufacturer string index
	2, // Product string index
	0, // Temporarily do not support a serial number string
	configsCount
};

static const std::array<usbConfigDescriptor_t, configsCount> usbConfigDesc
{{
	{
		sizeof(usbConfigDescriptor_t),
		usbDescriptor_t::configuration,
		sizeof(usbConfigDescriptor_t) + sizeof(usbInterfaceDescriptor_t) +
			sizeof(usbEndpointDescriptor_t) + sizeof(usbEndpointDescriptor_t) +
			sizeof(usbInterfaceDescriptor_t) + sizeof(dfu::functionalDescriptor_t),
		interfaceCount,
		1, // This config
		0, // No string to describe this configuration (for now)
		usbConfigAttr_t::defaults,
		50 // 100mA (the max our regulator can do)
	}
}};

static const std::array<usbInterfaceDescriptor_t, interfaceDescriptorCount> usbInterfaceDesc
{{
	{
		sizeof(usbInterfaceDescriptor_t),
		usbDescriptor_t::interface,
		0, // interface index 0
		0, // alternate 0
		2, // two endpoints to the interface
		usbClass_t::vendor,
		uint8_t(subclasses::vendor_t::none),
		uint8_t(protocols::vendor_t::flashprog),
		4 // "Flash access interface" string index
	},
	{
		sizeof(usbInterfaceDescriptor_t),
		usbDescriptor_t::interface,
		1, // Interface index 1
		0, // Alternate 0
		0, // No endpoints for this interface
		usbClass_t::application,
		uint8_t(subclasses::application_t::dfu),
		uint8_t(protocols::application_t::runtime),
		5, // "SPIFlashProgrammer Firmware Upgrade interface" string index
	}
}};

static const std::array<usbEndpointDescriptor_t, endpointDescriptorCount> usbEndpointDesc
{{
	{
		sizeof(usbEndpointDescriptor_t),
		usbDescriptor_t::endpoint,
		endpointAddress(usbEndpointDir_t::controllerOut, 1),
		usbEndpointType_t::interrupt,
		epBufferSize,
		1 // Poll once per frame
	},
	{
		sizeof(usbEndpointDescriptor_t),
		usbDescriptor_t::endpoint,
		endpointAddress(usbEndpointDir_t::controllerIn, 1),
		usbEndpointType_t::interrupt,
		epBufferSize,
		1 // Poll once per frame
	}
}};

static const dfu::functionalDescriptor_t usbDFUFunctionalDesc
{
	sizeof(dfu::functionalDescriptor_t),
	dfu::descriptor_t::functional,
	{dfu::willDetach_t::yes, dfu::manifestationTolerant_t::no, dfu::canUpload_t::no, dfu::canDownload_t::yes},
	10, // Set the detach timeout to 10ms
	epBufferSize, // Set the max transfer size to the endpoint buffer size
	0x011A // This is 1.1a in USB's BCD format
};

static const std::array<usbMultiPartDesc_t, 6> usbConfigSecs
{{
	{
		sizeof(usbConfigDescriptor_t),
		&usbConfigDesc[0]
	},
	{
		sizeof(usbInterfaceDescriptor_t),
		&usbInterfaceDesc[0]
	},
	{
		sizeof(usbEndpointDescriptor_t),
		&usbEndpointDesc[0]
	},
	{
		sizeof(usbEndpointDescriptor_t),
		&usbEndpointDesc[1]
	},
	{
		sizeof(usbInterfaceDescriptor_t),
		&usbInterfaceDesc[1]
	},
	{
		sizeof(dfu::functionalDescriptor_t),
		&usbDFUFunctionalDesc
	}
}};

namespace usb::descriptors
{
	const std::array<usbMultiPartTable_t, configsCount> usbConfigDescriptors
	{{
		{usbConfigSecs.begin(), usbConfigSecs.end()}
	}};
} // namespace usb::descriptors

static const std::array<usbStringDesc_t, stringCount + 1U> usbStringDescs
{{
	{{u"\x0904", 1}},
	{{u"bad_alloc Heavy Industries", 26}},
	{{u"SPIFlashProgrammer rev 2", 24}},
	{{u"", 0}}, // NOLINT(bugprone-string-constructor)
	{{u"Flash access interface", 22}},
	{{u"SPIFlashProgrammer Firmware Upgrade interface", 45}}
}};

static const std::array<std::array<usbMultiPartDesc_t, 2>, stringCount + 1U> usbStringParts
{{
	usbStringDescs[0].asParts(),
	usbStringDescs[1].asParts(),
	usbStringDescs[2].asParts(),
	usbStringDescs[3].asParts(),
	usbStringDescs[4].asParts(),
	usbStringDescs[5].asParts()
}};

static const std::array<usbMultiPartTable_t, stringCount + 1U> usbStrings
{{
	{usbStringParts[0].begin(), usbStringParts[0].end()},
	{usbStringParts[1].begin(), usbStringParts[1].end()},
	{usbStringParts[2].begin(), usbStringParts[2].end()},
	{usbStringParts[3].begin(), usbStringParts[3].end()},
	{usbStringParts[4].begin(), usbStringParts[4].end()},
	{usbStringParts[5].begin(), usbStringParts[5].end()}
}};

using namespace usb::types;
using namespace usb::core;

namespace usb::device
{
	answer_t handleGetDescriptor() noexcept
	{
		if (packet.requestType.dir() == endpointDir_t::controllerOut)
			return {response_t::unhandled, nullptr, 0};
		const auto descriptor = packet.value.asDescriptor();

		switch (descriptor.type)
		{
			// Handle device descriptor requests
			case usbDescriptor_t::device:
				return {response_t::data, &usbDeviceDesc, usbDeviceDesc.length};
			case usbDescriptor_t::deviceQualifier:
				return {response_t::stall, nullptr, 0};
			// Handle configuration descriptor requests
			case usbDescriptor_t::configuration:
			{
				if (descriptor.index >= configsCount)
					break;
				static_assert(sizeof(usbConfigDescriptor_t) == 9);
				static_assert(sizeof(usbInterfaceDescriptor_t) == 9);
				static_assert(sizeof(usbEndpointDescriptor_t) == 7);
				const auto &configDescriptor{usbConfigDescriptors[descriptor.index]};
				epStatusControllerIn[0].isMultiPart(true);
				epStatusControllerIn[0].partNumber = 0;
				epStatusControllerIn[0].partsData = &configDescriptor;
				return {response_t::data, nullptr, configDescriptor.totalLength()};
			}
			// Handle interface descriptor requests
			case usbDescriptor_t::interface:
			{
				if (descriptor.index >= interfaceDescriptorCount)
					break;
				const auto &interfaceDescriptor{usbInterfaceDesc[descriptor.index]};
				return {response_t::data, &interfaceDescriptor, interfaceDescriptor.length};
			}
			// Handle endpoint descriptor requests
			case usbDescriptor_t::endpoint:
			{
				if (descriptor.index >= endpointDescriptorCount)
					break;
				const auto &endpointDescriptor{usbEndpointDesc[descriptor.index]};
				return {response_t::data, &endpointDescriptor, endpointDescriptor.length};
			}
			// Handle string requests
			case usbDescriptor_t::string:
			{
				if (descriptor.index > stringCount)
					break;
				const auto &string{usbStrings[descriptor.index]};
				epStatusControllerIn[0].isMultiPart(true);
				epStatusControllerIn[0].partNumber = 0;
				epStatusControllerIn[0].partsData = &string;
				return {response_t::data, nullptr, string.totalLength()};
			}
			default:
				break;
		}
		return {response_t::unhandled, nullptr, 0};
	}
} // namespace usb::device
