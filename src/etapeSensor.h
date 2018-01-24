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
#ifndef ETAPESENSOR_H
#define ETAPESENSOR_H

#include "pins.h"
#include <QObject>
#include <QVector>

#define ETAPE_SLOPE 0.109359
#define ETAPE_INTERCEPT -53.785960

class EtapeSensor : public QObject {
  Q_OBJECT
public:
  explicit EtapeSensor(int nSamples, QObject *parent = 0);

  double getWaterLevel(int &ierr);

private:
  double _readEtape(int &ierr);
  double _interpolateWaterLevel(int reading);
  double _analyzeMeasurements(QVector<double> measurements);

  int _nSamples = 100;
};

#endif // ETAPESENSOR_H
