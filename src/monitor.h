#ifndef MONITOR_H
#define MONITOR_H

#include "etapeSensor.h"
#include "floatSensor.h"
#include "notifier.h"
#include "postsqldata.h"
#include "ultrasonicSensor.h"
#include "sumpdata.h"
#include <QVector>

class Monitor : public QObject {
  Q_OBJECT
public:
  explicit Monitor(int monitoringInterval, int nsamples, bool continuous,
                   bool quiet, bool notifications, bool postData,
                   bool ultrasonicSensor, bool floatSensor, bool etapeSensor,
                   int notificationHour, QObject *parent = 0);

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
  Notifier *pushMessageSender;
  PostSQLData *sqlDatabase;
  FloatSensor *_floatSensor;
  EtapeSensor *_etapeSensor;
  UltrasonicSensor *_ultrasonicSensor;
  QVector<SumpData*> _monitorData;

  int generateStatusMessage(bool floatStatus, double waterLevel, int &priority,
                            QString &title, QString &message);
};

#endif // MONITOR_H
