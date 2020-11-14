// SPDX-License-Identifier: BSD-3-Clause
#include "platform.hxx"
#include "osc.hxx"
#include "led.hxx"
#include "spi.hxx"

void run() noexcept
{
	ledInit();
	oscInit();
	spiInit();

	while (true);
}
