// SPDX-License-Identifier: BSD-3-Clause
#ifndef PLATFORM___HXX
#define PLATFORM___HXX

extern void run() noexcept;
[[gnu::isr]] extern void irqUSB() noexcept;

#endif /*PLATFORM___HXX*/
