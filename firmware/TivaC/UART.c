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

