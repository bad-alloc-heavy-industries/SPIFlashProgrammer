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
#include "GPIO.h"

void gpioInit()
{
	/* Initialise P1_1 as OK indicator */
	SCU->SFS_Port1[1] = SCU_SFS_MODE_0 | SCU_SFS_DPU;
	GPIO_PORT1_DIR |= 0x00000002;
	GPIO_PORT1_CLR = 0x00000002;
}

void gpioStopTimer()
{
}

void gpioStartTimer()
{
}

void gpioCheckIdle()
{
}

bool gpioCanTransfer()
{
	return false;
}

void gpioBeginTransfer()
{
}

void gpioSignalTransfer()
{
}

void gpioEndTransfer()
{
}

void gpioShowOK()
{
	GPIO_PORT1_SET = 0x00000002;
}

