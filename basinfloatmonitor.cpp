#include "basinfloatmonitor.h"
#include "wiringPi.h"

BasinFloatMonitor::BasinFloatMonitor(QObject *parent) : QObject(parent)
{
    this->_floatPin = FLOAT_PIN;
}

bool BasinFloatMonitor::getFloatStatus(int &ierr)
{
    ierr = 0;
    int pinState;

    //...Map the read location
    wiringPiSetup();
    pinMode(this->_floatPin,INPUT);

    //...Read the pin
    pinState = digitalRead(this->_floatPin);

    //...Return the state
    if(pinState==LOW)
        return false;
    else
        return true;

}
