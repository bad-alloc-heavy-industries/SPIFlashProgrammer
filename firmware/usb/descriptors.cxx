// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include "types.hxx"
#include "descriptors.hxx"

using namespace usb::descriptors;

static const usbDeviceDescriptor_t usbDeviceDesc
{
	sizeof(usbDeviceDescriptor_t),
	usbDescriptor_t::device,
	0x0200, // this is 2.00 in USB's BCD format
	usbClass_t::none,
	uint8_t(subclasses::device_t::none),
	uint8_t(protocols::device_t::none),
	usbTypes::epBufferSize,
	usbTypes::vid,
	usbTypes::pid,
	0x0001, // BCD encoded device version
	1, // Manufacturer string index
	2, // Product string index
	0, // Temporarily do not support a serial number string
	usbTypes::configDescriptorCount
};

static const std::array<usbConfigDescriptor_t, usbTypes::configDescriptorCount> usbConfigDesc
{{
	{
		sizeof(usbConfigDescriptor_t),
		usbDescriptor_t::configuration,
		sizeof(usbConfigDescriptor_t) + sizeof(usbInterfaceDescriptor_t) +
			sizeof(usbEndpointDescriptor_t) + sizeof(usbEndpointDescriptor_t),
		usbTypes::interfaceDescriptorCount,
		1, // This config
		4, // Configuration string index
		usbConfigAttr_t::defaults,
		50 // 100mA (the max our regulator can do)
	}
}};

static const std::array<usbInterfaceDescriptor_t, usbTypes::interfaceDescriptorCount> usbInterfaceDesc
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

static const std::array<usbEndpointDescriptor_t, usbTypes::endpointDescriptorCount> usbEndpointDesc
{{
	{
		sizeof(usbEndpointDescriptor_t),
		usbDescriptor_t::endpoint,
		endpointAddress(usbEndpointDir_t::controllerOut, 1),
		usbEndpointType_t::bulk,
		usbTypes::epBufferSize,
		1 // Poll once per frame
	},
	{
		sizeof(usbEndpointDescriptor_t),
		usbDescriptor_t::endpoint,
		endpointAddress(usbEndpointDir_t::controllerIn, 1),
		usbEndpointType_t::bulk,
		usbTypes::epBufferSize,
		1 // Poll once per frame
	}
}};

static const usbMultiPartDesc_t usbConfigSecs[]
{
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
};

static const std::array<usbStringDesc_t, usbTypes::stringCount> usbStrings
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