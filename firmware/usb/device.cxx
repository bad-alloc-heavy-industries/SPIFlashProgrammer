// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "usb/types.hxx"
#include "usb/device.hxx"

using namespace usbTypes;
void usbHandleStatusCtrlEP() noexcept;

answer_t usbHandleStandardRequest() noexcept
{
	const auto &packet{*static_cast<setupPacket_t *>(usbStatusOutEP[0].memBuffer)};
	const auto &epStatus{usbStatusInEP[0]};

	switch (packet.request)
	{
		case request_t::setAddress:
			usbState = deviceState_t::addressing;
			return {response_t::zeroLength, nullptr, 0};
	}

	return {response_t::unhandled, nullptr, 0};
}

void usbServiceCtrlEPRead() noexcept
{
	auto &epStatus{usbStatusOutEP[0]};
	uint8_t *const recvBuffer = static_cast<uint8_t *>(epStatus.memBuffer);
	auto &ep0 = usb.ep0Ctrl;
	auto readCount = ep0.rxCount;
	// Bounds sanity and then adjust how much is left to transfer
	if (readCount > epStatus.transferCount)
		readCount = epStatus.transferCount;
	epStatus.transferCount -= readCount;
	// Copy the received data to the user buffer
	for (uint8_t i{}; i < readCount & 0xFCU; i += 4)
		readFIFO<uint32_t>(usb.ep0FIFO, recvBuffer + i);
	if (readCount & 0x02U)
		readFIFO<uint16_t>(usb.ep0FIFO, recvBuffer + (readCount & 0xFEU) - 1);
	if (readCount & 0x01U)
		readFIFO<uint8_t>(usb.ep0FIFO, recvBuffer + readCount - 1);
	// Mark the FIFO contents as done with, and store the new start of buffer
	ep0.statusCtrlL |= vals::usb::epStatusCtrlLRxReadyClr;
	epStatus.memBuffer = recvBuffer + readCount;

	if (epStatus.transferCount > 0)
	{
	}
	else
	{
		epStatus.needsArming(false);
		usbHandleStatusCtrlEP();
	}
}

void usbHandleDataCtrlEP() noexcept
{
}

void usbHandleStatusCtrlEP() noexcept
{
}

void usbServiceCtrlEPComplete() noexcept
{
	auto &ep0 = usb.ep0Ctrl;

	if (!usbStatusInEP[0].needsArming())
	{
		if (usbStatusOutEP[0].needsArming())
		{
			usbCtrlState = ctrlState_t::rx;
			//if ((usbDeferalFlags & USB_DEFER_OUT_PACKETS) == 0)
				usbHandleDataCtrlEP();
		}
		else if (usbStatusInEP[0].stall())
		{
			ep0.statusCtrlL |= vals::usb::epStatusCtrlLStall;
			usbCtrlState = ctrlState_t::idle;
		}
	}
	else
	{
		const auto &packet{*reinterpret_cast<setupPacket_t *>(usbStatusOutEP[0].memBuffer)};
		if (packet.requestType.dir() == endpointDir_t::controllerIn)
		{
			usbCtrlState = ctrlState_t::tx;
			//if ((usbDeferalFlags & USB_DEFER_IN_PACKETS) == 0)
				usbHandleDataCtrlEP();
			//if ((usbDeferalFlags & USB_DEFER_STATUS_PACKETS) == 0)
				usbHandleStatusCtrlEP();
		}
		else
		{
			usbCtrlState = ctrlState_t::rx;

			//if ((usbDeferalFlags & USB_DEFER_STATUS_PACKETS) == 0)
				usbHandleStatusCtrlEP();
		}
	}
}

void usbHandleCtrlEPSetup() noexcept
{
	// Set up EP0 state for a reply of some kind
	//usbDeferalFlags = 0;
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
	usbStatusInEP[0].memBuffer = data;
	usbServiceCtrlEPComplete();
}

void usbHandleCtrlEPOut() noexcept
{
	if (usbCtrlState == ctrlState_t::rx)
		usbServiceCtrlEPRead();
}

void usbHandleCtrlEPIn() noexcept
{
	if (usbState == deviceState_t::addressing)
	{
		// We just handled an addressing request, and prepared our answer. Before we get a chance
		// to return from the interrupt that caused this chain of events, lets set the device address.
		const auto &packet{*static_cast<setupPacket_t *>(usbStatusOutEP[0].memBuffer)};
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
