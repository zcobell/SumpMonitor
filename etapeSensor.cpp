#include <QThread>
#include <unistd.h>
#include <algorithm>
#include "etapeSensor.h"
#include "wiringPi.h"
#include "mcp3004.h"
#include "pins.h"

EtapeSensor::EtapeSensor(QObject *parent) : QObject(parent)
{
    //...Setup the SPI channels
    wiringPiSetup();
    mcp3004Setup(SPI_BASE,SPI_CHANNEL_ETAPE);
}


double EtapeSensor::getWaterLevel(int &ierr)
{
    return this->_readEtape(ierr); 
}


double EtapeSensor::_readEtape(int &ierr)
{
    int i;
    double waterlevel;
    QVector<double> measurements;

    measurements.resize(this->_nSamples);

    for(i=0;i<this->_nSamples;i++)
    {
        measurements[i] = this->_interpolateWaterLevel(analogRead(SPI_BASE+SPI_CHANNEL_ETAPE));
        QThread::msleep(10);
    }

    waterlevel = this->_analyzeMeasurements(measurements);
    ierr = 0;
    return waterlevel;
}


double EtapeSensor::_interpolateWaterLevel(int reading)
{
    //...Use header defined slope and intercept
    //   to calculate water level as a function
    //   of reading from etape
    return ETAPE_SLOPE*(double)reading + ETAPE_INTERCEPT;
}


double EtapeSensor::_analyzeMeasurements(QVector<double> measurements)
{
    int q,n,i;
    double q1,q3,dq,f,f1,f3,wl;

    std::sort(measurements.begin(),measurements.end());
    
    q = round(measurements.size()/4);

    q1 = measurements[q];
    q3 = measurements[3*q];

    dq = q3-q1;

    f  = dq*1.5;

    f1 = q1 - f;
    f3 = q3 + f;

    wl = 0.0;
    n  = 0;

    for(i=0;i<measurements.size();i++)
    {
        if(measurements[i]>f1 && measurements[i]<f3)
        {
            wl = wl + measurements[i];
            n  = n + 1;
        }
    }

    wl = wl / (double)n;

    return wl;
}
