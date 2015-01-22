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
}

void spiWrite(uint8_t data)
{
	uint8_t temp;
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

