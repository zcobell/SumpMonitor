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
#ifndef MONITOR_H
#define MONITOR_H

#include "etapeSensor.h"
#include "floatSensor.h"
#include "notifier.h"
#include "postsqldata.h"
#include "sumpdata.h"
#include "ultrasonicSensor.h"
#include "netcdfdata.h"
#include <QVector>

class Monitor : public QObject {
  Q_OBJECT
public:
  explicit Monitor(int monitoringInterval, int nsamples, bool continuous,
                   bool quiet, bool notifications, bool postData,
                   bool ultrasonicSensor, bool floatSensor, bool etapeSensor,
                   int notificationHour, bool netcdfOutput, QString netcdfFilename,
                   QObject *parent = 0);

signals:
  void finished();
  void monitorError();

public slots:
  void run();
  void checkStatus();
  void endMonitor();

private:
  int _monitoringInterval;
  int _nsamples;
  bool _continuous;
  bool _verbose;
  bool _notifications;
  bool _postData;
  bool _useUltrasonic;
  bool _useFloat;
  bool _useEtape;
  bool _restartNotifications = false;
  bool _netcdfOutput;
  QString _netcdfFilename;
  Notifier *pushMessageSender;
  PostSQLData *sqlDatabase;
  FloatSensor *_floatSensor;
  EtapeSensor *_etapeSensor;
  UltrasonicSensor *_ultrasonicSensor;
  QVector<SumpData *> _monitorData;
  NetcdfData *_ncdata;

  int generateStatusMessage(bool floatStatus, double waterLevel, int &priority,
                            QString &title, QString &message);
};

#endif // MONITOR_H
