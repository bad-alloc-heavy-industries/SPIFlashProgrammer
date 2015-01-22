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
#include <stddef.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>
#ifndef NOCONFIG
#include "config.h"
#endif
#ifndef NOUSB
#include "USBInterface.h"
#endif

#include "SPI.h"

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

#ifndef NOUSB
uint8_t usbData[256];
uint32_t usbDataTotal;
uint32_t usbDataReceived;
#endif

/*
 * 0x20 => Manufacturer ID (Numonyx)
 * 0x71 => Memory Type (SPI Flash)
 * 0x14 => Memory Capacity - 4Mbit
 */
static const uint8_t expectedDID[3] = { 0x20, 0x71, 0x14 };

int datacmp(const uint8_t *a, const uint8_t *b, const size_t n)
{
	size_t i;
	for (i = 0; i < n; i++)
	{
		if (*a != *b)
			return *a - *b;
		a++;
		b++;
	}
	return 0;
}

#ifndef NOUSB
void writeUART(uint8_t data)
{
	while ((UART0_FR_R & UART_FR_TXFF) != 0);
	UART0_DR_R = data;
}

uint8_t readUART()
{
	while ((UART0_FR_R & UART_FR_RXFE) != 0);
	return UART0_DR_R & 0xFF;
}

uint16_t readData()
{
	uint8_t pageLen, i;
	if (readUART() != CMD_PAGE)
		return 0;
	pageLen = readUART();
	i = 0;
	do
	{
		usbData[i] = readUART();
		i++;
	}
	while (i != pageLen);

	if (pageLen == 0)
		return 0x100;
	else
		return pageLen;
}
#endif

bool verifyDID()
{
	uint8_t data[3], i;
	/* Select the device */
	spiChipSelect(true);
	/* Send a short DID read request */
	spiWrite(RDIDS);
	/* For each of the three bytes returned */
	for (i = 0; i < 3; i++)
		/* And having recieved the answer, buffer it */
		data[i] = spiRead();
	/* Deselect the device */
	spiChipSelect(false);
	/* Compare the recieved data to the expected data */
	/* I wanted to use memcmp here, but could not make use of the newlib implemenation */
	return datacmp(data, expectedDID, 3) == 0;
}

void writeEnable()
{
	/* Select the device */
	spiChipSelect(true);
	/* Write enable the device */
	spiWrite(WREN);
	/* Deselect the device - executes write-enable instruction */
	spiChipSelect(false);
	asm("nop");
	/* Select the device again */
	spiChipSelect(true);
	/* Issue Read Status Register instruction */
	spiWrite(RDSR);
	/* And use it's continuous read mode till the write enable completes (bit 1 => 1) */
	while ((spiRead() & 0x02) == 0);
	/* Deselect the device */
	spiChipSelect(false);
}

void unlockDevice()
{
	writeEnable();
	/* Select the device */
	spiChipSelect(true);
	/* Write the Status Register */
	spiWrite(WRSR);
	/* All zeros disables all enabled protections */
	spiWrite(0x00);
	/* Deselect the device */
	spiChipSelect(false);
}

void lockDevice()
{
	writeEnable();
	/* Select the device */
	spiChipSelect(true);
	/* Write the Status Register */
	spiWrite(WRSR);
	/* The three Block Protect bits are bits [4:2] */
	spiWrite(0x1C);
	/* Deselect the device */
	spiChipSelect(false);
}

void eraseDevice(const uint8_t *data)
{
	uint8_t i;
	/* Ensure the device is write enabled */
	writeEnable();
	/* Select the device */
	spiChipSelect(true);
	/* Issue erase instruction */
	spiWrite(BE);
	/* Deselect the device - executes erase */
	spiChipSelect(false);
	for (i = 0; i < 10; i++);
	/* Select the device */
	spiChipSelect(true);
	/* Write the Read Status Register instruction */
	spiWrite(RDSR);
	/* While write is not complete (bit 0 => 1) */
	while ((spiRead() & 0x01) != 0)
	{
#ifndef NOUSB
		if (data == usbData && (UART0_FR_R & UART_FR_RXFE) == 0)
		{
			/* It doesn't matter what the request was.. */
			readUART();
			/* Inform the connected PC */
			writeUART(CMD_ERASE);
			writeUART(RPL_BUSY);
		}
#endif
	}
	/* Deselect the device */
	spiChipSelect(false);
#ifndef NOUSB
	if (data == usbData)
	{
		/* It doesn't matter what the request was.. */
		readUART();
		/* Write complete, so say erase completed! */
		writeUART(CMD_ERASE);
		writeUART(RPL_OK);
	}
#endif
}

void writeData(const uint8_t sector, const uint8_t page, const uint8_t *data, const uint16_t dataLen)
{
	uint16_t i;
	writeEnable();
	/* Select the device */
	spiChipSelect(true);
	/* And issue the Page Program instruction */
	spiWrite(PP);
	/* Sector is (sector >> 4), and sub-sector is (sector & 0x0F) */
	spiWrite(sector);
	/* The page of the sector */
	spiWrite(page);
	/* Must be 0 to select the first byte of the page */
	spiWrite(0);
	/* Send the data */
	for (i = 0; i < dataLen; i++)
		spiWrite(data[i]);
	/* Deselect the device - executes write instruction */
	spiChipSelect(false);
}

bool verifyData(const uint16_t startPage, const uint8_t *data, const size_t dataLen)
{
	uint16_t i;
	bool ok = true;
	/* Select the device */
	spiChipSelect(true);
	spiWrite(READ);
	spiWrite(startPage >> 8);
	spiWrite(startPage & 0xFF);
	spiWrite(0);
	for (i = 0; i < dataLen; i++)
	{
		if (spiRead() != data[i])
		{
			ok = false;
			break;
		}
	}
	/* Deselect the device */
	spiChipSelect(false);
	return ok;
}

void waitWriteComplete()
{
	/* Select the device */
	spiChipSelect(true);
	/* Write the Read Status Register instruction */
	spiWrite(RDSR);
	/* And use it's continuous read mode till the write is complete (bit 0 => 0) */
	while ((spiRead() & 0x01) != 0);
	/* Deselect the device */
	spiChipSelect(false);
}

void transferBitfile(const void *data, const size_t dataLen)
{
	uint16_t addr, pages;
#ifndef NOCONFIG
	const uint8_t *dataPtr = data;
#endif
#ifndef NOUSB
	bool programmed = true;
#endif

	if (!verifyDID())
	{
#ifndef NOUSB
		if (data == usbData)
		{
			writeUART(CMD_ABORT);
			writeUART(RPL_FAIL);
		}
#endif
		return;
	}
#ifndef NOUSB
	else if (data == usbData)
	{
		writeUART(CMD_START);
		writeUART(RPL_OK);
	}
#endif

	pages = (dataLen >> 8) + ((dataLen & 0xFF) != 0 ? 1 : 0);
	unlockDevice();
	waitWriteComplete();
	eraseDevice(data);
	for (addr = 0; addr < pages; addr++)
	{
#ifndef NOUSB
		size_t pageLen;
		if (data == usbData)
		{
			pageLen = readData();
			/* If we failed to receive the page, immediately indicate failure */
			if (pageLen == 0)
			{
				writeUART(CMD_PAGE);
				writeUART(RPL_FAIL);
				programmed = false;
				break;
			}
			writeData(addr >> 8, addr & 0xFF, data, pageLen);
		}
#endif
#ifndef NOCONFIG
		if (data == config)
		{
			if ((addr + 1) < (dataLen >> 8))
				writeData(addr >> 8, addr & 0xFF, dataPtr, 256);
			else
				writeData(addr >> 8, addr & 0xFF, dataPtr, dataLen & 0xFF);
			dataPtr += 256;
		}
#endif
		waitWriteComplete();
#ifndef NOUSB
		if (data == usbData)
		{
			if (!verifyData(addr, data, pageLen))
			{
				writeUART(CMD_ABORT);
				writeUART(RPL_FAIL);
				programmed = false;
				break;
			}
			else
			{
				writeUART(CMD_PAGE);
				writeUART(RPL_OK);
				usbDataReceived += pageLen;
			}
		}
#endif
	}
	lockDevice();
	waitWriteComplete();
#ifndef NOCONFIG
	if (data == config)
	{
		if (verifyData(0, data, dataLen))
			GPIO_PORTF_DATA_BITS_R[0x0E] = 0x08;
	}
#endif
#ifndef NOUSB
	if (data == usbData)
	{
		uint8_t cmd = readUART();
		if (cmd == CMD_STOP)
		{
			uint8_t i;
			uint32_t diff = usbDataTotal - usbDataReceived;
			for (i = 0; i < 4; i++)
			{
				writeUART((diff >> 24) & 0xFF);
				diff <<= 8;
			}
			writeUART(CMD_STOP);
			if (programmed)
				writeUART(RPL_OK);
			else
				writeUART(RPL_FAIL);
		}
		else
		{
			writeUART(CMD_INVALID);
			writeUART(RPL_FAIL);
		}
	}
#endif
}

int main()
{
	/* Enable ports A and F */
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0 | SYSCTL_RCGCGPIO_R5;
	/* Enable SSI0 */
	SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;
	/* Enable Timer 0 */
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;
#ifndef NOUSB
	/* Enable UART0 */
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
#endif
	/* Wait for the ports to come online */
	while ((SYSCTL_PRGPIO_R & (SYSCTL_PRGPIO_R0 | SYSCTL_PRGPIO_R5)) != (SYSCTL_PRGPIO_R0 | SYSCTL_PRGPIO_R5));
	/* Port F is protected, so enable changing it to digital GPIO */
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= 0x01;
	GPIO_PORTF_LOCK_R = 0;
	/* Set the ports to digital mode */
	GPIO_PORTA_DEN_R |= 0xDF;
	GPIO_PORTF_DEN_R |= 0x1F;
	/* Wait for SSI0 to come online */
	while ((SYSCTL_PRSSI_R & SYSCTL_PRSSI_R0) != SYSCTL_PRSSI_R0);
	/* Wait for Timer 0 to come online */
	while ((SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R0) != SYSCTL_PRTIMER_R0);
#ifndef NOUSB
	/* Wait for UART 0 to come online */
	while ((SYSCTL_PRUART_R & SYSCTL_PRUART_R0) != SYSCTL_PRUART_R0);
#endif

	/* Configure PA7 as a digital O/D output */
	GPIO_PORTA_ODR_R |= 0x80;
	GPIO_PORTA_DIR_R |= 0x80;
	GPIO_PORTA_DATA_BITS_R[0x80] = 0x80;

#ifndef NOUSB
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
#endif

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

	/* Configure the Timer 0 module for submodule A operation */
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
	TIMER0_TAMR_R = TIMER_TAMR_TAMR_1_SHOT | TIMER_TAMR_TACDIR | TIMER_TAMR_TAMIE;
	/* Set the timeout for 500ms */
	TIMER0_TAMATCHR_R = 8000000;
	TIMER0_ICR_R = TIMER_ICR_TAMCINT;

	while (1)
	{
#ifndef NOCONFIG
		/* If either button has been pressed.. */
		if (GPIO_PORTF_DATA_BITS_R[0x11] != 0x11)
		{
			/* Stop the timer if it's already running */
			TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
			/* Set the LED to red for busy/processing */
			GPIO_PORTF_DATA_BITS_R[0x0E] = 0x02;
			/* Attempt the transfer */
			transferBitfile(config, configLen);
			/* Reset the timer and set it running */
			TIMER0_TAV_R = 0;
			TIMER0_CTL_R |= TIMER_CTL_TAEN;
		}
#endif
#ifndef NOUSB
		/* If the UART has recieved a byte.. */
		if ((UART0_FR_R & UART_FR_RXFE) == 0)
		{
			if ((UART0_DR_R & 0xFF) == CMD_START)
			{
				uint8_t i;
				/* Stop the timer if it's already running */
				TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
				/* Set the LED to red for busy/processing */
				GPIO_PORTF_DATA_BITS_R[0x0E] = 0x02;
				/* Enable the interface */
				GPIO_PORTA_ODR_R &= ~0x08;
				GPIO_PORTA_DATA_BITS_R[0x88] = 0x08;
				usbDataTotal = 0;
				usbDataReceived = 0;
				for (i = 0; i < 4; i++)
				{
					usbDataTotal <<= 8;
					usbDataTotal |= readUART();
				}
				GPIO_PORTA_ODR_R |= 0x08;
				transferBitfile(usbData, usbDataTotal);
				/* Disable the interface */
				GPIO_PORTA_DATA_BITS_R[0xAC] = 0xAC;
				/* Reset the timer and set it running */
				TIMER0_TAV_R = 0;
				TIMER0_CTL_R |= TIMER_CTL_TAEN;
			}
			else
			{
				writeUART(CMD_INVALID);
				writeUART(RPL_FAIL);
			}
		}
#endif
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

