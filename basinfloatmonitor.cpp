#include "basinfloatmonitor.h"
#include "mmapGpio.h"


BasinFloatMonitor::BasinFloatMonitor(QObject *parent) : QObject(parent)
{

}

bool BasinFloatMonitor::getFloatStatus(int &ierr)
{
    ierr = 0;
    int pinState;

    //...Map the read location
    mmapGpio *rpiGpio = new mmapGpio();
    rpiGpio->setPinDir(this->_floatPin,mmapGpio::INPUT);

    //...Read the pin
    pinState = rpiGpio->readPin(this->_floatPin);
    delete rpiGpio;

    //...Return the state
    if(pinState==mmapGpio::LOW)
        return false;
    else
        return true;

}
