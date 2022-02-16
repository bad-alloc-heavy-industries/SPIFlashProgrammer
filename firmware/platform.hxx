// SPDX-License-Identifier: BSD-3-Clause
#ifndef PLATFORM_HXX
#define PLATFORM_HXX

void run() noexcept;
[[gnu::isr]] void irqUSB() noexcept;

#endif /*PLATFORM_HXX*/
