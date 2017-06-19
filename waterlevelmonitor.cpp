#include "waterlevelmonitor.h"

WaterLevelMonitor::WaterLevelMonitor(QObject *parent) : QObject(parent)
{

}


double WaterLevelMonitor::getWaterLevel(int &ierr)
{
    ierr = 0;
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    return ((qrand() % ((this->_highWaterLevel + 1) - this->_lowWaterLevel)) + this->_lowWaterLevel);

}
