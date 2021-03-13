// SPDX-License-Identifier: BSD-3-Clause
#include "tm4c123gh6pm/platform.hxx"
#include "tm4c123gh6pm/constants.hxx"
#include "platform.hxx"
#include "usb.hxx"
#include "usb/core.hxx"
#include "usb/device.hxx"
#include "usb/flashProto.hxx"
#include "indexedIterator.hxx"

/*!
 * USB pinout:
 * PD4 - D-
 * PD5 - D+
 *
 * PB0 - ID
 * PB1 - VBus
 */

using namespace usb::types;

namespace usb::core
{
	deviceState_t usbState;
	usbEP_t usbPacket;
	bool usbSuspended;
	ctrlState_t usbCtrlState;
	uint8_t usbDeferalFlags;

	std::array<usbEPStatus_t<const void>, endpointCount> epStatusControllerIn;
	std::array<usbEPStatus_t<void>, endpointCount> epStatusControllerOut;
} // namespace usb::core

using namespace usb::core;

/*!
 * Transmitting packets:
 * Write data to endpoint TXFIFO register up to 4 bytes at a time,
 * but this must be kept consistent up to the last transfer.
 * Write TXRDY bit for the end point once ready for arming transmit.
 * On completion of transmission, TXRDY and FIFONE (FIFO Not Empty) get cleared.
 * When double-buffered, TXRDY gets immediately cleared when the second buffer
 * is also empty.
 * When either kind of transmission complete fires, a TX interrupt is triggered.
 *
 * Receiving packets:
 * Once data reception (automatic) is complete, RXRDY bit is set.
 * If double-buffered and the second buffer is now also full or if single-buffered,
 * endpoint FULL is set.
 * Read data from RXFIFO up to 4 bytes at a time, but this must also be kept
 * consistent up to the last transfer.
 * Once complete, RXDRY must be cleared to re-arm buffer. This also generates host ACK.
 * If single-buffered or this freed the second buffer in double-buffering,
 * this clears the FULL bit.
 *
 * DATAEND is the end-of-data-phase start-of-status-phase indicator
 */

void usbInit() noexcept
{
	// Enable the USB peripheral
	sysCtrl.runClockGateCtrlUSB |= vals::sysCtrl::runClockGateCtrlUSB;
	// and wait for it to become ready
	while (!(sysCtrl.periphReadyUSB & vals::sysCtrl::periphReadyUSB))
		continue;

	// Switch over to port B & D's AHB apeture
	sysCtrl.gpioAHBCtrl |= vals::sysCtrl::gpioAHBCtrlPortB | vals::sysCtrl::gpioAHBCtrlPortD;
	// Enable port B & D
	sysCtrl.runClockGateCtrlGPIO |= vals::sysCtrl::runClockGateCtrlGPIOB | vals::sysCtrl::runClockGateCtrlGPIOD;
	// Wait for the ports to come online
	while (!(sysCtrl.periphReadyGPIO & (vals::sysCtrl::periphReadyGPIOB | vals::sysCtrl::periphReadyGPIOD)))
		continue;

	// Configure the D+/D- pins as USB peripheral IO
	gpioD.amSel |= 0x30U;
	gpioD.portCtrl |= vals::gpio::portD::portCtrlPin4USBDataMinus | vals::gpio::portD::portCtrlPin5USBDataPlus;
	gpioD.afSel |= 0x30U;
	//gpioD.dir |= 0x30U;

	// Configure the ID and VBus pins as USB peripheral IO
	gpioB.amSel |= 0x03U;
	gpioB.portCtrl |= vals::gpio::portB::portCtrlPin0USBID | vals::gpio::portB::portCtrlPin1USBVBus;
	gpioB.afSel |= 0x03U;
	gpioB.dir &= ~0x01U;

	// Put the controller in device mode and reset the power control register completely.
	usbCtrl.gpCtrlStatus = vals::usb::gpCtrlStatusDeviceMode | vals::usb::gpCtrlStatusOTGModeDevice;
	usbCtrl.power &= vals::usb::powerMask;

	// Reset all USB interrupts
	usbCtrl.intEnable &= vals::usb::itrEnableDeviceMask;
	usbCtrl.txIntEnable &= vals::usb::txItrEnableMask;
	usbCtrl.rxIntEnable &= vals::usb::rxItrEnableMask;
	// And their flags
	vals::readDiscard(usbCtrl.intStatus);
	vals::readDiscard(usbCtrl.txIntStatus);
	vals::readDiscard(usbCtrl.rxIntStatus);
	// Ensure the device address is 0
	usbCtrl.address = 0;

	// Enable the USB NVIC and reset interrupt
	nvic.enableInterrupt(44);
	usbCtrl.intEnable |= vals::usb::itrEnableDeviceReset;

	// Initialise the state machine
	usbState = deviceState_t::detached;
	usbCtrlState = ctrlState_t::idle;
	usbDeferalFlags = 0;
	usbCtrl.power |= vals::usb::powerSoftConnect;
}

namespace usb::core
{
	void reset() noexcept
	{
		// Set up only EP0.
		usbCtrl.epIndex = 0;
		// 128 / 2 = 64, so this gives us 64 bytes per EP.
		usbCtrl.txFIFOSize = vals::usb::fifoSize128 | vals::usb::fifoSizeDoubleBuffered;
		usbCtrl.txFIFOAddr = vals::usb::fifoAddr(0);
		usbCtrl.rxFIFOSize = vals::usb::fifoSize128 | vals::usb::fifoSizeDoubleBuffered;
		usbCtrl.rxFIFOAddr = vals::usb::fifoAddr(128);
		// Really enable the double-buffers as apparently this isn't done just by the above.
		usbCtrl.txPacketDoubleBuffEnable |= vals::usb::txPacketDoubleBuffEnableEP1;
		usbCtrl.rxPacketDoubleBuffEnable |= vals::usb::rxPacketDoubleBuffEnableEP1;
		resetEPs(epReset_t::all);

		// Once we get done, idle the peripheral
		usbCtrl.address = 0;
		usbState = deviceState_t::attached;
		usbCtrl.intEnable |= vals::usb::itrEnableDisconnect | vals::usb::itrEnableSOF;
		usbCtrl.txIntEnable &= vals::usb::txItrEnableMask;
		usbCtrl.rxIntEnable &= vals::usb::rxItrEnableMask;
		usbCtrl.txIntEnable |= vals::usb::txItrEnableEP0;
		usb::device::activeConfig = 0;
	}

	void resetEPs(const epReset_t what) noexcept
	{
		for (auto &[i, epStatus] : utility::indexedIterator_t{epStatusControllerIn})
		{
			if (what == epReset_t::user && i == 0)
				continue;
			epStatus->resetStatus();
			epStatus->transferCount = 0;
			epStatus->ctrl.endpoint(i);
			epStatus->ctrl.dir(endpointDir_t::controllerIn);
		}

		for (auto &[i, epStatus] : utility::indexedIterator_t{epStatusControllerOut})
		{
			if (what == epReset_t::user && i == 0)
				continue;
			epStatus->resetStatus();
			epStatus->transferCount = 0;
			epStatus->ctrl.endpoint(i);
			epStatus->ctrl.dir(endpointDir_t::controllerOut);
		}
	}

	void detach() noexcept
	{
		if (usbState == deviceState_t::detached)
			return;
		usbCtrl.power &= vals::usb::powerSoftDisconnectMask;
		usbCtrl.intEnable &= vals::usb::itrEnableDeviceMask;
		usbCtrl.power |= vals::usb::powerSoftConnect;
		usbCtrl.intEnable |= vals::usb::itrEnableDeviceReset;
		usbState = deviceState_t::detached;
	}

	void wakeup() noexcept
	{
		usbSuspended = false;
		usbCtrl.power |= vals::usb::powerResume;
		while ((usbCtrl.power & vals::usb::powerSuspend) == vals::usb::powerSuspend)
			continue;
		usbCtrl.power &= ~vals::usb::powerResume;
		usbCtrl.intEnable &= ~vals::usb::itrEnableResume;
		usbCtrl.intEnable |= vals::usb::itrEnableSuspend;
	}

	void suspend() noexcept
	{
		usbCtrl.intEnable &= ~vals::usb::itrEnableSuspend;
		usbCtrl.intEnable |= vals::usb::itrEnableResume;
		usbCtrl.power |= vals::usb::powerSuspend;
		usbSuspended = true;
	}

	const uint8_t *sendData(const uint8_t ep, const uint8_t *const buffer, const uint8_t length) noexcept
	{
		// Copy the data to tranmit from the user buffer
		for (uint8_t i{}; i < (length & 0xFCU); i += 4)
			writeFIFO_t<uint32_t>{}(usbCtrl.epFIFO[ep], buffer + i);
		if (length & 0x02U)
			writeFIFO_t<uint16_t>{}(usbCtrl.epFIFO[ep], buffer + (length & 0xFEU) - 2);
		if (length & 0x01U)
			writeFIFO_t<uint8_t>{}(usbCtrl.epFIFO[ep], buffer + length - 1);
		return buffer + length;
	}

	uint8_t *recvData(const uint8_t ep, uint8_t *const buffer, const uint8_t length) noexcept
	{
		// Copy the received data to the user buffer
		for (uint8_t i{}; i < (length & 0xFCU); i += 4)
			readFIFO_t<uint32_t>{}(usbCtrl.epFIFO[ep], buffer + i);
		if (length & 0x02U)
			readFIFO_t<uint16_t>{}(usbCtrl.epFIFO[ep], buffer + (length & 0xFEU) - 2);
		if (length & 0x01U)
			readFIFO_t<uint8_t>{}(usbCtrl.epFIFO[ep], buffer + length - 1);
		return buffer + length;
	}

	/*!
	* @returns true when the all the data to be read has been retreived,
	* false if there is more left to fetch.
	*/
	bool readEP(const uint8_t endpoint) noexcept
	{
		if (!endpoint)
			return false;
		auto &epStatus{epStatusControllerOut[endpoint]};
		auto readCount{usbCtrl.epCtrls[endpoint - 1].rxCount};
		// Bounds sanity and then adjust how much is left to transfer
		if (readCount > epStatus.transferCount)
			readCount = epStatus.transferCount;
		epStatus.transferCount -= readCount;
		epStatus.memBuffer = recvData(endpoint, static_cast<uint8_t *>(epStatus.memBuffer), readCount);
		// Mark the FIFO contents as done with
		usbCtrl.epCtrls[endpoint - 1].rxStatusCtrlL &= ~vals::usb::epStatusCtrlLRxReady;
		return !epStatus.transferCount;
	}

	/*!
	* @returns true when the data to be transmitted is entirely sent,
	* false if there is more left to send.
	*/
	bool writeEP(const uint8_t endpoint) noexcept
	{
		if (!endpoint)
			return false;
		auto &epStatus{epStatusControllerIn[endpoint]};
		const auto sendCount{[&]() noexcept -> uint8_t
		{
			// Bounds sanity and then adjust how much is left to transfer
			if (epStatus.transferCount < usb::types::epBufferSize)
				return epStatus.transferCount;
			return usb::types::epBufferSize;
		}()};
		epStatus.transferCount -= sendCount;

		if (!epStatus.isMultiPart())
			epStatus.memBuffer = sendData(endpoint, static_cast<const uint8_t *>(epStatus.memBuffer), sendCount);
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
					sendData(endpoint, leftoverBytes.data(), leftoverBytes.size());

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
					epStatus.memBuffer = sendData(endpoint, static_cast<const uint8_t *>(epStatus.memBuffer),
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
					sendData(endpoint, leftoverBytes.data(), leftoverCount);
				epStatus.isMultiPart(false);
			}
		}
		// Mark the FIFO contents as done with
		usbCtrl.epCtrls[endpoint - 1].txStatusCtrlL &= ~(vals::usb::epStatusCtrlLTxReady |
			vals::usb::epStatusCtrLTxUnderRun);
		return !epStatus.transferCount;
	}
} // namespace usb::core

void irqUSB() noexcept
{
	const auto status{usbCtrl.intStatus & usbCtrl.intEnable};
	const auto rxStatus{usbCtrl.rxIntStatus & usbCtrl.rxIntEnable};
	const auto txStatus{usbCtrl.txIntStatus & usbCtrl.txIntEnable};

	if (status & vals::usb::itrStatusDisconnect)
		return detach();
	else if (usbState == deviceState_t::attached)
	{
		usbCtrl.intEnable |= vals::usb::itrEnableSuspend;
		usbState = deviceState_t::powered;
	}

	if (status & vals::usb::itrStatusResume)
		wakeup();
	else if (usbSuspended)
		return;

	if (status & vals::usb::itrStatusDeviceReset)
	{
		reset();
		usbState = deviceState_t::waiting;
		return;
	}

	if (status & vals::usb::itrStatusSuspend)
		suspend();

	if (usbState == deviceState_t::detached ||
		usbState == deviceState_t::attached ||
		usbState == deviceState_t::powered ||
		(!rxStatus && !txStatus))
		return;

	for (uint8_t endpoint{}; endpoint < endpointCount; ++endpoint)
	{
		const uint16_t endpointMask = 1U << endpoint;
		if (rxStatus & endpointMask || txStatus & endpointMask)
		{
			usbPacket.endpoint(endpoint);
			if (rxStatus & endpointMask || (endpoint == 0 &&
					(usbCtrl.ep0Ctrl.statusCtrlL & vals::usb::epStatusCtrlLRxReady)))
				usbPacket.dir(endpointDir_t::controllerOut);
			else
				usbPacket.dir(endpointDir_t::controllerIn);

			if (endpoint == 0)
				usb::device::handleControlPacket();
			else if (usb::device::activeConfig == 1 && endpoint == 1)
				usb::flashProto::handlePacket();
		}
	}
}
