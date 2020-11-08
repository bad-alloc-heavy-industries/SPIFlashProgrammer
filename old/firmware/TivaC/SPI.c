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

#include <tm4c123gh6pm.h>
#include "SPI.h"

void spiInit()
{
	/* Enable SSI0 */
	SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;
	/* Set the port to digital mode */
	GPIO_PORTA_DEN_R |= 0x5C;
	/* Wait for SSI0 to come online */
	while ((SYSCTL_PRSSI_R & SYSCTL_PRSSI_R0) != SYSCTL_PRSSI_R0);
	/* Configure the SSI (SPI) pins as alternative function and enable their use by the SPI module */
	GPIO_PORTA_AFSEL_R |= 0x34;
	GPIO_PORTA_PCTL_R |= GPIO_PCTL_PA5_SSI0TX | GPIO_PCTL_PA4_SSI0RX | GPIO_PCTL_PA2_SSI0CLK;
	SSI0_CR1_R = 0;
	GPIO_PORTA_DR2R_R |= 0x3C;
	/* Pull all the pins high with a 2mA drive strength */
	GPIO_PORTA_PUR_R |= 0x3C;
	/* Set the output pins for open-collector function */
	GPIO_PORTA_ODR_R |= 0x28;
	/* Set the TX/!CS/CLK pins to outputs */
	GPIO_PORTA_DIR_R |= 0x28;
	/* And send !CS high */
	GPIO_PORTA_DATA_BITS_R[0x08] = 0x08;
	/* Set Freescale SPI, SPO = 1, SPH = 1 */
	SSI0_CR0_R = SSI_CR0_SPO | SSI_CR0_SPH | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_8;
	/* We have a 16 MHz clock, and we interface to the SPI Flash chip at 8MHz */
	SSI0_CC_R = 0;
	/* Scale the clock by 8 to make it 2MHz */
	SSI0_CPSR_R = 8;
	/* Enable the interface */
	SSI0_CR1_R = SSI_CR1_SSE;
}

void spiWrite(uint8_t data)
{
	uint8_t temp __attribute__((unused));
	SSI0_DR_R = data;
	while ((SSI0_SR_R & 0x01) != 1);
	temp = SSI0_DR_R & 0xFF;
	while ((SSI0_SR_R & 0x04) != 4);
	temp = SSI0_DR_R & 0xFF;
}

uint8_t spiRead()
{
	SSI0_DR_R = 0;
	while ((SSI0_SR_R & 0x04) != 4);
	return SSI0_DR_R & 0xFF;
}

void spiChipSelect(bool select)
{
	if (select)
		GPIO_PORTA_DATA_BITS_R[0x08] = 0;
	else
		GPIO_PORTA_DATA_BITS_R[0x08] = 8;
}

