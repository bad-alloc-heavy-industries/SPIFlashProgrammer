// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include "types.hxx"
#include "descriptors.hxx"
#include "device.hxx"

using namespace usbTypes;
using namespace usbDescriptors;

static const usbDeviceDescriptor_t usbDeviceDesc
{
	sizeof(usbDeviceDescriptor_t),
	usbDescriptor_t::device,
	0x0200, // this is 2.00 in USB's BCD format
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
		0, // one endpoint to the interface
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
		usbEndpointType_t::bulk,
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

static const std::array<usbStringDesc_t, stringCount> usbStrings
{{
	{
		0,
		usbDescriptor_t::string,
		u"bad_alloc Heavy Industries"
	},
	{
		0,
		usbDescriptor_t::string,
		u"SPIFlashProgrammer rev 2"
	},
	{
		0,
		usbDescriptor_t::string,
		u""
	},
	{
		0,
		usbDescriptor_t::string,
		u"Flash access interface"
	}
}};

using namespace usbTypes;

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
			// Handle configuration descriptor requests
			case usbDescriptor_t::configuration:
			{
				if (descriptor.index >= configDescriptorCount)
					break;
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
				if (descriptor.index >= stringCount)
					break;
				const auto &string{usbStrings[descriptor.index]};
				return {response_t::data, &string, string.length};
			}
			default:
				break;
		}
		return {response_t::unhandled, nullptr, 0};
	}
} // namespace usbDevice
