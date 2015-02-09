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

/*
 * Func1 for pins 3, 6, 7, 8, uses block SPI
 * Func3 for pins 3, 4, 5, 6, 7, 8 uses block SPIFI
 */

void spiInit()
{
	/*
	 * Set the port to digital mode, configure for open-collector based IO and switch into SPI mode
	 * This sets the TX/!CS/CLK pins to outputs, RX as input
	 */
	SCU->SFS_Port3[3] = SCU_SFS_MODE_1;
	SCU->SFS_Port3[6] = SCU_SFS_MODE_1 | SCU_SFS_DPU | SCU_SFS_EIB;
	SCU->SFS_Port3[7] = SCU_SFS_MODE_1;
	SCU->SFS_Port3[8] = SCU_SFS_MODE_0; /* !CS needs to be under our control so do not send it into the SPI peripheral */
	SPI->CR = 0;

	/* Set !CS as GPIO output */
	GPIO_PORT3_DIR |= 0x00000100;
	/* And send !CS high */
	GPIO_PORT3_SET = 0x00000100;

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
	if (select)
		GPIO_PORT3_CLR = 0x00000100;
	else
		GPIO_PORT3_SET = 0x00000100;
}

