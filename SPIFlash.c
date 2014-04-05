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

#include <stdint.h>
#include <tm4c123gh6pm.h>

int main()
{
	/* Enable ports A and F */
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0 | SYSCTL_RCGCGPIO_R5;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA | SYSCTL_RCGC2_GPIOF;
	/* Enable SSI0 */
	SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_SSI0;
	/* Wait for the ports to come online */
	while ((SYSCTL_PRGPIO_R & (SYSCTL_PRGPIO_R0 | SYSCTL_PRGPIO_R5)) != (SYSCTL_PRGPIO_R0 | SYSCTL_PRGPIO_R5));
	/* Port F is protected, so enable changing it to digital GPIO */
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= 0x01;
	GPIO_PORTF_LOCK_R = 0;
	/* Set them to digital mode */
	GPIO_PORTA_DEN_R |= 0x3C;
	/* Wait for SSI0 to come online */
	while ((SYSCTL_PRSSI_R & SYSCTL_PRSSI_R0) != SYSCTL_PRSSI_R0);

	GPIO_PORTF_AFSEL_R &= ~0x1F;
	/* Enable the LED pin outputs */
	GPIO_PORTF_DIR_R = 0x0E;
	/* Set pullups on the 2 buttons */
	GPIO_PORTF_DR2R_R |= 0x11;
	GPIO_PORTF_PUR_R |= 0x11;
	/* Set the blue LED on */
	GPIO_PORTF_DATA_BITS_R[0x0E] = 0x04;

	//GPIO_PORTA_AFSEL_R;

	GPIO_PORTA_PCTL_R |= GPIO_PCTL_PA5_SSI0TX | GPIO_PCTL_PA4_SSI0RX |
		GPIO_PCTL_PA3_SSI0FSS | GPIO_PCTL_PA2_SSI0CLK;
	GPIO_PORTA_DEN_R;
	/* Set Freescale SPI, SPO = 0, SPH = 1 */
	SSI0_CR0_R = SSI_CR0_SPH | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_8;
	SSI0_CR1_R = SSI_CR1_SSE;
	SSI0_SR_R;
	SSI0_CC_R;
	SSI0_CPSR_R;

	while (1)
	{
		if (GPIO_PORTF_DATA_BITS_R[0x01] == 0)
			GPIO_PORTF_DATA_BITS_R[0x0E] = 0x02;
		else if (GPIO_PORTF_DATA_BITS_R[0x10] == 0)
			GPIO_PORTF_DATA_BITS_R[0x0E] = 0x08;
	}

	return 0;
}

