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
volatile bool usbSuspended;
volatile usbCtrlState_t usbCtrlState;
volatile uint8_t usbDeferalFlags;
volatile usbStallState_t usbStallState;

volatile uint8_t usbActiveConfig, usbStatusTimeout;
volatile bool usbStageLock1 = false, usbStageLock2 = false;

volatile usbBDTEntry_t usbBDT[USB_BDT_ENTRIES] USB_BTD_ADDR;

void usbInit()
{
	USB0->usbMode = USB_MODE_DEVICE | USB_MODE_SLOD;
	USB0->usbCmd = USB_DCMD_RST;
	USB0->listAddr = (uint32_t)&usbBDT;
	usbReset();
	usbCtrlState = USB_CTRL_STATE_WAIT;
	usbStallState = USB_STALL_STATE_STALL;
	usbDeferalFlags = 0;
	usbStatusTimeout = 0;
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
	USB0->epFlush = USB_EP_ALL_MASK;
	USB0->epNAK = USB_EP_ALL_MASK;
	USB0->epNAKEnable &= ~USB_EP_ALL_MASK;
	USB0->deviceAddr = 0;

	/* Reset status flags */
	usbStageLock1 = false;
	usbStageLock2 = false;
	usbSuspended = false;

	/* Prepare for an EP0 Setup packet */
	USB0->epCtrl0 = USB_EP_RX_TYPE_CTRL | USB_EP_RXR | USB_EP_RXE | USB_EP_TX_TYPE_CTRL | USB_EP_TXR | USB_EP_TXE;
	USB0->epPrime |= USB_EP0_RX_MASK;

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

void irqUSB()
{
	if (usbState == USB_STATE_ATTACHED)
	{
		/* Clear interrupt condition */
		USB0->usbIF = ~USB_DINT_URIE;
		/* Enable Idle and Reset interrupts now it makes sense to have them */
		USB0->usbIE |= USB_DINT_URIE | USB_DINT_SLIE;
		usbState = USB_STATE_POWERED;
	}

	/* If we are in a suspended state due to inactivity, ignore all further USB interrupt processors */
	if (usbSuspended)
		return;

	if (USB0->usbIF & USB_DINT_URI)
	{
		usbReset();
		usbState = USB_STATE_WAITING;
		USB0->usbIF = USB_DINT_URI;
	}

	if (USB0->usbIF & USB_INT_SRI)
	{
		//if (USB0->usbIE & USB_INT_SRI);
		USB0->usbIF = USB_INT_SRI;

		if (usbStatusTimeout != 0)
			--usbStatusTimeout;
//		else
//			usbHandleStatusCtrlEP();
	}

	/* If we are not yet configured, process no further. */
	if (usbState < USB_STATE_WAITING)
		return;
}
