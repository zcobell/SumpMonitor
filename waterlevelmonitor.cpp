#include "waterlevelmonitor.h"
#include <QDebug>
#include <unistd.h>
#include <QElapsedTimer>

WaterLevelMonitor::WaterLevelMonitor(QObject *parent) : QObject(parent)
{

}


double WaterLevelMonitor::getWaterLevel(int &ierr)
{
    ierr = 0;

    struct timespec tim, tim2;
    tim.tv_sec  = 0;
    tim.tv_nsec = 10000L;

    double pulse_duration;
    double distance;
    QElapsedTimer timer;

    pulse_duration = 0;
    distance       = 0.0;

    //...Map the input and output pins
    mmapGpio rpiGpio;
    rpiGpio.setPinDir(this->_triggerPin,mmapGpio::OUTPUT);
    rpiGpio.setPinDir(this->_echoPin,mmapGpio::INPUT);

    //...Settle the sensor
    rpiGpio.writePinLow(this->_triggerPin);
    QThread::sleep(2);

    //...Send the sound wave
    rpiGpio.writePinHigh(this->_triggerPin);
    nanosleep(&tim,&tim2); 
    rpiGpio.writePinLow(this->_triggerPin);

    //...Recieve the beginning of the sound wave
    while(rpiGpio.readPin(this->_echoPin)==mmapGpio::LOW)

    //...Start the timer
    timer.start();

    //...Recieve the end of the sound wave
    while(rpiGpio.readPin(this->_echoPin)==mmapGpio::HIGH)

    //...Stop the timer
    pulse_duration = timer.nsecsElapsed()*1e-9;

    //...Calculate the distance to the object in ft
    distance       = pulse_duration * 17150.0 / 30.48;

    //...Return the distance to the object
    return distance;
}
