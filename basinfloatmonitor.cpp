#include "basinfloatmonitor.h"

BasinFloatMonitor::BasinFloatMonitor(QObject *parent) : QObject(parent)
{

}

bool BasinFloatMonitor::getFloatStatus(int &ierr)
{
    ierr = 0;
    int floatStat = ((qrand() % ((1 + 1) - 0)) + 0);
    if(floatStat==0)
        return false;
    else
        return true;
}
