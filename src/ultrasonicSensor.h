#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include "pins.h"
#include <QObject>

class UltrasonicSensor : public QObject {
  Q_OBJECT
public:
  explicit UltrasonicSensor(int n, QObject *parent = 0);

  //...Function that performs a series
  //   of measurements and averages the
  //   results
  double getWaterLevel(int &ierr);

private:
  int _highWaterLevel = 10;
  int _lowWaterLevel = 0;

  int _triggerPin = ULTRASONIC_TRIGGER;
  int _echoPin = ULTRASONIC_ECHO;

  int _nProbes;

  double _temperature = 65; //...Temperature in degrees fahrenheit
  double _speedOfSound;

  //...Function that performs the measurement
  double _probeWaterLevel(int &ierr);

  //...Calculate the speed of sound in air based on
  //   a simple formula related to degrees celcius
  double speedOfSound(double temperature);

  //...Calculate celcius from temperature in degrees fahrenheit
  double fahrenheit2Celcius(double temperature);
};

#endif // ULTRASONICSENSOR_H
