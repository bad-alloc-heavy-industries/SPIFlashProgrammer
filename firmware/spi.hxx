// SPDX-License-Identifier: BSD-3-Clause
#ifndef SPI__HXX
#define SPI__HXX

#include <cstdint>

extern void spiInit() noexcept;
extern uint8_t spiIntRead() noexcept;
extern void spiIntWrite(uint8_t value) noexcept;
extern uint8_t spiExtRead() noexcept;
extern void spiExtWrite(uint8_t value) noexcept;

#endif /*SPI__HXX*/
