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

/* The startup code for the LPC4370's Cortex-M4 */
#include <stdint.h>

#ifndef NULL
#define NULL	(void *)0
#endif

extern int main();
void irqReset();
void irqNMI();
void irqEmptyDef();
void irqHardFault() __attribute__((naked));

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
	irqEmptyDef, /* DAC */
	irqEmptyDef, /* Cortex-M4 (Application) */
	irqEmptyDef, /* DMA */
	NULL, /* Reserved */
	irqEmptyDef, /* Flash A, B and EEPROM */
	irqEmptyDef, /* Ethernet */
	irqEmptyDef, /* SDIO */
	irqEmptyDef, /* LCD */
	irqEmptyDef, /* USB 0 */
	irqEmptyDef, /* USB 1 */
	irqEmptyDef, /* SCT */
	irqEmptyDef, /* RITimer */
	irqEmptyDef, /* Timer 0 */
	irqEmptyDef, /* Timer 1 */
	irqEmptyDef, /* Timer 2 */
	irqEmptyDef, /* Timer 3 */
	irqEmptyDef, /* Motor Ctrl PWM */
	irqEmptyDef, /* ADC 0 */
	irqEmptyDef, /* I2C 0 */
	irqEmptyDef, /* I2C 1 */
	irqEmptyDef, /* SPI */
	irqEmptyDef, /* ADC 1 */
	irqEmptyDef, /* SSP 0 */
	irqEmptyDef, /* SSP 1 */
	irqEmptyDef, /* USART 0 */
	irqEmptyDef, /* UART 1 */
	irqEmptyDef, /* USART 2 */
	irqEmptyDef, /* USART 3 */
	irqEmptyDef, /* I2S 0 */
	irqEmptyDef, /* I2S 1 */
	irqEmptyDef, /* SPIFI */
	irqEmptyDef, /* SGPIO */
	irqEmptyDef, /* GPIO Int 0 */
	irqEmptyDef, /* GPIO Int 1 */
	irqEmptyDef, /* GPIO Int 2 */
	irqEmptyDef, /* GPIO Int 3 */
	irqEmptyDef, /* GPIO Int 4 */
	irqEmptyDef, /* GPIO Int 5 */
	irqEmptyDef, /* GPIO Int 6 */
	irqEmptyDef, /* GPIO Int 7 */
	irqEmptyDef, /* GPIO Glbl 0 */
	irqEmptyDef, /* GPIO Glbl 1 */
	irqEmptyDef, /* Event Router */
	irqEmptyDef, /* CAN 1 */
	NULL, /* Reserved */
	irqEmptyDef, /* ADCHS */
	irqEmptyDef, /* Alarm Timer */
	irqEmptyDef, /* RTC */
	NULL, /* Reserved */
	irqEmptyDef, /* WDT */
	irqEmptyDef, /* Cortex-M0 (Subsystem) */
	irqEmptyDef, /* CAN 0 */
	irqEmptyDef /* QEI */
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
	/* Get some information about the fault for the debugger.. */
	__asm__("	movs	r0, #4\n"
		"	movs	r1, lr\n"
		"	tst		r0, r1\n"
		"	beq		_MSP\n"
		"	mrs		r0, psp\n"
		"	b		_HALT\n"
		"_MSP:\n"
		"	mrs		r0, msp\n"
		"_HALT:\n"
		"	ldr		r1, [r0, 0x00]\n" /* r0 */
		"	ldr		r2, [r0, 0x04]\n" /* r1 */
		"	ldr		r3, [r0, 0x08]\n" /* r2 */
		"	ldr		r4, [r0, 0x0C]\n" /* r3 */
		"	ldr		r5, [r0, 0x10]\n" /* r12 */
		"	ldr		r6, [r0, 0x14]\n" /* lr */
		"	ldr		r7, [r0, 0x18]\n" /* pc */
		"	ldr		r8, [r0, 0x1C]\n" /* xpsr */
		"	bkpt	#0\n");
	/* The lowest 8 bits of r8 (xpsr) contain which handler triggered this, if there is a signal handler frame before this. */

	while (1);
}

void irqEmptyDef()
{
	while (1);
}

