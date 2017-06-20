#include <QTime>
#include <QThread>
#include <unistd.h>
#include <QElapsedTimer>
#include "waterlevelmonitor.h"
#include "mmapGpio.h"

WaterLevelMonitor::WaterLevelMonitor(QObject *parent) : QObject(parent)
{
    this->_speedOfSound = this->speedOfSound(this->fahrenheit2Celcius(this->_temperature));
}


double WaterLevelMonitor::getWaterLevel(int &ierr)
{
    double total;

    //...Probe the water level a number of times
    //   and take the average. Attempts to smooth
    //   some of the noise in the measurement
    total = 0.0;
    for(int i=0;i<this->_nProbes;i++)
    {
        total = total + this->_probeWaterLevel(ierr);
    }
    
    ierr = 0;

    return total/this->_nProbes;

}

double WaterLevelMonitor::_probeWaterLevel(int &ierr)
{

    struct timespec tim, tim2;
    tim.tv_sec  = 0;
    tim.tv_nsec = 10000L;

    double pulse_duration;
    double distance;
    QElapsedTimer timer;

    pulse_duration = 0;
    distance       = 0.0;

    //...Map the input and output pins
    mmapGpio *rpiGpio = new mmapGpio();
    rpiGpio->setPinDir(this->_triggerPin,mmapGpio::OUTPUT);
    rpiGpio->setPinDir(this->_echoPin,mmapGpio::INPUT);

    //...Settle the sensor
    rpiGpio->writePinLow(this->_triggerPin);
    QThread::msleep(500);

    //...Send the sound wave
    rpiGpio->writePinHigh(this->_triggerPin);
    nanosleep(&tim,&tim2); 
    rpiGpio->writePinLow(this->_triggerPin);

    //...Recieve the beginning of the sound wave
    while(rpiGpio->readPin(this->_echoPin)==mmapGpio::LOW)

    //...Start the timer
    timer.start();

    //...Recieve the end of the sound wave
    while(rpiGpio->readPin(this->_echoPin)==mmapGpio::HIGH)

    //...Stop the timer
    pulse_duration = timer.nsecsElapsed()*1e-9;

    //...Calculate the distance to the object in ft
    distance       = pulse_duration * this->_speedOfSound / 2.0 * 3.28084;

    //...Cleanup
    delete rpiGpio;

    //...Return the distance to the object
    return distance;

}

double WaterLevelMonitor::speedOfSound(double temperature)
{
    return 331.0+(0.6*temperature);
}

double WaterLevelMonitor::fahrenheit2Celcius(double temperature)
{
    return (temperature-32.0)*(5.0/9.0);
}

