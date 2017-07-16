#include <QThread>
#include <unistd.h>
#include <algorithm>
#include "etapeSensor.h"
#include "wiringPi.h"
#include "mcp3008.h"
#include "pins.h"
#include <QDebug>

EtapeSensor::EtapeSensor(int nSamples, QObject *parent) : QObject(parent)
{
    
    this->_nSamples = nSamples;

    //...Setup the SPI channels
    wiringPiSetup();
    mcp3008Setup(SPI_BASE,SPI_CHANNEL_ETAPE);
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
    double wl;
    int i,n,q1,q2;
    
    wl = 0.0;
    n  = 0;

    if(measurements.size()>20)
    {
        std::sort(measurements.begin(),measurements.end());
        q1 = measurements.size()/2;
        q2 = measurements.size()/10;
    
        for(i=q1-q2;i<q1+q2;i++)
        {
            wl = wl + measurements[i];
            n  = n + 1;
        }
        wl = wl / n;
    }
    else
    {
        for(i=0;i<measurements.size();i++)
            wl = wl + measurements[i]; 
        wl = wl / measurements.size();
    }
    return wl;
}
