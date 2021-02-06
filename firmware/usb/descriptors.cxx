// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include "tm4c123gh6pm/platform.hxx"
#include "core.hxx"
#include "descriptors.hxx"
#include "device.hxx"

using namespace usbTypes;
using namespace usbDescriptors;
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
	configDescriptorCount
};

static const usbDeviceQualifierDescriptor_t usbDeviceQualifierDesc
{
	sizeof(usbDeviceQualifierDescriptor_t),
	usbDescriptor_t::deviceQualifier,
	0x0200, // This is 2.00 in USB's BCD format
	usbClass_t::none,
	uint8_t(subclasses::device_t::none),
	uint8_t(protocols::device_t::none),
	epBufferSize,
	0, // No other configurations
	0 // reserved field that must be 0.
};

static const std::array<usbConfigDescriptor_t, configDescriptorCount> usbConfigDesc
{{
	{
		sizeof(usbConfigDescriptor_t),
		usbDescriptor_t::configuration,
		sizeof(usbConfigDescriptor_t) + sizeof(usbInterfaceDescriptor_t) +
			sizeof(usbEndpointDescriptor_t) + sizeof(usbEndpointDescriptor_t),
		interfaceDescriptorCount,
		1, // This config
		4, // Configuration string index
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
		0 // No string to describe this interface (for now)
	}
}};

static const std::array<usbEndpointDescriptor_t, endpointDescriptorCount> usbEndpointDesc
{{
	{
		sizeof(usbEndpointDescriptor_t),
		usbDescriptor_t::endpoint,
		endpointAddress(usbEndpointDir_t::controllerOut, 1),
		usbEndpointType_t::bulk,
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

static const std::array<usbMultiPartDesc_t, 4> usbConfigSecs
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
	}
}};

static const std::array<usbMultiPartTable_t, 1> usbConfigDescriptors
{{
	{usbConfigSecs.begin(), usbConfigSecs.end()}
}};

static const std::array<usbStringDesc_t, stringCount + 1> usbStringDescs
{{
	{{u"\x0904", 1}},
	{{u"bad_alloc Heavy Industries", 27}},
	{{u"SPIFlashProgrammer rev 2", 25}},
	{{u"", 1}},
	{{u"Flash access interface", 23}}
}};

static const std::array<std::array<usbMultiPartDesc_t, 2>, stringCount + 1> usbStringParts
{{
	usbStringDescs[0].asParts(),
	usbStringDescs[1].asParts(),
	usbStringDescs[2].asParts(),
	usbStringDescs[3].asParts(),
	usbStringDescs[4].asParts()
}};

static const std::array<usbMultiPartTable_t, stringCount + 1> usbStrings
{{
	{usbStringParts[0].begin(), usbStringParts[0].end()},
	{usbStringParts[1].begin(), usbStringParts[1].end()},
	{usbStringParts[2].begin(), usbStringParts[2].end()},
	{usbStringParts[3].begin(), usbStringParts[3].end()},
	{usbStringParts[4].begin(), usbStringParts[4].end()}
}};

using namespace usbTypes;
using namespace usbCore;

namespace usbDevice
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
				return {response_t::data, &usbDeviceQualifierDesc, usbDeviceQualifierDesc.length};
			// Handle configuration descriptor requests
			case usbDescriptor_t::configuration:
			{
				if (descriptor.index >= configDescriptorCount)
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
				gpioD.dataBits[4] = 4;
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
} // namespace usbDevice
