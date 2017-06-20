#include "waterlevelmonitor.h"
#include <QDebug>

WaterLevelMonitor::WaterLevelMonitor(QObject *parent) : QObject(parent)
{

}


double WaterLevelMonitor::getWaterLevel(int &ierr)
{
    ierr = 0;

    quint64 pulse_start,pulse_end,pulse_duration;
    double distance;

    //...Map the input and output pins
    mmapGpio rpiGpio;
    rpiGpio.setPinDir(this->_triggerPin,mmapGpio::OUTPUT);
    rpiGpio.setPinDir(this->_echoPin,mmapGpio::INPUT);

    //...Settle the sensor
    rpiGpio.writePinLow(this->_triggerPin);
    QThread::sleep(2);

    //...Send the sound wave
    rpiGpio.writePinHigh(this->_triggerPin);
    QThread::msleep(1);
    rpiGpio.writePinLow(this->_triggerPin);

    //...Recieve the beginning of the sound wave
    while(rpiGpio.readPin(this->_echoPin)==mmapGpio::LOW)
        pulse_start = QDateTime::currentMSecsSinceEpoch();

    //...Recieve the end of the sound wave
    while(rpiGpio.readPin(this->_echoPin)==mmapGpio::HIGH)
        pulse_end   = QDateTime::currentMSecsSinceEpoch();

    //...Calculate the distance to the object in cm
    pulse_duration = pulse_end - pulse_start;
    distance       = pulse_duration * 17150.0;

    qDebug() << distance;


}
