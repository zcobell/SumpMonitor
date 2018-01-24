//------------------------------GPLv3------------------------------------//
//
//  This file is part of SumpMonitor.
//
//  SumpMonitor is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  SumpMonitor is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SumpMonitor. If not, see <http://www.gnu.org/licenses/>.
//
//------------------------------GPLv3------------------------------------//
#ifndef PINS_H
#define PINS_H

//...Ultrasonic sensor setup
#define ULTRASONIC_TRIGGER 4
#define ULTRASONIC_ECHO    5

//...Float sensor setup
#define FLOAT_PIN          7

//...ETape SPI Channel setup 
//   SPI Channel in the MCP3008
#define SPI_BASE           100
#define SPI_CHANNEL_ETAPE  0

#endif //PINS_H
