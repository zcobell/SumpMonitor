#include <QTime>
#include <QThread>
#include <unistd.h>
#include <QElapsedTimer>
#include "ultrasonicSensor.h"
#include "wiringPi.h"

UltrasonicSensor::UltrasonicSensor(int n, QObject *parent) : QObject(parent)
{
    this->_speedOfSound = this->speedOfSound(this->fahrenheit2Celcius(this->_temperature));
    this->_nProbes = n;
}


double UltrasonicSensor::getWaterLevel(int &ierr)
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

double UltrasonicSensor::_probeWaterLevel(int &ierr)
{

    struct timespec tim, tim2;
    tim.tv_sec  = 0;
    tim.tv_nsec = 10000L;

    double pulse_duration;
    double distance;
    QElapsedTimer timer;

    ierr = 0;

    pulse_duration = 0;
    distance       = 0.0;

    //...Map the input and output pins
    wiringPiSetupSys();
    pinMode(this->_triggerPin,OUTPUT);
    pinMode(this->_echoPin,INPUT);


    //...Settle the sensor
    digitalWrite(this->_triggerPin,LOW);
    QThread::msleep(500);

    //...Send the sound wave
    digitalWrite(this->_triggerPin,HIGH);
    nanosleep(&tim,&tim2); 
    digitalWrite(this->_triggerPin,LOW);

    //...Recieve the beginning of the sound wave
    while(digitalRead(this->_echoPin)==LOW)

    //...Start the timer
    timer.start();

    //...Recieve the end of the sound wave
    while(digitalRead(this->_echoPin)==HIGH);

    //...Stop the timer
    pulse_duration = timer.nsecsElapsed()*1e-9;

    //...Calculate the distance to the object in ft
    distance       = pulse_duration * this->_speedOfSound / 2.0 * 3.28084;

    //...Return the distance to the object
    //   and determine if we think this 
    //   measurement is valid (i.e. > 8ft is bogus)
    if(distance>8.0)
        ierr = -1;

    return distance;

}

double UltrasonicSensor::speedOfSound(double temperature)
{
    return 331.0+(0.6*temperature);
}

double UltrasonicSensor::fahrenheit2Celcius(double temperature)
{
    return (temperature-32.0)*(5.0/9.0);
}

