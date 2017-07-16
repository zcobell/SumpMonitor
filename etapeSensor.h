#ifndef ETAPESENSOR_H
#define ETAPESENSOR_H

#include <QObject>
#include <QHash>
#include "pins.h"

class EtapeSensor : public QObject
{
    Q_OBJECT
public:
    explicit EtapeSensor(QObject *parent = 0);

    double getWaterLevel(int &ierr);

private:

    double _readEtape(int &ierr);
    double _interpolateWaterLevel(int reading);

    void _buildInterpolant();

    int    _nSamples = 20;

    QHash<int,double> _interpolant;


};

#endif // ETAPESENSOR_H
