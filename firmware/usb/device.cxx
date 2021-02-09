// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "usb/core.hxx"
#include "usb/device.hxx"

using namespace usbTypes;
using namespace usbCore;
void usbHandleStatusCtrlEP() noexcept;

namespace usbDevice
{
	setupPacket_t packet;

	answer_t handleStandardRequest() noexcept
	{
		const auto &epStatus{epStatusControllerIn[0]};

		switch (packet.request)
		{
			case request_t::setAddress:
				usbState = deviceState_t::addressing;
				return {response_t::zeroLength, nullptr, 0};
			case request_t::getDescriptor:
				return handleGetDescriptor();
			case request_t::setConfiguration:
				// TODO: build initialisation logic for bringing up the EP1 "driver" and endpoint configuration.
				// handleSetConfiguration();
				// Acknowledge the request.
				return {response_t::zeroLength, nullptr, 0};
		}

		return {response_t::unhandled, nullptr, 0};
	}
}

/*!
 * @returns true when the all the data to be read has been retreived,
 * false if there is more left to fetch.
 */
bool usbServiceCtrlEPRead() noexcept
{
	auto &epStatus{epStatusControllerOut[0]};
	auto readCount = usbCtrl.ep0Ctrl.rxCount;
	// Bounds sanity and then adjust how much is left to transfer
	if (readCount > epStatus.transferCount)
		readCount = epStatus.transferCount;
	epStatus.transferCount -= readCount;
	epStatus.memBuffer = recvData(0, static_cast<uint8_t *>(epStatus.memBuffer), readCount);
	// Mark the FIFO contents as done with
	if (epStatus.transferCount || usbCtrlState == ctrlState_t::statusRX)
		usbCtrl.ep0Ctrl.statusCtrlL |= vals::usb::epStatusCtrlLRxReadyClr;
	else
		usbCtrl.ep0Ctrl.statusCtrlL |= vals::usb::epStatusCtrlLRxReadyClr | vals::usb::epStatusCtrlLDataEnd;
	return !epStatus.transferCount;
}

/*!
 * @returns true when the data to be transmitted is entirely sent,
 * false if there is more left to send.
 */
bool usbServiceCtrlEPWrite() noexcept
{
	if (epStatusControllerIn[0].transferCount < usbTypes::epBufferSize)
	{
	}

	auto &epStatus{epStatusControllerIn[0]};
	const auto sendCount{[&]() noexcept -> uint8_t
	{
		// Bounds sanity and then adjust how much is left to transfer
		if (epStatus.transferCount < usbTypes::epBufferSize)
			return epStatus.transferCount;
		return usbTypes::epBufferSize;
	}()};
	epStatus.transferCount -= sendCount;

	if (!epStatus.isMultiPart())
		epStatus.memBuffer = sendData(0, static_cast<const uint8_t *>(epStatus.memBuffer), sendCount);
	else
	{
		std::array<uint8_t, 4> leftoverBytes{};
		uint8_t leftoverCount{};

		if (!epStatus.memBuffer)
			epStatus.memBuffer = epStatus.partsData->part(0).descriptor;
		auto sendAmount{sendCount};
		while (sendAmount)
		{
			const auto &part{epStatus.partsData->part(epStatus.partNumber)};
			auto *const begin{static_cast<const uint8_t *>(part.descriptor)};
			const auto partAmount{[&]() -> uint8_t
			{
				auto *const buffer{static_cast<const uint8_t *>(epStatus.memBuffer)};
				const auto amount{part.length - (buffer - begin)};
				if (amount > sendAmount)
					return sendAmount;
				return amount;
			}()};
			sendAmount -= partAmount;
			// If we have bytes left over from the previous loop
			if (leftoverCount)
			{
				// How many bytes do we need to completely fill the leftovers buffer
				const auto diffAmount{leftoverBytes.size() - leftoverCount};
				// Copy that in and queue it from the front of the new chunk
				memcpy(leftoverBytes.data() + leftoverCount, epStatus.memBuffer, diffAmount);
				sendData(0, leftoverBytes.data(), leftoverBytes.size());

				// Now compute how many bytes will be left at the end of this new chunk
				// in queueing only amounts divisable-by-4
				const auto remainder{(partAmount - diffAmount) & 0x03U};
				// Queue as much as we can
				epStatus.memBuffer = sendData(0, static_cast<const uint8_t *>(epStatus.memBuffer) + diffAmount,
					(partAmount - diffAmount) - remainder) + remainder;
				// And copy any new leftovers to the leftovers buffer.
				memcpy(leftoverBytes.data(), static_cast<const uint8_t *>(epStatus.memBuffer) - remainder, remainder);
				leftoverCount = remainder;
			}
			else
			{
				// How many bytes will be left over by queueing only a divisible-by-4 amount
				const auto remainder{partAmount & 0x03U};
				// Queue as much as we can
				epStatus.memBuffer = sendData(0, static_cast<const uint8_t *>(epStatus.memBuffer),
					partAmount - remainder) + remainder;
				// And copy the leftovers to the leftovers buffer.
				memcpy(leftoverBytes.data(), static_cast<const uint8_t *>(epStatus.memBuffer) - remainder, remainder);
				leftoverCount = remainder;
			}
			// Get the buffer back to check if we exhausted it
			auto *const buffer{static_cast<const uint8_t *>(epStatus.memBuffer)};
			if (buffer - begin == part.length &&
					epStatus.partNumber + 1 < epStatus.partsData->count())
				// We exhausted the chunk's buffer, so grab the next chunk
				epStatus.memBuffer = epStatus.partsData->part(++epStatus.partNumber).descriptor;
		}
		if (!epStatus.transferCount)
		{
			if (leftoverCount)
				sendData(0, leftoverBytes.data(), leftoverCount);
			epStatus.isMultiPart(false);
		}
	}
	// Mark the FIFO contents as done with
	if (epStatus.transferCount || usbCtrlState == ctrlState_t::statusTX)
		usbCtrl.ep0Ctrl.statusCtrlL |= vals::usb::epStatusCtrlLTxReady;
	else
		usbCtrl.ep0Ctrl.statusCtrlL |= vals::usb::epStatusCtrlLTxReady | vals::usb::epStatusCtrlLDataEnd;
	return !epStatus.transferCount;
}

void usbHandleDataCtrlEP() noexcept
{
	if (usbCtrlState == ctrlState_t::dataTX)
	{
		if (epStatusControllerIn[0].transferCount > usbDevice::packet.length)
			epStatusControllerIn[0].transferCount = usbDevice::packet.length;
		usbServiceCtrlEPWrite();
	}
}

void usbHandleStatusCtrlEP() noexcept
{
}

namespace usbDevice
{
	void completeSetupPacket() noexcept
	{
		auto &ep0 = usbCtrl.ep0Ctrl;

		// If we have no response
		if (!epStatusControllerIn[0].needsArming())
		{
			// But rather need more data
			if (epStatusControllerOut[0].needsArming())
			{
				// <SETUP[0]><OUT[1]><OUT[0]>...<IN[1]>
				usbCtrlState = ctrlState_t::dataRX;
			}
			// We need to stall in answer
			else if (epStatusControllerIn[0].stall())
			{
				// <SETUP[0]><STALL>
				ep0.statusCtrlL |= vals::usb::epStatusCtrlLStall;
				usbCtrlState = ctrlState_t::idle;
			}
		}
		// We have a valid response
		else
		{
			// Is this as part of a multi-part transaction?
			if (packet.requestType.dir() == endpointDir_t::controllerIn)
				// <SETUP[0]><IN[1]><IN[0]>...<OUT[1]>
				usbCtrlState = ctrlState_t::dataTX;
			// Or just a quick answer?
			else
				//  <SETUP[0]><IN[1]>
				usbCtrlState = ctrlState_t::statusTX;
			if (usbServiceCtrlEPWrite())
			{
				if (usbCtrlState == ctrlState_t::dataTX)
					usbCtrlState = ctrlState_t::statusRX;
				else
					usbCtrlState = ctrlState_t::idle;
			}
		}
	}

	void handleSetupPacket() noexcept
	{
		// Read in the new setup packet
		static_assert(sizeof(setupPacket_t) == 8); // Setup packets must be 8 bytes.
		epStatusControllerOut[0].memBuffer = &packet;
		epStatusControllerOut[0].transferCount = sizeof(setupPacket_t);
		if (!usbServiceCtrlEPRead())
		{
			// Truncated transfer.. WTF.
			usbCtrl.ep0Ctrl.statusCtrlL |= vals::usb::epStatusCtrlLStall;
			return;
		}

		// Set up EP0 state for a reply of some kind
		//usbDeferalFlags = 0;
		usbCtrlState = ctrlState_t::wait;
		epStatusControllerIn[0].needsArming(false);
		epStatusControllerIn[0].stall(false);
		epStatusControllerIn[0].transferCount = 0;
		epStatusControllerOut[0].needsArming(false);
		epStatusControllerOut[0].stall(false);
		epStatusControllerOut[0].transferCount = 0;

		const auto &[response, data, size] = handleStandardRequest();

		epStatusControllerIn[0].stall(response == response_t::stall || response == response_t::unhandled);
		epStatusControllerIn[0].needsArming(response == response_t::data || response == response_t::zeroLength);
		epStatusControllerIn[0].memBuffer = data;
		const uint16_t transferCount = response == response_t::zeroLength ? 0U : size;
		epStatusControllerIn[0].transferCount = std::min(transferCount, packet.length);
		// If the response is whacko, don't do the stupid thing
		if (response == response_t::data && !data && !epStatusControllerIn[0].isMultiPart())
			epStatusControllerIn[0].needsArming(false);
		completeSetupPacket();
	}

	void handleControllerOutPacket() noexcept
	{
		// If we're in the data phase
		if (usbCtrlState == ctrlState_t::dataRX)
		{
			if (usbServiceCtrlEPRead())
			{
				// If we now have all the data for the transaction..
				usbCtrlState = ctrlState_t::statusTX;
				// TODO: Handle data and generate status response.
			}
		}
		// If we're in the status phase
		else
			usbCtrlState = ctrlState_t::idle;
	}

	void handleControllerInPacket() noexcept
	{
		if (usbState == deviceState_t::addressing)
		{
			// We just handled an addressing request, and prepared our answer. Before we get a chance
			// to return from the interrupt that caused this chain of events, lets set the device address.
			const auto address{packet.value.asAddress()};

			// Check that the last setup packet was actually a set address request
			if (packet.requestType.type() != setupPacket::request_t::typeStandard ||
				packet.request != request_t::setAddress || address.addrH != 0)
			{
				usbCtrl.address &= vals::usb::addressClrMask;
				usbState = deviceState_t::waiting;
			}
			else
			{
				usbCtrl.address = (usbCtrl.address & vals::usb::addressClrMask) |
					(address.addrL & vals::usb::addressMask);
				usbState = deviceState_t::addressed;
			}
		}

		// If we're in the data phase
		if (usbCtrlState == ctrlState_t::dataTX)
		{
			if (usbServiceCtrlEPWrite())
			{
				// If we now have all the data for the transaction..
				//usbCtrlState = ctrlState_t::statusRX;
				usbCtrlState = ctrlState_t::idle;
			}
		}
		// Otherwise this was a status phase TX-complete interrupt
		else
			usbCtrlState = ctrlState_t::idle;
	}

	void handleControlPacket() noexcept
	{
		if (usbCtrl.ep0Ctrl.statusCtrlL & vals::usb::epStatusCtrlLSetupEnd)
			usbCtrl.ep0Ctrl.statusCtrlL |= vals::usb::epStatusCtrlLSetupEndClr;
		// If we received a packet..
		if (usbPacket.dir() == endpointDir_t::controllerOut)
		{
			if (usbCtrlState == ctrlState_t::idle)
				handleSetupPacket();
			else
				handleControllerOutPacket();
		}
		else
			handleControllerInPacket();
	}
} // namespace usbDevice
