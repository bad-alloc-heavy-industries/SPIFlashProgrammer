// SPDX-License-Identifier: BSD-3-Clause
#ifndef SFDP_HXX
#define SFDP_HXX

#include <cstdint>
#include "usbContext.hxx"

namespace sfdp
{
	struct usbDataSource_t
	{
		uint8_t interface;
		uint8_t endpoint;
	};

	bool readAndDisplay(const usbDeviceHandle_t &device, usbDataSource_t dataSource);
} // namespace sfdp

#endif /*SFDP_HXX*/
