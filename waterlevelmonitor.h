#ifndef WATERLEVELMONITOR_H
#define WATERLEVELMONITOR_H

#include <QObject>
#include <QTime>

class WaterLevelMonitor : public QObject
{
    Q_OBJECT
public:
    explicit WaterLevelMonitor(QObject *parent = 0);

    double getWaterLevel();

signals:

public slots:

private:
    int _highWaterLevel = 10;
    int _lowWaterLevel  = 0;

};

#endif // WATERLEVELMONITOR_H
