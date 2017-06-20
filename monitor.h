#ifndef MONITOR_H
#define MONITOR_H

#include "notifier.h"
#include "postsqldata.h"

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
    int _monitoringInterval = 10000; //msec
    Notifier *pushMessageSender;
    PostSQLData *sqlDatabase;

};

#endif // MONITOR_H
