#ifndef MONITOR_H
#define MONITOR_H

#include <QObject>
#include <QTimer>
#include "notifier.h"
#include "waterlevelmonitor.h"
#include "basinfloatmonitor.h"

class Monitor : public QObject
{
    Q_OBJECT
public:
    explicit Monitor(QObject *parent = 0);

    int generateStatusMessage(bool floatStatus, double waterLevel, int &priority, QString &title, QString &message);

signals:
    void finished();
    void monitorError();

public slots:
    void run();
    void checkStatus();
    void endMonitor();

private:
    int _monitoringInterval = 60000;

};

#endif // MONITOR_H
