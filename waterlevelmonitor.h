#ifndef WATERLEVELMONITOR_H
#define WATERLEVELMONITOR_H

#include <QObject>
#include <QTime>
#include <QThread>
#include "mmapGpio.h"

class WaterLevelMonitor : public QObject
{
    Q_OBJECT
public:
    explicit WaterLevelMonitor(QObject *parent = 0);

    double getWaterLevel(int &ierr);

signals:

public slots:

private:
    int _highWaterLevel = 10;
    int _lowWaterLevel  = 0;

    int _triggerPin     = 23;
    int _echoPin        = 24;

};

#endif // WATERLEVELMONITOR_H
