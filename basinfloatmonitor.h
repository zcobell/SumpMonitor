#ifndef BASINFLOATMONITOR_H
#define BASINFLOATMONITOR_H

#include <QObject>
#include "pins.h"

class BasinFloatMonitor : public QObject
{
    Q_OBJECT
public:
    explicit BasinFloatMonitor(QObject *parent = 0);

    bool getFloatStatus(int &ierr);

signals:

public slots:

private:
    int _floatPin = FLOAT_PIN;

};

#endif // BASINFLOATMONITOR_H
