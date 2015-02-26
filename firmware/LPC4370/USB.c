/*
 * This file is part of SPI Flash Programmer (SPIFP)
 * Copyright © 2014 Rachel Mant (dx-mon@users.sourceforge.net)
 *
 * SPIFP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SPIFP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <LPC4370.h>
#include <stdint.h>
#include <stdbool.h>
#include "USB.h"
#include "USBTypes.h"

volatile usbDeviceState usbState;
volatile usbEP_t usbPacket;
volatile bool usbSuspended;
volatile usbCtrlState_t usbCtrlState;
volatile uint8_t usbDeferalFlags;
volatile usbStallState_t usbStallState;

volatile uint8_t usbActiveConfig, usbStatusTimeout;
volatile bool usbStageLock1 = false, usbStageLock2 = false;

usbEPStatus_t usbStatusInEP[USB_ENDPOINTS];
usbEPStatus_t usbStatusOutEP[USB_ENDPOINTS];
/* Defines the buffer descriptor table and places it at it's aligned address in RAM */
volatile usbBDTEntry_t usbBDT[USB_BDT_ENTRIES] USB_BTD_ADDR;
/* Define endpoint 0's buffers */
volatile usbTD_t setupTD USB_TD_ALIGN;
volatile usbTD_t dataTD USB_TD_ALIGN;
volatile uint8_t usbEP0Data[USB_EP0_DATA_LEN] USB_EP0_DATA_ADDR;

void usbInit()
{
	USB0->usbMode = USB_MODE_DEVICE | USB_MODE_SLOD;
	USB0->usbCmd &= USB_DCMD_STOP_MASK;
	usbReset();
	usbCtrlState = USB_CTRL_STATE_WAIT;
	usbStallState = USB_STALL_STATE_STALL;
	usbDeferalFlags = 0;
	usbStatusTimeout = 0;

	usbStatusInEP[0].epLen = USB_EP0_DATA_LEN;
}

void usbReset()
{
	uint8_t i;

	/* Reset all USB related interrupt enables */
	USB0->usbIE = 0;

	/* And flags */
	USB0->usbIF = 0;

	/*
	 * Configure EP0 as default control endpoint.
	 * By the defaults in the device, this leaves the others off,
	 * we just ensure that with the loop that follows.
	 */
	for (i = 0; i < USB_ENDPOINTS; i++)
		USB0->epCtrl[i] = 0;

	/*
	 * Enable necessary interrupts
	 * UEIE: Listen to all error interrupts
	 * usbIE: Listen to only:
	 *     USB Reset (URIE)
	 *     Transaction Complete ()
	 *     Idle Detected (SLIE)
	 *     Wakeup Detect (PCIE)
	 *     Stall Handshake ()
	 *     USB Error (UEIE)
	 *     Start-Of-Frame (SRIE)
	 */
	USB0->usbIE = USB_INT_UEIE | USB_INT_PCIE | USB_DINT_URIE | USB_INT_SRIE | USB_DINT_SLIE;

	/* Reset the ping-pong buffers, bus address and transfer status */
	USB0->epSetupStat = USB0->epSetupStat;
	USB0->epComplete = USB0->epComplete;
	while (USB0->epPrime);
	USB0->epFlush = USB_EP_ALL_MASK;
	USB0->epNAK = USB_EP_ALL_MASK;
	USB0->epNAKEnable &= ~USB_EP_ALL_MASK;
	USB0->deviceAddr = 0;

	for (i = 0; i < (USB_ENDPOINTS << 1); i++)
	{
		usbBDT[i].activeTD.nextTD = USB_INVALID_TD;
		usbBDT[i].epCaps &= ~(USB_EPCAP_LEN_MASK | USB_EPCAP_IOS | USB_EPCAP_ZLT | USB_EPCAP_MULT_MASK);
	}
	usbBDT[0].epCaps |= ((USB_EP0_SETUP_LEN << 16) & USB_EPCAP_LEN_MASK) | USB_EPCAP_IOS;
	usbBDT[1].epCaps |= (USB_EP0_DATA_LEN << 16) & USB_EPCAP_LEN_MASK;
	USB0->listAddr = (uint32_t)&usbBDT;

	/* Reset status flags */
	usbStageLock1 = false;
	usbStageLock2 = false;
	usbSuspended = false;

	/* Reset driver endpoint states */
	for (i = 0; i < USB_ENDPOINTS; i++)
	{
		usbStatusInEP[i].value = 0;
		usbStatusInEP[i].xferCount = 0;
		usbStatusInEP[i].ep.value = 0;
		usbStatusInEP[i].ep.epNum = i;
		usbStatusInEP[i].ep.dir = USB_DIR_IN;
		usbStatusInEP[i].ep.buff = 0;
		usbStatusOutEP[i].value = 0;
		usbStatusOutEP[i].xferCount = 0;
		usbStatusOutEP[i].ep.value = 0;
		usbStatusOutEP[i].ep.epNum = i;
		usbStatusOutEP[i].ep.dir = USB_DIR_OUT;
		usbStatusOutEP[i].ep.buff = 0;
	}

	/* Prepare for an EP0 Setup packet */
	USB0->epCtrl0 = USB_EP_RX_TYPE_CTRL | USB_EP_RXR | USB_EP_RXE | USB_EP_TX_TYPE_CTRL | USB_EP_TXR | USB_EP_TXE;
	setupTD.nextTD = USB_INVALID_TD;
	setupTD.status = ((USB_EP0_SETUP_LEN << 16) & USB_TD_COUNT_MASK) | USB_TD_IOC;
	setupTD.buffer0 = &usbBDT[0].setupPacket;
	setupTD.buffer1 = nullptr;
	setupTD.buffer2 = nullptr;
	setupTD.buffer3 = nullptr;
	setupTD.buffer4 = nullptr;
	usbBDT[0].activeTD.nextTD = &setupTD;
	USB0->epPrime |= USB_EP0_RX_MASK;

	dataTD.nextTD = USB_INVALID_TD;
	dataTD.status = ((USB_EP0_DATA_LEN << 16) & USB_TD_COUNT_MASK) | USB_TD_IOC;
	dataTD.buffer0 = usbEP0Data;
	dataTD.buffer1 = nullptr;
	dataTD.buffer2 = nullptr;
	dataTD.buffer3 = nullptr;
	dataTD.buffer4 = nullptr;

	/* Finally, idle the peripheral */
	usbState = USB_STATE_DETACHED;
	usbActiveConfig = 0;
}

void usbAttach()
{
	/* It is an error to call this if we do not have VBus or are already attached */
	if (usbState != USB_STATE_DETACHED || false)
		return;

	/* Ensure we are currently detached */
	USB0->usbCmd &= USB_DCMD_STOP_MASK;
	USB0->usbIE = 0;

	// Configure the controller here
	/* Except, ignore reset and idle conditions for the time being */
	USB0->usbIE = USB_INT_UEIE | USB_INT_PCIE | USB_INT_SRIE;

	/* And enable USB interrupts */
	USB0->usbIE |= USB_INT_UIE;

	/* Try to attach to the bus */
	USB0->usbCmd |= USB_DCMD_RUN;

	usbState = USB_STATE_ATTACHED;
}

void usbDetach()
{
	if (usbState == USB_STATE_DETACHED)
		return;

	USB0->usbCmd &= USB_DCMD_STOP_MASK;
	USB0->usbIE = 0;
	usbState = USB_STATE_DETACHED;
}

void usbWakeup()
{
	usbSuspended = false;
	// Wakeup here
	USB0->usbIE &= ~USB_INT_PCIE;
	USB0->usbIF = USB_INT_PCI;
}

void usbSuspend()
{
	USB0->usbIE |= USB_INT_PCIE;
	USB0->usbIF = USB_DINT_SLI;
	// Suspend here
	usbSuspended = true;
}

bool usbHandleStandardRequest(volatile usbBDTEntry_t *BD)
{
	volatile usbSetupPacket_t *packet = &BD->setupPacket;
	if (packet->requestType.type != USB_REQUEST_TYPE_STANDARD)
		return false;

	switch (packet->request)
	{
		case USB_REQUEST_SET_ADDRESS:
			/* Generate a reply that is 0 bytes long to acknowledge */
			usbStatusInEP[0].needsArming = 1;
			/* And check + try setting the address */
			if (packet->value.address.addrL >= 128 ||
				packet->value.address.addrH != 0)
				USB0->deviceAddr = USB_DEVICEADDR_ADV;
			else
				USB0->deviceAddr = (packet->value.address.addrL << 25) | USB_DEVICEADDR_ADV;

			/* If the address payload was not 100% correct, enter the waiting state again */
			if ((USB0->deviceAddr & USB_DEVICEADDR_MASK) == 0)
				usbState = USB_STATE_WAITING;
			else
				usbState = USB_STATE_ADDRESSED;
			return true;
		case USB_REQUEST_GET_DESCRIPTOR:
			/* Transmits the requested descriptor of the requested type to the host */
			//usbRequestGetDescriptor();
			return true;
		case USB_REQUEST_SET_CONFIGURATION:
			/* Reconfigures the device endpoints for the indicated configuration index */
			//usbRequestSetConfiguration();
			usbPacket.buff = 1;
			return true;
		case USB_REQUEST_GET_CONFIGURATION:
			/* Returns the index of the active configuration */
			usbStatusInEP[0].buffSrc = USB_BUFFER_SRC_MEM;
			usbStatusInEP[0].buffer.memPtr = &usbActiveConfig;
			usbStatusInEP[0].xferCount = 1;
			usbStatusInEP[0].needsArming = 1;
			return true;
		case USB_REQUEST_GET_STATUS:
			/* Returns the device status of the requested sub-entity */
			//usbRequestGetStatus();
			return true;
		case USB_REQUEST_CLEAR_FEATURE:
		case USB_REQUEST_SET_FEATURE:
			/* Manipulates a device feature */
			//usbRequestDoFeature();
			return true;
		case USB_REQUEST_GET_INTERFACE:
			/* Set source and types */
			usbStatusInEP[0].buffSrc = USB_BUFFER_SRC_MEM;
			usbStatusInEP[0].xferCount = 1;
			usbStatusInEP[0].needsArming = 1;
			return true;
		case USB_REQUEST_SET_INTERFACE:
			/* Generate a reply that is 0 bytes long to acknowledge */
			usbStatusInEP[0].needsArming = 1;
			/* AltID */
			return true;
		case USB_REQUEST_SET_DESCRIPTOR:
			/* Set descriptor handler */
			return true;
		case USB_REQUEST_SYNC_FRAME:
			return true;
	}
	return false;
}

void usbServiceCtrlEP()
{
	if (usbPacket.epNum != 0)
		return;
	usbStatusTimeout = USB_STATUS_TIMEOUT;
	/*if (USB0->epSetupStat & 0x00000001)
		usbHandleCtrlEPSetup();
	else if (USB0->epComplete & 0x00000001)
		usbHandleCtrlEPOut();
	else if (USB0->epComplete & 0x00010000)
		usbHandleCtrlEPIn();*/
}

void irqUSB()
{
	/*
	 * When Single-Ended 0 condition clears and we are in the freshly attached state,
	 * switch state processing to the "powered" state where we are unconfigured, but
	 * ready and awaiting address assignment and enumeration.
	 * SE0 is held on the bus during the process of the D+ and D-
	 * pair being bought up to operating voltage.
	 */
	if (usbState == USB_STATE_ATTACHED)
	{
		/* Clear interrupt condition */
		USB0->usbIF = ~USB_DINT_URIE;
		/* Enable Idle and Reset interrupts now it makes sense to have them */
		USB0->usbIE |= USB_DINT_URIE | USB_DINT_SLIE;
		usbState = USB_STATE_POWERED;
	}

	/* If we detect activity, ensure we are in an awake state */
	if ((USB0->usbIF & USB0->usbIE) & USB_INT_PCI)
	{
		USB0->usbIF = USB_INT_PCI;
		usbWakeup();
	}

	/* If we are in a suspended state due to inactivity, ignore all further USB interrupt processors */
	if (usbSuspended)
		return;

	/* If we detect the USB reset condition then ready processing getting an address, etc */
	if ((USB0->usbIF & USB0->usbIE) & USB_DINT_URI)
	{
		usbReset();
		usbState = USB_STATE_WAITING;
		USB0->usbIF = USB_DINT_URI;
	}

	/* If we detect an idle condition, suspend activity */
	if ((USB0->usbIF & USB0->usbIE) & USB_DINT_SLI)
	{
		usbSuspend();
		USB0->usbIF = USB_DINT_SLI;
	}

	/* If we detect a start of frame, check the status stage timeout and dispatch as necessary */
	if (USB0->usbIF & USB_INT_SRI)
	{
		//if (USB0->usbIE & USB_INT_SRIE);
		USB0->usbIF = USB_INT_SRI;

		if (usbStatusTimeout != 0)
			--usbStatusTimeout;
//		else
//			usbHandleStatusCtrlEP();
	}

	/* If we are not yet configured, process no further. */
	if (usbState < USB_STATE_WAITING)
		return;

	/* If we detect the SIE has completed IO for a transaction, handle that transaction */
	while ((USB0->epComplete | USB0->epSetupStat) != 0)
	{
		uint32_t epMask = 0x01;
		uint8_t endpointNum = 0;
		/* Locate the next endpoint to process, we always start with the control endpoint */
		while ((USB0->epComplete & ((epMask << 16) | epMask)) == 0 && (USB0->epSetupStat & epMask) == 0)
		{
			epMask <<= 1;
			++endpointNum;
		}
		usbPacket.epNum = endpointNum;
		usbPacket.dir = ((USB0->epComplete | USB0->epSetupStat) & epMask) ? USB_DIR_OUT : USB_DIR_IN;

		if (usbPacket.dir == USB_DIR_OUT)
		{
			usbPacket.buff = usbStatusOutEP[endpointNum].ep.buff;
			usbStatusOutEP[endpointNum].ep.buff ^= 1;
		}
		else
		{
			usbPacket.buff = usbStatusInEP[endpointNum].ep.buff;
			usbStatusInEP[endpointNum].ep.buff ^= 1;
		}

		if (endpointNum == 0)
			usbServiceCtrlEP();
	}
}
