// SPDX-License-Identifier: BSD-3-Clause
#include "platform.hxx"
#include "led.hxx"
#include "spi.hxx"

void run() noexcept
{
	ledInit();
	spiInit();

	while (true);
}
