#include <QObject>
#include <QTimer>
#include <stdio.h>
#include "monitor.h"
#include "ultrasonicSensor.h"
#include "floatSensor.h"
#include "etapeSensor.h"

Monitor::Monitor(int monitoringInterval, int navg, bool continuous, bool quiet, bool notifications, bool postData,
                 bool ultrasonicSensor, bool floatSensor, bool etapeSensor, int notificationHour, QObject *parent) : QObject(parent)
{
    this->pushMessageSender = new Notifier(notificationHour, this);
    this->sqlDatabase = new PostSQLData(this);
    this->_monitoringInterval = monitoringInterval*1000;
    this->_continuous = continuous;
    this->_verbose = !quiet;
    this->_notifications = notifications;
    this->_postData = postData;
    this->_navg = navg;
    this->_useUltrasonic = ultrasonicSensor;
    this->_useFloat = floatSensor;
    this->_useEtape = etapeSensor;

    if(this->_useFloat)
        this->_floatSensor = new FloatSensor(this);
    if(this->_useUltrasonic)
        this->_ultrasonicSensor = new UltrasonicSensor(this->_navg,this);
    if(this->_useEtape)
        this->_etapeSensor = new EtapeSensor(this);
}


void Monitor::run()
{

    QTextStream out(stdout,QIODevice::WriteOnly);

    if(this->_verbose)
    {
        out << "Starting the monitoring routine...\n";
        out.flush();
    }

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

        //...Call the status routine the first time
        this->checkStatus();

        //...Now call the status routine every _montitoringInterval seconds
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
    double wl;
    bool fl;
    QString message;
    QString title;

    QTextStream out(stdout,QIODevice::WriteOnly);
    
    //...Check the water level in the sump
    if(this->_useUltrasonic)
    {
        wl = this->_ultrasonicSensor->getWaterLevel(ierr);
        if(ierr!=0)
            wl = 0.0;
    }
    else if(this->_useEtape)
    {
        wl = this->_etapeSensor->getWaterLevel(ierr);
        if(ierr!=0)
            wl=0.0;
    }
    else
        wl = 0.0;

    //...Check the basin float status
    if(this->_useFloat)
    {
        fl = this->_floatSensor->getFloatStatus(ierr);
        if(ierr!=0)
            fl = false;
    }
    else
        fl = false;

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
        if(this->_useUltrasonic || this->_useEtape )
            out << QString("|      Water Level: "+wls+" inches                             |\n");
        if(this->_useFloat)
        {
            if(fl)
                out << QString("|     Float Status: Triggered                                 |\n");
            else
                out << QString("|     Float Status: Not Triggered                             |\n");
        }
        out << QString("|-------------------------------------------------------------| \n\n");
        out.flush();
    }

    return;

}


int Monitor::generateStatusMessage(bool floatStatus, double waterLevel, int &priority, QString &title, QString &message)
{
    QString wls;
    wls.sprintf("%5.2f",waterLevel);
    //...Generate the status
    if(floatStatus)
    {
        priority = PRIORITY_EMERGENCY;
        title    = "Sump Level Critical!";
        message  = "CRITICAL! Sump Water Level is "+wls+" inches. Attention is required immediately. The high water float has been toggled.";
    }
    else if(this->_useUltrasonic || this->_useEtape)
    {
        if(waterLevel>15.0)
        {
            priority = PRIORITY_HIGH;
            title    = "Sump Level is Abnormal";
            message  = "Sump pump should be checked immediately. Water level is "+wls+" inches but the high water float has not been toggled yet.";
        }
        else
        {
            priority = PRIORITY_STANDARD;
            title    = "Sump Level is Normal";
            message  = "Sump level is normal. No need for action at this time. Water level is "+wls+" inches and the float has not been toggled.";
        }
    }
    else
    {
        priority = PRIORITY_STANDARD;
        title    = "Sump Level is Normal";
        message  = "Sump level is normal. No need for action at this time.";
    }

    return 0;
}


void Monitor::endMonitor()
{
    emit finished();
    return;
}
