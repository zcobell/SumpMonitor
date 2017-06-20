#include <QObject>
#include <QTimer>
#include <stdio.h>
#include "monitor.h"
#include "waterlevelmonitor.h"
#include "basinfloatmonitor.h"

Monitor::Monitor(int monitoringInterval, bool continuous, bool verbose, bool notifications, bool postData, QObject *parent) : QObject(parent)
{
    this->pushMessageSender = new Notifier(this);
    this->sqlDatabase = new PostSQLData(this);
    this->_monitoringInterval = monitoringInterval*1000;
    this->_continuous = continuous;
    this->_verbose = verbose;
    this->_notifications = notifications;
    this->_postData = postData;
}


void Monitor::run()
{

    QTextStream out(stdout,QIODevice::WriteOnly);

    if(this->_verbose)
        out << "Starting the monitoring routine...\n";
        out.flush();

    //...Alter the user that the monitor was restarted
    if(this->_notifications)
        this->pushMessageSender->sendRestartMessage();

    //...Begin the code to fire every _monitoringInterval
    //   milliseconds which will probe the status of the
    //   sump
    if(this->_continuous)
    {
        QTimer *monitorTimer = new QTimer(this);
        connect(monitorTimer,SIGNAL(timeout()),this,SLOT(checkStatus()));
        connect(this,SIGNAL(monitorError()),monitorTimer,SLOT(stop()));
        monitorTimer->start(this->_monitoringInterval);
    }
    else
    {    
        this->checkStatus();
        this->endMonitor();
    }

}


void Monitor::checkStatus()
{
    int ierr;
    int priority;
    QString message;
    QString title;

    QTextStream out(stdout,QIODevice::WriteOnly);
    
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
    if(this->_postData)
        this->sqlDatabase->postData(wl,fl);

    //...Generate the status messages
    if(this->_notifications)
    {
        this->generateStatusMessage(fl,wl,priority,title,message);

        //...Send the message
        ierr = this->pushMessageSender->sendMessage(priority,title,message);

        if(ierr!=0)
            endMonitor();
    }

    //...Log message to screen
    if(this->_verbose)
    {
        QString wls;
        wls.sprintf("%6.2f",wl);
        out << QString("|-------------------------------------------------------------| \n");
        out << QString("|  Sump Status @ "+QDateTime::currentDateTime().toString()+"                     |\n");
        out << QString("|                                                             |\n");
        out << QString("|      Water Level: "+wls+"                                    |\n");
        if(fl)
            out << QString("|     Float Status: Triggered                                 |\n");
        else
            out << QString("|     Float Status: Not Triggered                             |\n");
        out << QString("|-------------------------------------------------------------| \n\n");
        out.flush();
    }

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
