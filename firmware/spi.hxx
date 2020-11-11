// SPDX-License-Identifier: BSD-3-Clause
#ifndef SPI__HXX
#define SPI__HXX

#include <cstdint>

enum class spiChip_t
{
	local1,
	local2,
	target,
	none
};

extern void spiInit() noexcept;
extern void spiSelect(spiChip_t chip) noexcept;
extern uint8_t spiIntRead() noexcept;
extern void spiIntWrite(uint8_t value) noexcept;
extern uint8_t spiExtRead() noexcept;
extern void spiExtWrite(uint8_t value) noexcept;

#endif /*SPI__HXX*/
