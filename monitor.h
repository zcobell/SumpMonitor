#ifndef MONITOR_H
#define MONITOR_H

#include "notifier.h"
#include "postsqldata.h"
#include "ultrasonicSensor.h"
#include "floatSensor.h"
#include "etapeSensor.h"

class Monitor : public QObject
{
    Q_OBJECT
public:
    explicit Monitor(int monitoringInterval, int navg, bool continuous, bool quiet, bool notifications, bool postData,
                     bool ultrasonicSensor, bool floatSensor, bool etapeSensor, int notificationHour, QObject *parent = 0);

signals:
    void finished();
    void monitorError();

public slots:
    void run();
    void checkStatus();
    void endMonitor();

private:
    int _monitoringInterval;
    int _navg;
    bool _continuous;
    bool _verbose;
    bool _notifications;
    bool _postData;
    bool _useUltrasonic;
    bool _useFloat;
    bool _useEtape;
    Notifier *pushMessageSender;
    PostSQLData *sqlDatabase;
    FloatSensor *_floatSensor;
    EtapeSensor *_etapeSensor;
    UltrasonicSensor *_ultrasonicSensor;
    
    int generateStatusMessage(bool floatStatus, double waterLevel, int &priority, QString &title, QString &message);

};

#endif // MONITOR_H
