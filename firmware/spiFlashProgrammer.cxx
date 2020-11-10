// SPDX-License-Identifier: BSD-3-Clause
#include "platform.hxx"
#include "led.hxx"

void run() noexcept
{
	ledInit();

	while (true);
}
