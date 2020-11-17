// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_DEVICE__HXX
#define USB_DEVICE__HXX

#include <cstdint>
#include "types.hxx"

extern void usbServiceCtrlEP() noexcept;
extern usbTypes::answer_t usbHandleStandardRequest() noexcept;

#endif /*USB_DEVICE__HXX*/
