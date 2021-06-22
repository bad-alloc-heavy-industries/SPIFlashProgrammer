// SPDX-License-Identifier: BSD-3-Clause
#include "platform.hxx"
#include "osc.hxx"
#include "led.hxx"
#include "spi.hxx"
#include <usb/core.hxx>
#include "usb/flashProto.hxx"

using usb::types::endpointDir_t;

void run() noexcept
{
	ledInit();
	oscInit();
	spiInit();
	usb::flashProto::registerHandlers();
	usb::core::init();

	while (true)
		__asm__("wfi");
}

void irqUSB() noexcept { usb::core::handleIRQ(); }
