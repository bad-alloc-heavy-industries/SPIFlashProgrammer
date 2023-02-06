// SPDX-License-Identifier: BSD-3-Clause
#ifndef SFDP_HXX
#define SFDP_HXX

#include <cstdint>
#include "usbContext.hxx"

namespace sfdp
{
	void readAndDisplay(const usbDeviceHandle_t &device, uint8_t interface);
} // namespace sfdp

#endif /*SFDP_HXX*/
