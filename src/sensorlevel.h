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
#ifndef SENSORLEVEL_H
#define SENSORLEVEL_H

namespace SensorLevel{
  //...Alert water levels for the eTape
  const double eTapeHighLevel = 15.0;
  const double eTapeEmergencyLevel = 20.0;

  //...Alert water levels for the ultrasonic sensor
  const double ultrasonicHighLevel = 15.0;
  const double ultrasonicEmergencyLevel = 20.0;

  //...Linear calibration line for the eTape
  const double eTapeRegressionYIntercept = -53.785960;
  const double eTapeRegressionSlope = 0.109359;
}

#endif //SENSORLEVEL_H
