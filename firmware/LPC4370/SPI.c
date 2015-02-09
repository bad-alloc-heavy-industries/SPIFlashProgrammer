/*
 * This file is part of SPI Flash Programmer (SPIFP)
 * Copyright © 2014 Rachel Mant (dx-mon@users.sourceforge.net)
 *
 * SPIFP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SPIFP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <LPC4370.h>
#include "SPI.h"

void spiInit()
{
	/* Initialise SPIFI (P3_{3..8}) to access SPI Flash */
	SPI->CR = 0;
	/* Set Freescale SPI, SPO = 1, SPH = 1 */
	SPI->CR = SPI_CR_SPO | SPI_CR_SPH | SPI_CR_MASTER | SPI_CR_DSS_EN | SPI_CR_DSS_8;
	/* Scale the clock by 8 (the datasheet minimum) to make it 4MHz */
	SPI->CCR = 8;
	/* Enable the interface */
	/*SSI0_CR1_R = SSI_CR1_SSE;*/
}

void spiWrite(uint8_t data)
{
	uint8_t temp __attribute__((unused));
	SPI->DR = data;
	while ((SPI->SR & SPI_SR_SPIF) != 0);
	temp = SPI->DR & 0xFF;
/*	while ((SPI->*/
}

uint8_t spiRead()
{
	SPI->DR = 0;
	while ((SPI->SR & SPI_SR_SPIF) != 0);
	return SPI->DR & 0xFF;
}

void spiChipSelect(bool select)
{
}

