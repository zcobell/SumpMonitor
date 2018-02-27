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
#include "etapeSensor.h"
#include <unistd.h>
#include <QThread>
#include <algorithm>
#include "mcp3004.h"
#include "pins.h"
#include "sensorlevel.h"
#include "wiringPi.h"

EtapeSensor::EtapeSensor(int nSamples, QObject *parent) : QObject(parent) {
  this->_nSamples = nSamples;

  //...Setup the SPI channels
  wiringPiSetup();
  mcp3004Setup(SPI_BASE, SPI_CHANNEL_ETAPE);
}

double EtapeSensor::getWaterLevel(int &ierr) { return this->_readEtape(ierr); }

double EtapeSensor::getWaterLevel(int &raw, int &ierr) {
  return this->_readEtape(raw, ierr);
}

double EtapeSensor::_readEtape(int &ierr) {
  int i;
  double waterlevel;
  QVector<double> measurements;

  measurements.resize(this->_nSamples);

  for (i = 0; i < this->_nSamples; i++) {
    measurements[i] =
        this->_interpolateWaterLevel(analogRead(SPI_BASE + SPI_CHANNEL_ETAPE));
    QThread::msleep(10);
  }

  waterlevel = this->_analyzeMeasurements(measurements);
  ierr = 0;
  return waterlevel;
}

double EtapeSensor::_readEtape(int &raw, int &ierr) {
  ierr = 0;
  raw = analogRead(SPI_BASE + SPI_CHANNEL_ETAPE);
  return this->_interpolateWaterLevel(raw);
}

double EtapeSensor::_interpolateWaterLevel(int reading) {
  //...Use defined slope and intercept
  //   to calculate water level as a function
  //   of reading from etape
  return SensorLevel::eTapeRegressionSlope * (double)reading +
         SensorLevel::eTapeRegressionYIntercept;
}

double EtapeSensor::_analyzeMeasurements(QVector<double> measurements) {
  double wl;
  int i, n, q1, q2;

  wl = 0.0;
  n = 0;

  if (measurements.size() > 20) {
    std::sort(measurements.begin(), measurements.end());
    q1 = measurements.size() / 2;
    q2 = measurements.size() / 10;

    for (i = q1 - q2; i < q1 + q2; i++) {
      wl = wl + measurements[i];
      n = n + 1;
    }
    wl = wl / n;
  } else {
    for (i = 0; i < measurements.size(); i++) wl = wl + measurements[i];
    wl = wl / measurements.size();
  }
  return wl;
}
