#ifndef BASINFLOATMONITOR_H
#define BASINFLOATMONITOR_H

#include <QObject>

class BasinFloatMonitor : public QObject
{
    Q_OBJECT
public:
    explicit BasinFloatMonitor(QObject *parent = 0);

    bool getFloatStatus();

signals:

public slots:
};

#endif // BASINFLOATMONITOR_H
