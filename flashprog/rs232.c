/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2005, 2006, 2007, 2008, 2009 Teunis van Beelen
*
* teuniz@gmail.com
*
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
*
* This version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*
* Modified by Rachel Mant
* Copyright (C) 2012,2014 Rachel Mant
*
* Revisions made:
*	Unified the function names so they all match in style
*	Fixed some clarity issues with function names
*	Rewritten the error reporting system so the user can do what they will with the error strings
*		rather than having to use stderr
*	Fixed the stupid that was rs232.h and moved the worst into this file
*	Made the header make proper use of extern
*	Added errno saving where needed to avoid errno wierdness after close() calls
*	Added the parameter names from the functions here to the prototypes in the header
*	Reformatted much of the code for easier maintenance
*	Fixed the weird use of return as if it were a function
*	Fixed some evil code that would break on 64-bit platforms in the Windows version of the code
*	Fixed some usage of globals where they should have been locals
*	Massively simplified the Windows baud rate selection code
*	Switched various memset calls over to on-definition local initialisations for various structures
*	Changed OpenCOMPort to taking the path of the port to open rather than an integer index.
*	Changed the code to only support one open port at a time to simplify useage
*
*/

#include <stdio.h>
#include <string.h>

#ifndef _MSC_VER
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#else
#include <windows.h>
#endif
#include <errno.h>
#include <malloc.h>

#include "strUtils.h"
#include "rs232.h"

char *COMError = NULL;

void FreeLastError()
{
	if (COMError != NULL)
		free(COMError);
	COMError = NULL;
}

#ifndef _MSC_VER /* Linux */

int error, portFD = -1;
struct termios oldPortSettings;

void doHackClear(int fd)
{
	char _;
	int _read, toRead = 0;
	ioctl(fd, FIONREAD, &toRead);
	for (_read = 0; _read < toRead; _read++)
		read(fd, &_, 1);
}

int OpenCOMPort(char *comPort, int baudrate)
{
	struct termios newPortSettings = {0};
	int baudr;

	FreeLastError();
	if (comPort == NULL)
	{
		COMError = strdup("Illegal COM port path");
		return 1;
	}
	else if (portFD != -1)
	{
		COMError = strdup("Another COM port is already open");
		return 1;
	}

	switch (baudrate)
	{
		case 50:
			baudr = B50;
			break;
		case 75:
			baudr = B75;
			break;
		case 110:
			baudr = B110;
			break;
		case 134:
			baudr = B134;
			break;
		case 150:
			baudr = B150;
			break;
		case 200:
			baudr = B200;
			break;
		case 300:
			baudr = B300;
			break;
		case 600:
			baudr = B600;
			break;
		case 1200:
			baudr = B1200;
			break;
		case 1800:
			baudr = B1800;
			break;
		case 2400:
			baudr = B2400;
			break;
		case 4800:
			baudr = B4800;
			break;
		case 9600:
			baudr = B9600;
			break;
		case 19200:
			baudr = B19200;
			break;
		case 38400:
			baudr = B38400;
			break;
		case 57600:
			baudr = B57600;
			break;
		case 115200:
			baudr = B115200;
			break;
		case 230400:
			baudr = B230400;
			break;
		case 460800:
			baudr = B460800;
			break;
		case 500000:
			baudr = B500000;
			break;
		case 576000:
			baudr = B576000;
			break;
		case 921600:
			baudr = B921600;
			break;
		case 1000000:
			baudr = B1000000;
			break;
		default:
			COMError = strdup("Invalid baudrate");
			return 1;
			break;
	}

	portFD = open(comPort, O_RDWR | O_NOCTTY | O_NDELAY | O_SYNC);
	if (portFD == -1)
	{
		COMError = formatString("Unable to open comport: %s", strerror(errno));
		return 1;
	}

	error = tcgetattr(portFD, &oldPortSettings);
	if (error == -1)
	{
		int errNo = errno;
		close(portFD);
		COMError = formatString("Unable to read portsettings: %s", strerror(errNo));
		return 1;
	}

	newPortSettings.c_cflag = baudr | CS8 | CLOCAL | CREAD;
	newPortSettings.c_iflag = IGNPAR | IGNCR | IGNBRK;

	error = cfsetspeed(&newPortSettings, baudr);
	if (error >= 0)
		error = tcsetattr(portFD, TCSAFLUSH, &newPortSettings);
	if (error == -1)
	{
		int errNo = errno;
		close(portFD);
		COMError = formatString("Unable to adjust port settings: %s", strerror(errNo));
		return 1;
	}
	posix_fadvise(portFD, 0, 0, POSIX_FADV_SEQUENTIAL | POSIX_FADV_NOREUSE);
	tcflush(portFD, TCIOFLUSH);
	doHackClear(portFD);

	return 0;
}

int ReadCOMPort(unsigned char *buf, int size)
{
	FreeLastError();
#ifndef __STRICT_ANSI__	/* __STRICT_ANSI__ is defined when the -ansi option is used for gcc */
	if (size > SSIZE_MAX)
		size = (int)SSIZE_MAX;	/* SSIZE_MAX is defined in limits.h */
#else
	if (size > 4096)
		size = 4096;
#endif
	return read(portFD, buf, size);
}

int WriteCOMPort(unsigned char byte)
{
	int n;

	FreeLastError();
	n = write(portFD, &byte, 1);
	if (n < 0)
		return 1;
	tcdrain(portFD);
	return 0;
}

int SendBuf(unsigned char *buf, int size)
{
	FreeLastError();
	return write(portFD, buf, size);
}

void CloseCOMPort()
{
	FreeLastError();
	tcsetattr(portFD, TCSAFLUSH, &oldPortSettings);
	close(portFD);
}

/*
Constant	Description
TIOCM_LE	DSR (data set ready/line enable)
TIOCM_DTR DTR (data terminal ready)
TIOCM_RTS RTS (request to send)
TIOCM_ST	Secondary TXD (transmit)
TIOCM_SR	Secondary RXD (receive)
TIOCM_CTS CTS (clear to send)
TIOCM_CAR DCD (data carrier detect)
TIOCM_CD	Synonym for TIOCM_CAR
TIOCM_RNG RNG (ring)
TIOCM_RI	Synonym for TIOCM_RNG
TIOCM_DSR DSR (data set ready)
*/

int IsCTSEnabled()
{
	int status;

	FreeLastError();
	status = ioctl(portFD, TIOCMGET, &status);

	if (status & TIOCM_CTS)
		return 1;
	else
		return 0;
}


#else /* windows */

HANDLE portFD;
static const char *portFmt = "\\\\.\\COM%u";

int OpenCOMPort(int comNum, int baudrate)
{
	char *baudr, *port;
	COMMTIMEOUTS portTimeouts = {0};
	DCB portSettings = {0};
	COMSTAT status = {0};

	FreeLastError();
	if ((comNum > 15) || (comNum < 0))
	{
		COMError = strdup("Illegal comport number");
		return 1;
	}

	switch (baudrate)
	{
		case 110:
		case 300:
		case 600:
		case 1200:
		case 2400:
		case 4800:
		case 9600:
		case 19200:
		case 38400:
		case 57600:
		case 128000:
		case 256000:
			baudr = formatString("baud=%d data=8 parity=N stop=1", baudrate);
			break;
		default:
			COMError = strdup("invalid baudrate");
			return 1;
			break;
	}

	port = formatString(portFormat, comNum);
	portFD = CreateFileA(, GENERIC_READ|GENERIC_WRITE, 0 /* no share */, NULL /* no security */, OPEN_EXISTING, 0 /* no threads */, NULL /* no templates */);
	free(port);

	if (portFD == INVALID_HANDLE_VALUE)
	{
		COMError = strdup("unable to open comport");
		free(baudr);
		return 1;
	}

	port_settings.DCBlength = sizeof(portSettings);
	if (!BuildCommDCBA(baudr, &portSettings))
	{
		COMError = strdup("unable to set comport dcb settings");
		free(baudr);
		CloseHandle(portFD);
		return 1;
	}
	free(baudr);

	if (!SetCommState(portFD, &portSettings))
	{
		COMError = strdup("unable to set comport cfg settings");
		CloseHandle(portFD);
		return 1;
	}

	Cptimeouts.ReadIntervalTimeout = MAXDWORD;

	if (!SetCommTimeouts(portFD, &portTimeouts))
	{
		COMError = strdup("unable to set comport time-out settings");
		CloseHandle(portFD);
		return 1;
	}

	SetCommMask(portFD, EV_RXCHAR);
	ClearCommError(portFD, NULL, &Status);
	{
		unsigned char c[2];
		while (Status.cbInQue > 0)
		{
			ReadCOMPort(c, 1);
			Status.cbInQue--;
		}
	}

	return 0;
}

int ReadCOMPort(unsigned char *buf, int size)
{
	DWORD n;
	FreeLastError();
	if (size > 4096)
		size = 4096;
	WaitCommEvent(portFD, &n, NULL);
	ReadFile(portFD, buf, size, &n, NULL);
	return n;
}

int WriteCOMPort(unsigned char byte)
{
	DWORD n;
	FreeLastError();
	PurgeComm(portFD, PURGE_RXCLEAR);
	WriteFile(portFD, &byte, 1, &n, NULL);
	if (n < 0)
		return 1;
	return 0;
}

int SendBuf(unsigned char *buf, int size)
{
	DWORD n;
	FreeLastError();
	if (WriteFile(portFD, buf, size, &n, NULL))
		return n;
	return -1;
}

void CloseCOMPort()
{
	CloseHandle(portFD);
}

int IsCTSEnabled()
{
	DWORD status;
	FreeLastError();
	GetCommModemStatus(portFD, &status);
	if (status & MS_CTS_ON)
		return 1;
	else
		return 0;
}
#endif

void SendString(const char *text)	/* sends a string to serial port */
{
	while (*text != 0)
	{
		WriteCOMPort(*text);
		text++;
	}
}
