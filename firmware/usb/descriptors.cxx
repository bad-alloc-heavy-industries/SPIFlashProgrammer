// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include <usb/descriptors.hxx>
#include "constants.hxx"

using namespace std::literals::string_view_literals;

namespace usb::descriptors
{
	const usbDeviceDescriptor_t deviceDescriptor
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

	static const std::array<usbConfigDescriptor_t, configsCount> configDescs
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

	const std::array<usbInterfaceDescriptor_t, interfaceDescriptorCount> interfaceDescriptors
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
			uint8_t(protocols::dfu_t::runtime),
			5, // "SPIFlashProgrammer Firmware Upgrade interface" string index
		}
	}};

	const std::array<usbEndpointDescriptor_t, endpointDescriptorCount> endpointDescriptors
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
			usbEndpointType_t::bulk,
			epBufferSize,
			1 // Poll once per frame
		}
	}};

	static const dfu::functionalDescriptor_t dfuFunctionalDesc
	{
		sizeof(dfu::functionalDescriptor_t),
		dfu::descriptor_t::functional,
		{dfu::willDetach_t::yes, dfu::manifestationTolerant_t::no, dfu::canUpload_t::no, dfu::canDownload_t::yes},
		// Set the detach timeout to 1000ms which should be the upper bound for how long a PC takes to renumerate us
		1000,
		epBufferSize, // Set the max transfer size to the endpoint buffer size
		0x0110 // This is 1.1 in USB's BCD format
	};

	static const std::array<usbMultiPartDesc_t, 6> configSecs
	{{
		{
			sizeof(usbConfigDescriptor_t),
			&configDescs[0]
		},
		{
			sizeof(usbInterfaceDescriptor_t),
			&interfaceDescriptors[0]
		},
		{
			sizeof(usbEndpointDescriptor_t),
			&endpointDescriptors[0]
		},
		{
			sizeof(usbEndpointDescriptor_t),
			&endpointDescriptors[1]
		},
		{
			sizeof(usbInterfaceDescriptor_t),
			&interfaceDescriptors[1]
		},
		{
			sizeof(dfu::functionalDescriptor_t),
			&dfuFunctionalDesc
		}
	}};

	const std::array<usbMultiPartTable_t, configsCount> configDescriptors
	{{
		{configSecs.begin(), configSecs.end()}
	}};

	static const std::array<usbStringDesc_t, stringCount> stringDescs
	{{
		{{u"bad_alloc Heavy Industries", 26}},
		{{u"SPIFlashProgrammer rev 2", 24}},
		{{u"", 0}}, // NOLINT(bugprone-string-constructor)
		{{u"Flash access interface", 22}},
		{{u"SPIFlashProgrammer Firmware Upgrade interface", 45}}
	}};

	static const std::array<std::array<usbMultiPartDesc_t, 2>, stringCount> stringParts
	{{
		stringDescs[0].asParts(),
		stringDescs[1].asParts(),
		stringDescs[2].asParts(),
		stringDescs[3].asParts(),
		stringDescs[4].asParts()
	}};

	const std::array<usbMultiPartTable_t, stringCount> strings
	{{
		{stringParts[0].begin(), stringParts[0].end()},
		{stringParts[1].begin(), stringParts[1].end()},
		{stringParts[2].begin(), stringParts[2].end()},
		{stringParts[3].begin(), stringParts[3].end()},
		{stringParts[4].begin(), stringParts[4].end()}
	}};
} // namespace usb::descriptors
