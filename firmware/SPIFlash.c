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
#ifndef NOCONFIG
#include "config.h"
#endif
#ifndef NOUSB
#include "USBInterface.h"
#include "UART.h"
#endif
#include "SPI.h"
#include "GPIO.h"

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
uint16_t readData()
{
	uint8_t pageLen, i;
	if (uartRead() != CMD_PAGE)
		return 0;
	pageLen = uartRead();
	i = 0;
	do
	{
		usbData[i] = uartRead();
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
		if (data == usbData && uartHaveData())
		{
			/* It doesn't matter what the request was.. */
			uartRead();
			/* Inform the connected PC */
			uartWrite(CMD_ERASE);
			uartWrite(RPL_BUSY);
		}
#endif
	}
	/* Deselect the device */
	spiChipSelect(false);
#ifndef NOUSB
	if (data == usbData)
	{
		/* It doesn't matter what the request was.. */
		uartRead();
		/* Write complete, so say erase completed! */
		uartWrite(CMD_ERASE);
		uartWrite(RPL_OK);
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
			uartWrite(CMD_ABORT);
			uartWrite(RPL_FAIL);
		}
#endif
		return;
	}
#ifndef NOUSB
	else if (data == usbData)
	{
		uartWrite(CMD_START);
		uartWrite(RPL_OK);
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
				uartWrite(CMD_PAGE);
				uartWrite(RPL_FAIL);
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
				uartWrite(CMD_ABORT);
				uartWrite(RPL_FAIL);
				programmed = false;
				break;
			}
			else
			{
				uartWrite(CMD_PAGE);
				uartWrite(RPL_OK);
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
			gpioShowOK();
	}
#endif
#ifndef NOUSB
	if (data == usbData)
	{
		uint8_t cmd = uartRead();
		if (cmd == CMD_STOP)
		{
			uint8_t i;
			uint32_t diff = usbDataTotal - usbDataReceived;
			for (i = 0; i < 4; i++)
			{
				uartWrite((diff >> 24) & 0xFF);
				diff <<= 8;
			}
			uartWrite(CMD_STOP);
			if (programmed)
			{
				uartWrite(RPL_OK);
				gpioShowOK();
			}
			else
				uartWrite(RPL_FAIL);
		}
		else
		{
			uartWrite(CMD_INVALID);
			uartWrite(RPL_FAIL);
		}
	}
#endif
}

int main()
{
	gpioInit();
	spiInit();
#ifndef NOUSB
	uartInit();
#endif

	while (1)
	{
#ifndef NOCONFIG
		/* If either button has been pressed.. */
		if (gpioCanTransfer())
		{
			gpioStopTimer();
			gpioBeginTransfer();
			gpioSignalTransfer();
			/* Attempt the transfer */
			transferBitfile(config, configLen);
			gpioEndTransfer();
			gpioStartTimer();
		}
#endif
#ifndef NOUSB
		/* If the UART has recieved a byte.. */
		if (uartHaveData())
		{
			if (uartPeak() == CMD_START)
			{
				uint8_t i;
				gpioStopTimer();
				gpioBeginTransfer();
				usbDataTotal = 0;
				usbDataReceived = 0;
				for (i = 0; i < 4; i++)
				{
					usbDataTotal <<= 8;
					usbDataTotal |= uartRead();
				}
				gpioSignalTransfer();
				transferBitfile(usbData, usbDataTotal);
				gpioEndTransfer();
				gpioStartTimer();
			}
			else
			{
				uartWrite(CMD_INVALID);
				uartWrite(RPL_FAIL);
			}
		}
#endif
		gpioCheckIdle();
	}

	return 0;
}

