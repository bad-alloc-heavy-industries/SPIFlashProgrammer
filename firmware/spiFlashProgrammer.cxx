// SPDX-License-Identifier: BSD-3-Clause
#include "platform.hxx"
#include "osc.hxx"
#include "led.hxx"
#include "spi.hxx"
#include <usb/core.hxx>
#include "usb/flashProto.hxx"

void run() noexcept
{
	ledInit();
	oscInit();
	spiInit();
	usb::core::init();
	usb::core::registerHandler(1, 1, usb::flashProto::flashProtoHandler);

	while (true)
		__asm__("wfi");
}

void irqUSB() noexcept { usb::core::handleIRQ(); }
