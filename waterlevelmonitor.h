#ifndef WATERLEVELMONITOR_H
#define WATERLEVELMONITOR_H

#include <QObject>

class WaterLevelMonitor : public QObject
{
    Q_OBJECT
public:
    explicit WaterLevelMonitor(QObject *parent = 0);

    //...Function that performs a series
    //   of measurements and averages the 
    //   results
    double getWaterLevel(int &ierr);

private:
    int _highWaterLevel = 10;
    int _lowWaterLevel  = 0;

    int _triggerPin     = 23;
    int _echoPin        = 24;

    int _nProbes        = 5;

    double _temperature  = 65; //...Temperature in degrees fahrenheit
    double _speedOfSound;

    //...Function that performs the measurement
    double _probeWaterLevel(int &ierr);

    //...Calculate the speed of sound in air based on
    //   a simple formula related to degrees celcius
    double speedOfSound(double temperature);

    //...Calculate celcius from temperature in degrees fahrenheit
    double fahrenheit2Celcius(double temperature);

};

#endif // WATERLEVELMONITOR_H
