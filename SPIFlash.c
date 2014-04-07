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
#include <stdbool.h>
#include <tm4c123gh6pm.h>
#include "config.h"

#define WREN	0x06
#define WRDI	0x04
#define RDIDL	0x9F
#define RDIDS	0x9E
#define RDSR	0x05
#define WRSR	0x01
#define WRLR	0xE5
#define RDLR	0xE8
#define READ	0x03
#define PP		0x02
#define SSE		0x20
#define SE		0xD8
#define BE		0xC7

/*
 * 0x20 => Manufacturer ID (Numonyx)
 * 0x71 => Memory Type (SPI Flash)
 * 0x14 => Memory Capacity - 4Mbit
 */
static const char expectedDID[3] = { 0x20, 0x71, 0x14 };

bool verifyDID()
{
	char temp, data[3], i;
	GPIO_PORTA_DATA_BITS_R[0x08] = 0;
	SSI0_DR_R = RDIDS;
	while ((SSI0_SR_R & 0x01) != 1);
	temp = SSI0_DR_R & 0xFF;
	for (i = 0; i < 3; i++)
	{
		SSI0_DR_R = 0;
		while ((SSI0_SR_R & 0x04) != 4);
		data[i] = SSI0_DR_R & 0xFF;
	}
	GPIO_PORTA_DATA_BITS_R[0x08] = 8;
	return true;
}

void transferBitfile()
{
	if (!verifyDID())
		return;
	GPIO_PORTF_DATA_BITS_R[0x0E] = 0x08;
}

int main()
{
	/* Enable ports A and F */
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0 | SYSCTL_RCGCGPIO_R5;
	/* Enable SSI0 */
	SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;
	/* Enable Timer 0 */
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;
	/* Wait for the ports to come online */
	while ((SYSCTL_PRGPIO_R & (SYSCTL_PRGPIO_R0 | SYSCTL_PRGPIO_R5)) != (SYSCTL_PRGPIO_R0 | SYSCTL_PRGPIO_R5));
	/* Port F is protected, so enable changing it to digital GPIO */
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= 0x01;
	GPIO_PORTF_LOCK_R = 0;
	/* Set the ports to digital mode */
	GPIO_PORTA_DEN_R |= 0x3C;
	GPIO_PORTF_DEN_R |= 0x1F;
	/* Wait for SSI0 to come online */
	while ((SYSCTL_PRSSI_R & SYSCTL_PRSSI_R0) != SYSCTL_PRSSI_R0);
	/* Wait for Timer 1 to come online */
	while ((SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R0) != SYSCTL_PRTIMER_R0);

	/* Ensure the LED and switch pins on Port F are straight GPIO */
	GPIO_PORTF_AFSEL_R &= ~0x1F;
	/* Enable the LED pin outputs */
	GPIO_PORTF_DIR_R = 0x0E;
	/* Set pullups on the 2 buttons */
	GPIO_PORTF_DR2R_R |= 0x11;
	GPIO_PORTF_PUR_R |= 0x11;
	/* Set the blue LED on */
	GPIO_PORTF_DATA_BITS_R[0x0E] = 0x04;

	/* Configure the SSI (SPI) pins as alternative function and enable their use by the SPI module */
	GPIO_PORTA_AFSEL_R = 0x34;
	GPIO_PORTA_PCTL_R |= GPIO_PCTL_PA5_SSI0TX | GPIO_PCTL_PA4_SSI0RX |
		/*GPIO_PCTL_PA3_SSI0FSS | */GPIO_PCTL_PA2_SSI0CLK;
	SSI0_CR1_R = 0;
	GPIO_PORTA_DR2R_R |= 0x3C;
	/* Pull the !CS pin high with a 2mA drive strength */
	GPIO_PORTA_PUR_R |= 0x08;
	/* Pull the Rx, Tx and Clk pins low with a 2mA drive strength */
	GPIO_PORTA_PDR_R |= 0x34;
	/* Set Freescale SPI, SPO = 1, SPH = 1 */
	SSI0_CR0_R = SSI_CR0_SPO | SSI_CR0_SPH | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_8;
	/* We have a 16 MHz clock, and we interface to the SPI Flash chip at 8MHz */
	SSI0_CC_R = 0;
	/* Scale the clock by 2 to make it 8MHz */
	SSI0_CPSR_R = 2;
	/* Enable the interface */
	SSI0_CR1_R = SSI_CR1_SSE;

	/* Configure the Timer 0 module for submodule A operation */
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
	TIMER0_TAMR_R = TIMER_TAMR_TAMR_1_SHOT | TIMER_TAMR_TACDIR | TIMER_TAMR_TAMIE;
	/* Set the timeout for 500us */
	TIMER0_TAMATCHR_R = 8000000;
	TIMER0_ICR_R = TIMER_ICR_TAMCINT;

	while (1)
	{
		/* If either button has been pressed.. */
		if (GPIO_PORTF_DATA_BITS_R[0x11] != 0x11)
		{
			/* Stop the timer if it's already running */
			TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
			/* Set the LED to red for busy/processing */
			GPIO_PORTF_DATA_BITS_R[0x0E] = 0x02;
			/* Attempt the transfer */
			transferBitfile();
			/* Reset the timer and set it running */
			TIMER0_TAV_R = 0;
			TIMER0_CTL_R |= TIMER_CTL_TAEN;
		}
		/* If the timer has triggered the Match event */
		if ((TIMER0_RIS_R & TIMER_RIS_TAMRIS) != 0)
		{
			/* Reset the LED back to blue for idle */
			GPIO_PORTF_DATA_BITS_R[0x0E] = 0x04;
			/* And stop the timer while resetting the interrupt flag for match */
			TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
			TIMER0_ICR_R = TIMER_ICR_TAMCINT;
		}
	}

	return 0;
}

