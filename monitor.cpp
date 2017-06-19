#include "monitor.h"

Monitor::Monitor(QObject *parent) : QObject(parent)
{
    this->pushMessageSender = new Notifier(this);
    this->sqlDatabase = new PostSQLData(this);
}


void Monitor::run()
{

    qDebug() << "Starting the monitoring routine...";

    //...Alter the user that the monitor was restarted
    this->pushMessageSender->sendRestartMessage();

    //...Begin the code to fire every _monitoringInterval
    //   milliseconds which will probe the status of the
    //   sump
    QTimer *monitorTimer = new QTimer(this);
    connect(monitorTimer,SIGNAL(timeout()),this,SLOT(checkStatus()));
    connect(this,SIGNAL(monitorError()),monitorTimer,SLOT(stop()));
    monitorTimer->start(this->_monitoringInterval);

}


void Monitor::checkStatus()
{
    int ierr;
    int priority;
    QString message;
    QString title;

    qDebug() << "Checking status at" << QDateTime::currentDateTime();

    //...Check the water level in the sump
    WaterLevelMonitor *waterLevel = new WaterLevelMonitor(this);
    double wl = waterLevel->getWaterLevel(ierr);
    if(ierr!=0)
        endMonitor();
    delete waterLevel;

    //...Check the basin float status
    BasinFloatMonitor *basinMonitor = new BasinFloatMonitor(this);
    bool fl = basinMonitor->getFloatStatus(ierr);
    if(ierr!=0)
        endMonitor();
    delete basinMonitor;

    //...Post data to SQL database on web server
    this->sqlDatabase->postData(wl,fl);

    //...Generate the status messages
    this->generateStatusMessage(fl,wl,priority,title,message);

    //...Send the message
    ierr = this->pushMessageSender->sendMessage(priority,title,message);

    if(ierr!=0)
        endMonitor();

    return;

}


int Monitor::generateStatusMessage(bool floatStatus, double waterLevel, int &priority, QString &title, QString &message)
{
    //...Generate the status
    if(floatStatus)
    {
        priority = PRIORITY_EMERGENCY;
        title    = "Sump Level Critical!";
        message  = "CRITICAL! Sump Water Level is "+QString::number(waterLevel)+". Attention is required immediately. The high water float has been toggled.";
    }
    else if(waterLevel>5)
    {
        priority = PRIORITY_HIGH;
        title    = "Sump Level is Abnormal";
        message  = "Sump pump should be checked immediately. Water level is "+QString::number(waterLevel)+" but the high water float has not been toggled yet.";
    }
    else
    {
        priority = PRIORITY_STANDARD;
        title    = "Sump Level is Normal";
        message  = "Sump level is normal. No need for action at this time. Water level is "+QString::number(waterLevel)+" and the float has not been toggled.";
    }
    return 0;
}


void Monitor::endMonitor()
{
    emit finished();
    return;
}
