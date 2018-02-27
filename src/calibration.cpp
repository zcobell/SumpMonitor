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
#include "calibration.h"
#include <stdio.h>
#include <QObject>
#include <QTextStream>
#include <QTimer>
#include "etapeSensor.h"
#include "sensorlevel.h"

Calibration::Calibration(QObject *parent) : QObject(parent) {
  this->_etapeSensor = new EtapeSensor(0, this);
}

void Calibration::run() {
  QTextStream out(stdout, QIODevice::WriteOnly);

  QTimer *calibrationTimer = new QTimer(this);
  connect(calibrationTimer, SIGNAL(timeout()), this, SLOT(checkStatus()));
  connect(this, SIGNAL(calibrationError()), calibrationTimer, SLOT(stop()));

  //...Header
  out << "INTERPOLATED VALUE         READING\n";
  out.flush();

  //...Call the status routine the first time
  this->checkStatus();

  //...Now call the status routine every _montitoringInterval seconds
  calibrationTimer->start(600);

  return;
}

void Calibration::checkStatus() {
  int ierr;
  int raw;

  QTextStream out(stdout, QIODevice::WriteOnly);

  double wl = this->_etapeSensor->getWaterLevel(raw, ierr);

  out << wl << "              " << raw << "\n";
  out.flush();
}

void Calibration::endCalibration() {
  emit finished();
  return;
}
