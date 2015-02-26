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

#ifndef USBREQUESTS_H
#define	USBREQUESTS_H

extern void usbRequestGetDescriptor(volatile usbSetupPacket_t *packet);
extern void usbRequestSetConfiguration();
extern void usbRequestGetStatus();
extern void usbRequestDoFeature();

extern volatile usbDeviceState usbState;
extern volatile uint8_t usbActiveConfig;

#endif	/* USBREQUESTS_H */
