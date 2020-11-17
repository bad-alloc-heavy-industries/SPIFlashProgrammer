// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_DEVICE__HXX
#define USB_DEVICE__HXX

#include <cstdint>
#include "types.hxx"

extern usbTypes::usbEP_t usbPacket;
extern usbTypes::ctrlState_t usbCtrlState;

extern std::array<usbTypes::usbEPStatus_t, usbTypes::endpointCount> usbStatusInEP;
extern std::array<usbTypes::usbEPStatus_t, usbTypes::endpointCount> usbStatusOutEP;

extern void usbServiceCtrlEP() noexcept;
extern usbTypes::answer_t usbHandleStandardRequest() noexcept;

#endif /*USB_DEVICE__HXX*/
