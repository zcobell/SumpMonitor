#ifndef FLOATSENSOR_H
#define FLOATSENSOR_H

#include <QObject>
#include "pins.h"

class FloatSensor : public QObject
{
    Q_OBJECT
public:
    explicit FloatSensor(QObject *parent = 0);

    bool getFloatStatus(int &ierr);

signals:

public slots:

private:
    int _floatPin;

};

#endif // FLOATSENSOR_H
