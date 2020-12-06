// SPDX-License-Identifier: BSD-3-Clause
#include "platform.hxx"
#include "osc.hxx"
#include "led.hxx"
#include "spi.hxx"
#include "usb.hxx"

void run() noexcept
{
	ledInit();
	oscInit();
	spiInit();
	usbInit();

	while (true)
		continue;
}
