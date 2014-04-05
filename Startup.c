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

/* The startup code for the TivaC */
#include <stdint.h>

#ifndef NULL
#define NULL	(void *)0
#endif

extern int main();
void irqReset();
void irqNMI();
void irqEmptyDef();
void irqHardFault();

extern uint32_t _stack_top;
extern uint32_t _start_text, _end_text;
extern uint32_t _start_data, _end_data;
extern uint32_t _start_bss, _end_bss;

typedef void (*irqFunction)();
irqFunction vectorTable[] __attribute__((section(".nvic_table"))) =
{
	(irqFunction)&_stack_top, /* Startup value for stack pointer */
	irqReset, /* Reset handler */
	irqNMI, /* NMI handler */
	irqHardFault, /* Hard Fault handler */

	/* Configurable priority handlers */
	irqEmptyDef, /* MMU Fault handler */
	irqEmptyDef, /* Bus Fault handler */
	irqEmptyDef, /* Usage Fault */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	irqEmptyDef, /* SV Call */
	irqEmptyDef, /* Debug Monitor */
	NULL, /* Reserved */
	irqEmptyDef, /* Pending SV */
	irqEmptyDef, /* Sys Tick */

	/* Peripheral handlers */
	irqEmptyDef, /* GPIO Port A */
	irqEmptyDef, /* GPIO Port B */
	irqEmptyDef, /* GPIO Port C */
	irqEmptyDef, /* GPIO Port D */
	irqEmptyDef, /* GPIO Port E */
	irqEmptyDef, /* UART 0 */
	irqEmptyDef, /* UART 1 */
	irqEmptyDef, /* SSI 0 */
	irqEmptyDef, /* I2C 0 */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	irqEmptyDef, /* ADC 0 Seq 0 */
	irqEmptyDef, /* ADC 0 Seq 1 */
	irqEmptyDef, /* ADC 0 Seq 2 */
	irqEmptyDef, /* ADC 0 Seq 3 */
	irqEmptyDef, /* WDT 0 and 1 */
	irqEmptyDef, /* Timer 0 A (16/32-bit) */
	irqEmptyDef, /* Timer 0 B (16/32-bit) */
	irqEmptyDef, /* Timer 1 A (16/32-bit) */
	irqEmptyDef, /* Timer 1 B (16/32-bit) */
	irqEmptyDef, /* Timer 2 A (16/32-bit) */
	irqEmptyDef, /* Timer 2 B (16/32-bit) */
	irqEmptyDef, /* Analog Comparator 0 */
	irqEmptyDef, /* Analog Comparator 1 */
	NULL, /* Reserved */
	irqEmptyDef, /* SysCtl */
	irqEmptyDef, /* Flash + EEPROM Ctl */
	irqEmptyDef, /* GPIO Port F */
	NULL, /* Reserved */
	irqEmptyDef, /* UART 2 */
	irqEmptyDef, /* SSI 1 */
	irqEmptyDef, /* Timer 3 A (16/32-bit) */
	irqEmptyDef, /* Timer 3 B (16/32-bit) */
	irqEmptyDef, /* I2C 1 */
	NULL, /* Reserved */
	irqEmptyDef, /* CAN 0 */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	irqEmptyDef, /* Hibernation Module */
	irqEmptyDef, /* USB */
	NULL, /* Reserved */
	irqEmptyDef, /* UDMA SW */
	irqEmptyDef, /* UDMA Error */
	irqEmptyDef, /* ADC 1 Seq 0 */
	irqEmptyDef, /* ADC 1 Seq 1 */
	irqEmptyDef, /* ADC 1 Seq 2 */
	irqEmptyDef, /* ADC 1 Seq 3 */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	irqEmptyDef, /* SSI 2 */
	irqEmptyDef, /* SSI 3 */
	irqEmptyDef, /* UART 3 */
	irqEmptyDef, /* UART 4 */
	irqEmptyDef, /* UART 5 */
	irqEmptyDef, /* UART 6 */
	irqEmptyDef, /* UART 7 */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	irqEmptyDef, /* I2C 2 */
	irqEmptyDef, /* I2C 4 */
	irqEmptyDef, /* Timer 4 A (16/32-bit) */
	irqEmptyDef, /* Timer 4 B (16/32-bit) */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	irqEmptyDef, /* Timer 5 A (16/32-bit) */
	irqEmptyDef, /* Timer 5 B (16/32-bit) */
	irqEmptyDef, /* Timer 0 A (32/64-bit) */
	irqEmptyDef, /* Timer 0 B (32/64-bit) */
	irqEmptyDef, /* Timer 1 A (32/64-bit) */
	irqEmptyDef, /* Timer 1 B (32/64-bit) */
	irqEmptyDef, /* Timer 2 A (32/64-bit) */
	irqEmptyDef, /* Timer 2 B (32/64-bit) */
	irqEmptyDef, /* Timer 3 A (32/64-bit) */
	irqEmptyDef, /* Timer 3 B (32/64-bit) */
	irqEmptyDef, /* Timer 4 A (32/64-bit) */
	irqEmptyDef, /* Timer 4 B (32/64-bit) */
	irqEmptyDef, /* Timer 5 A (32/64-bit) */
	irqEmptyDef, /* Timer 5 B (32/64-bit) */
	irqEmptyDef, /* System Exception */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL, /* Reserved */
	NULL /* Reserved */
};

void irqReset()
{
	uint32_t *src, *dst;

	while (1)
	{
		src = &_end_text;
		dst = &_start_data;
		while (dst < &_end_data)
			*dst++ = *src++;

		dst = &_start_bss;
		while (dst < &_end_bss)
			*dst++ = 0;

		main();
	}
}

void irqNMI()
{
	while (1);
}

void irqHardFault()
{
	while (1);
}

void irqEmptyDef()
{
	while (1);
}

