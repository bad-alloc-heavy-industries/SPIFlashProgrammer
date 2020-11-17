// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "usb/types.hxx"
#include "usb/device.hxx"

using namespace usbTypes;

answer_t usbHandleStandardRequest() noexcept
{
	const auto &packet{*reinterpret_cast<setupPacket_t *>(usbStatusOutEP[0].usbBuffer)};
	const auto &epStatus{usbStatusInEP[0]};

	switch (packet.request)
	{
		case request_t::setAddress:
			usbState = deviceState_t::addressing;
			return {response_t::zeroLength, nullptr, 0};
	}

	return {response_t::unhandled, nullptr, 0};
}

void usbHandleDataCtrlEP()
{
}

void usbHandleStatusCtrlEP()
{
}

void usbServiceCtrlEPComplete()
{
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
	usbStatusInEP[0].srcBuffer = data;
	usbStatusInEP[0].transferCount = size;
	usbServiceCtrlEPComplete();
}

void usbHandleCtrlEPOut()
{
}

void usbHandleCtrlEPIn()
{
	if (usbState == deviceState_t::addressing)
	{
		// We just handled an addressing request, and prepared our answer. Before we get a chance
		// to return from the interrupt that caused this chain of events, lets set the device address.
		const auto &packet{*reinterpret_cast<setupPacket_t *>(usbStatusOutEP[0].usbBuffer)};
		const auto address{packet.value.asAddress()};

		if (packet.requestType.type() != setupPacket::request_t::typeStandard ||
			packet.request != request_t::setAddress || address.addrH != 0)
		{
			usb.address &= vals::usb::addressClrMask;
			usbState = deviceState_t::waiting;
		}
		else
		{
			usb.address = (usb.address & vals::usb::addressClrMask) |
				(address.addrL & vals::usb::addressMask);
			usbState = deviceState_t::addressed;
		}
		return;
	}
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
