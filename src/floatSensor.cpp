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
#include "floatSensor.h"
#include "wiringPi.h"

FloatSensor::FloatSensor(QObject *parent) : QObject(parent) {
  this->_floatPin = FLOAT_PIN;
  
  //...Map the read location
  wiringPiSetup();
  pinMode(this->_floatPin, INPUT);

}

bool FloatSensor::getFloatStatus(int &ierr) {
  ierr = 0;
  int pinState;

  //...Read the pin
  pinState = digitalRead(this->_floatPin);

  //...Return the state
  if (pinState == LOW)
    return false;
  else
    return true;
}
