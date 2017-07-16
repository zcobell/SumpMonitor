#include <QTime>
#include <QThread>
#include <unistd.h>
#include <QElapsedTimer>
#include "etapeSensor.h"
#include "wiringPi.h"
#include "mcp3004.h"
#include "pins.h"
#include <QDebug>

#define BASE 100

EtapeSensor::EtapeSensor(QObject *parent) : QObject(parent)
{
    this->_buildInterpolant();
}


double EtapeSensor::getWaterLevel(int &ierr)
{
    return this->_readEtape(ierr); 
}


double EtapeSensor::_readEtape(int &ierr)
{
    int i;
    double waterLevel;

    //...Setup the SPI channels
    wiringPiSetup();
    mcp3004Setup(100,SPI_CHANNEL_ETAPE);

    //...Since sensor isn't totally stables, 
    //   average a number of readings
    waterLevel = 0.0;
    for(i=0;i<this->_nSamples;i++)
    {
        waterLevel = waterLevel + this->_interpolateWaterLevel(analogRead(BASE));
        QThread::msleep(50);
    }
    waterLevel = waterLevel / this->_nSamples;

    return waterLevel;
}


double EtapeSensor::_interpolateWaterLevel(int reading)
{
    return double(reading);
}


void EtapeSensor::_buildInterpolant()
{
    this->_interpolant[500] = 0.0;
    this->_interpolant[1000] = 1.0;
    return;
}
