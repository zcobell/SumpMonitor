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
#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include "pins.h"
#include <QObject>

class UltrasonicSensor : public QObject {
  Q_OBJECT
public:
  explicit UltrasonicSensor(int n, QObject *parent = 0);

  //...Function that performs a series
  //   of measurements and averages the
  //   results
  double getWaterLevel(int &ierr);

private:
  int _highWaterLevel = 10;
  int _lowWaterLevel = 0;

  int _triggerPin = ULTRASONIC_TRIGGER;
  int _echoPin = ULTRASONIC_ECHO;

  int _nProbes;

  double _temperature = 65; //...Temperature in degrees fahrenheit
  double _speedOfSound;

  //...Function that performs the measurement
  double _probeWaterLevel(int &ierr);

  //...Calculate the speed of sound in air based on
  //   a simple formula related to degrees celcius
  double speedOfSound(double temperature);

  //...Calculate celcius from temperature in degrees fahrenheit
  double fahrenheit2Celcius(double temperature);
};

#endif // ULTRASONICSENSOR_H
