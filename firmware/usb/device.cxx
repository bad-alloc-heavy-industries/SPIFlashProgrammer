// SPDX-License-Identifier: BSD-3-Clause
#include "usb/types.hxx"
#include "usb/device.hxx"

void usbHandleCtrlEPSetup()
{
}

void usbHandleCtrlEPOut()
{
}

void usbHandleCtrlEPIn()
{
}

void usbServiceCtrlEP() noexcept
{
	if (usbPacket.endpoint() != 0)
		return;
	if (usbPacket.dir() == usbTypes::endpointDir_t::controllerOut)
	{
		if (usbCtrlState == usbTypes::ctrlState_t::idle)
			usbHandleCtrlEPSetup();
		else
			usbHandleCtrlEPOut();
	}
	else
		usbHandleCtrlEPIn();
}
