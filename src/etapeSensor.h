#ifndef ETAPESENSOR_H
#define ETAPESENSOR_H

#include "pins.h"
#include <QObject>
#include <QVector>

#define ETAPE_SLOPE 0.109359
#define ETAPE_INTERCEPT -53.785960

class EtapeSensor : public QObject {
  Q_OBJECT
public:
  explicit EtapeSensor(int nSamples, QObject *parent = 0);

  double getWaterLevel(int &ierr);

private:
  double _readEtape(int &ierr);
  double _interpolateWaterLevel(int reading);
  double _analyzeMeasurements(QVector<double> measurements);

  int _nSamples = 100;
};

#endif // ETAPESENSOR_H
