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
#include "UART.h"

void uartInit()
{
	/* Enable UART0 */
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
	/* Set the port to digital mode */
	GPIO_PORTA_DEN_R |= 0x03;
	/* Wait for UART 0 to come online */
	while ((SYSCTL_PRUART_R & SYSCTL_PRUART_R0) != SYSCTL_PRUART_R0);
	/* Configure the UART pins as alternative function and enable their use by the UART module */
	GPIO_PORTA_AFSEL_R |= 0x03;
	GPIO_PORTA_PCTL_R |= GPIO_PCTL_PA1_U0TX | GPIO_PCTL_PA0_U0RX;
	GPIO_PORTA_DR2R_R |= 0x03;
	GPIO_PORTA_PUR_R |= 0x02;
	/*
	 * Set the UART up for 8-bit, 115200 baud operation with one stop bit + FIFOs
	 * BRD = 16MHz / (16 * 115200) = 8.68055*
	 * IBRD = 8
	 * FBRD = 0.68055* * 64 = 43.555 ~= 44
	 */
	UART0_IBRD_R = 8;
	UART0_FBRD_R = 44;
	UART0_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN;
	UART0_CTL_R = UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN;
}

void uartWrite(uint8_t data)
{
	while ((UART0_FR_R & UART_FR_TXFF) != 0);
	UART0_DR_R = data;
}

uint8_t uartRead()
{
	while ((UART0_FR_R & UART_FR_RXFE) != 0);
	return UART0_DR_R & 0xFF;
}

bool uartHaveData()
{
	return (UART0_FR_R & UART_FR_RXFE) == 0;
}

uint8_t uartPeak()
{
	return UART0_DR_R & 0xFF;
}

