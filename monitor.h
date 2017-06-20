#ifndef MONITOR_H
#define MONITOR_H

#include "notifier.h"
#include "postsqldata.h"

class Monitor : public QObject
{
    Q_OBJECT
public:
    explicit Monitor(int monitoringInterval, int navg, bool continuous, bool verbose, bool notifications, bool postData, QObject *parent = 0);

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
    Notifier *pushMessageSender;
    PostSQLData *sqlDatabase;
    
    int generateStatusMessage(bool floatStatus, double waterLevel, int &priority, QString &title, QString &message);

};

#endif // MONITOR_H
