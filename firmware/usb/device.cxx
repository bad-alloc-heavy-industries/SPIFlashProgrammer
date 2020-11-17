// SPDX-License-Identifier: BSD-3-Clause
#include "usb/types.hxx"
#include "usb/device.hxx"

using namespace usbTypes;

answer_t usbHandleStandardRequest() noexcept
{
	const auto &packet{*reinterpret_cast<setupPacket_t *>(usbStatusOutEP[0].usbBuffer)};
	const auto &epStatus{usbStatusInEP[0]};

	switch (packet.request)
	{
	}

	return {response_t::unhandled, nullptr, 0};
}

void usbHandleCtrlEPSetup()
{
	// Set up EP0 state for a reply of some kind
	usbCtrlState = ctrlState_t::wait;
	usbStatusInEP[0].needsArming(false);
	usbStatusInEP[0].stall(false);
	usbStatusInEP[0].transferCount = 0;
	usbStatusOutEP[0].needsArming(false);
	usbStatusOutEP[0].stall(false);
	usbStatusOutEP[0].transferCount = 0;

	const auto &[response, data, size] = usbHandleStandardRequest();

	usbStatusInEP[0].stall(response == response_t::stall || response == response_t::unhandled);
	usbStatusInEP[0].needsArming(response == response_t::data || response == response_t::zeroLength);
	usbStatusInEP[0].transferCount = size;
	usbServiceCtrlEPComplete();
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
	if (usbPacket.dir() == endpointDir_t::controllerOut)
	{
		if (usbCtrlState == ctrlState_t::idle)
			usbHandleCtrlEPSetup();
		else
			usbHandleCtrlEPOut();
	}
	else
		usbHandleCtrlEPIn();
}
