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
#include "GPIO.h"

void gpioInit()
{
	/* Enable ports A and F */
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0 | SYSCTL_RCGCGPIO_R5;
	/* Enable Timer 0 */
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;

	/* Wait for the ports to come online */
	while ((SYSCTL_PRGPIO_R & (SYSCTL_PRGPIO_R0 | SYSCTL_PRGPIO_R5)) != (SYSCTL_PRGPIO_R0 | SYSCTL_PRGPIO_R5));
	/* Port F is protected, so enable changing it to digital GPIO */
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= 0x01;
	GPIO_PORTF_LOCK_R = 0;
	/* Set the ports to digital mode */
	GPIO_PORTA_DEN_R |= 0xC0;
	GPIO_PORTF_DEN_R |= 0x1F;
	/* Wait for Timer 0 to come online */
	while ((SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R0) != SYSCTL_PRTIMER_R0);

	/* Configure PA7 as a digital O/D output */
	GPIO_PORTA_ODR_R |= 0x80;
	GPIO_PORTA_DIR_R |= 0x80;
	GPIO_PORTA_DATA_BITS_R[0x80] = 0x80;

	/* Ensure the LED and switch pins on Port F are straight GPIO */
	GPIO_PORTF_AFSEL_R &= ~0x1F;
	/* Enable the LED pin outputs */
	GPIO_PORTF_DIR_R = 0x0E;
#ifndef NOCONFIG
	/* Set pullups on the 2 buttons */
	GPIO_PORTF_DR2R_R |= 0x11;
	GPIO_PORTF_PUR_R |= 0x11;
#endif
	/* Set the blue LED on */
	GPIO_PORTF_DATA_BITS_R[0x0E] = 0x04;

	/* Configure the Timer 0 module for submodule A operation */
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
	TIMER0_TAMR_R = TIMER_TAMR_TAMR_1_SHOT | TIMER_TAMR_TACDIR | TIMER_TAMR_TAMIE;
	/* Set the timeout for 500ms */
	TIMER0_TAMATCHR_R = 8000000;
	TIMER0_ICR_R = TIMER_ICR_TAMCINT;
}

void gpioStopTimer()
{
	/* Stop the timer if it's already running */
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
}

void gpioStartTimer()
{
	/* Reset the timer and set it running */
	TIMER0_TAV_R = 0;
	TIMER0_CTL_R |= TIMER_CTL_TAEN;
}

void gpioCheckIdle()
{
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

