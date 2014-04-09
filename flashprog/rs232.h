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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
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
*/

#ifndef RS232_H
#define RS232_H

#ifndef RS232_EXTERN
	#ifndef __cplusplus
		#define RS232_EXTERN extern
	#else
		#define RS232_EXTERN extern "C"
	#endif
#endif

RS232_EXTERN char *COMError;

#ifndef _MSC_VER
RS232_EXTERN int OpenCOMPort(char *comPort, int baudrate);
#else
RS232_EXTERN int OpenCOMPort(int comNum, int baudrate);
#endif
RS232_EXTERN int ReadCOMPort(unsigned char *buf, int size);
RS232_EXTERN int WriteCOMPort(unsigned char byte);
RS232_EXTERN int SendBuf(unsigned char *buf, int size);
RS232_EXTERN void CloseCOMPort();
RS232_EXTERN void SendString(const char *text);
RS232_EXTERN int IsCTSEnabled();

#endif /*RS232_H*/
