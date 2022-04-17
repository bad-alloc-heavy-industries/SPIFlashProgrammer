// SPDX-License-Identifier: BSD-3-Clause
#include "platform.hxx"
#include "osc.hxx"
#include "led.hxx"
#include "spi.hxx"
#include "timer.hxx"
#include <usb/core.hxx>
#include <usb/drivers/dfu.hxx>
#include "usb/flashProto.hxx"

using usb::types::endpointDir_t;

void run() noexcept
{
	ledInit();
	oscInit();
	timerInit();
	spiInit();
	usb::core::init();
	usb::flashProto::registerHandlers(1, 1, 0, 1);
	usb::dfu::registerHandlers({}, 1, 1);
	usb::core::attach();

	while (true)
		__asm__("wfi");
}

void irqUSB() noexcept { usb::core::handleIRQ(); }

namespace usb::dfu
{
	// The linker script pins this to 0x20001000
	[[gnu::section(".bootMagic")]] static volatile uint16_t bootMagic;
	constexpr static uint16_t bootMagicDFU{0xBADB};

	void reboot() noexcept
	{
		bootMagic = bootMagicDFU;
		scb.apint = vals::scb::apintKey | vals::scb::apintSystemResetRequest;
		while (true)
			continue;
	}

	bool flashBusy() noexcept { return false; }
	void erase(const std::uintptr_t) noexcept { }
	void write(const std::uintptr_t, const std::size_t, const uint8_t *const) noexcept { }
}
