/*
 * mcp3008.c:
 *	Extend wiringPi with the MCP3008 SPI Analog to Digital convertor
 *	Copyright (c) 2012-2013 Gordon Henderson
 *
 *	Thanks also to "ShorTie" on IRC for some remote debugging help!
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with wiringPi.
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "mcp3008.h"

/*
 * myAnalogRead:
 *	Return the analog value of the given pin
 *********************************************************************************
 */

static int myAnalogRead (struct wiringPiNodeStruct *node, int pin)
{
  unsigned char spiData [3] ;
  unsigned char chanBits ;
  int chan = pin - node->pinBase ;

  chanBits = 0b10000000 | (chan << 4) ;

  spiData [0] = 1 ;		// Start bit
  spiData [1] = chanBits ;
  spiData [2] = 0 ;

  wiringPiSPIDataRW (node->fd, spiData, 3) ;

  return ((spiData [1] << 8) | spiData [2]) & 0x3FF ;
}


/*
 * mcp3008Setup:
 *	Create a new wiringPi device node for an mcp3008 on the Pi's
 *	SPI interface.
 *********************************************************************************
 */

int mcp3008Setup (const int pinBase, int spiChannel)
{
  struct wiringPiNodeStruct *node ;

  if (wiringPiSPISetup (spiChannel, 75000) < 0)
    return FALSE ;

  node = wiringPiNewNode (pinBase, 8) ;

  node->fd         = spiChannel ;
  node->analogRead = myAnalogRead ;

  return TRUE ;
}
