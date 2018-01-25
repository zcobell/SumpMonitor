//------------------------------GPLv3------------------------------------//
//
//  This file is part of SumpMonitor.
//
//  SumpMonitor is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  SumpMonitor is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SumpMonitor. If not, see <http://www.gnu.org/licenses/>.
//
//------------------------------GPLv3------------------------------------//
#include "monitor.h"
#include "etapeSensor.h"
#include "floatSensor.h"
#include "network.h"
#include "sumpmonitor.h"
#include "tokens.h"
#include "ultrasonicSensor.h"
#include <QObject>
#include <QTimer>
#include "sensorlevel.h"
#include <stdio.h>

Monitor::Monitor(int monitoringInterval, int nsamples, bool continuous,
                 bool quiet, bool notifications, bool postData,
                 bool ultrasonicSensor, bool floatSensor, bool etapeSensor,
                 int notificationHour, QObject *parent)
    : QObject(parent) {
  this->pushMessageSender = new Notifier(notificationHour, this);
  this->sqlDatabase = new PostSQLData(this);
  this->_monitoringInterval = monitoringInterval * 1000;
  this->_continuous = continuous;
  this->_verbose = !quiet;
  this->_notifications = notifications;
  this->_postData = postData;
  this->_nsamples = nsamples;
  this->_useUltrasonic = ultrasonicSensor;
  this->_useFloat = floatSensor;
  this->_useEtape = etapeSensor;

  if (this->_useFloat)
    this->_floatSensor = new FloatSensor(this);
  if (this->_useUltrasonic)
    this->_ultrasonicSensor = new UltrasonicSensor(this->_nsamples, this);
  if (this->_useEtape)
    this->_etapeSensor = new EtapeSensor(this->_nsamples, this);
}

void Monitor::run() {

  QTextStream out(stdout, QIODevice::WriteOnly);

  if (this->_verbose) {
    out << "Starting the monitoring routine...\n";
    out.flush();
  }

  //...Alert the user that the monitor was restarted
  if (this->_notifications && this->_restartNotifications)
    this->pushMessageSender->sendRestartMessage();

  //...Begin the code to fire every _monitoringInterval
  //   milliseconds which will probe the status of the
  //   sump
  if (this->_continuous) {
    QTimer *monitorTimer = new QTimer(this);
    connect(monitorTimer, SIGNAL(timeout()), this, SLOT(checkStatus()));
    connect(this, SIGNAL(monitorError()), monitorTimer, SLOT(stop()));

    //...Call the status routine the first time
    this->checkStatus();

    //...Now call the status routine every _montitoringInterval seconds
    monitorTimer->start(this->_monitoringInterval);
  } else {
    this->checkStatus();
    this->endMonitor();
  }
}

void Monitor::checkStatus() {
  int ierr;
  int priority;
  double wl;
  bool fl;
  QString message;
  QString title;

  QTextStream out(stdout, QIODevice::WriteOnly);

  //...Check the water level in the sump
  if (this->_useUltrasonic) {
    wl = this->_ultrasonicSensor->getWaterLevel(ierr);
    if (ierr != 0)
      wl = 0.0;
  } else if (this->_useEtape) {
    wl = this->_etapeSensor->getWaterLevel(ierr);
    if (ierr != 0)
      wl = 0.0;
  } else
    wl = 0.0;

  //...Check the basin float status
  if (this->_useFloat) {
    fl = this->_floatSensor->getFloatStatus(ierr);
    if (ierr != 0)
      fl = false;
  } else
    fl = false;

  //...Post data to SQL database on web server
  if (this->_postData) {

    //...Generate the data for posting
    this->_monitorData.push_back(
        new SumpData(QDateTime::currentDateTime(), wl, fl));

    //...Check if data can be posted
    if (Network::isUp("http://"+QString(SERVER))) {
      int sizeBefore = this->_monitorData.size();
      this->sqlDatabase->initDatabase();
      this->sqlDatabase->postData(this->_monitorData);
      this->sqlDatabase->closeDatabase();
      int sizeAfter = this->_monitorData.size();

      if(sizeBefore > 1 && sizeAfter == 0){
          out << "INFO: Server returned to service at " << QDateTime::currentDateTime().toString("MM/dd/yyyy hh:mm:ss") << "\n";
          out << "      " << sizeBefore << " datasets were successfully posted.\n"; 
          out.flush();
      }

    } else {
      out << "ERROR: Server down at " << QDateTime::currentDateTime().toString("MM/dd/yyyy hh:mm:ss") << "\n";
      out << "       dataPostQueueSize: " << this->_monitorData.size() << "\n";
      out.flush();
    }
  }

  //...Generate the status messages
  if (this->_notifications) {
    if(Network::isUp()){
        this->generateStatusMessage(fl, wl, priority, title, message);

        //...Send the message
        ierr = this->pushMessageSender->sendMessage(priority, title, message);

        if (ierr != 0)
          endMonitor();
    }
  }

  //...Log message to screen
  if (this->_verbose) {
    QString wls;
    wls.sprintf("%6.2f", wl);
    out << QString(
        "|-------------------------------------------------------------| \n");
    out << QString(
        "|  Sump Status @ " +
        QDateTime::currentDateTime().toString("MM/dd/yyyy hh:mm:ss") +
        "                          |\n");
    out << QString(
        "|                                                             |\n");
    if (this->_useUltrasonic || this->_useEtape)
      out << QString("|      Water Level: " + wls +
                     " inches                             |\n");
    if (this->_useFloat) {
      if (fl)
        out << QString("|     Float Status: Triggered                          "
                       "       |\n");
      else
        out << QString("|     Float Status: Not Triggered                      "
                       "       |\n");
    }
    out << QString(
        "|-------------------------------------------------------------| \n\n");
    out.flush();
  }

  return;
}

int Monitor::generateStatusMessage(bool floatStatus, double waterLevel,
                                   int &priority, QString &title,
                                   QString &message) {
  QString wls;
  wls.sprintf("%5.2f", waterLevel);
  
  //...Use float as first and most critical
  if( this->_useFloat ){
      if (floatStatus) {
        priority = Priority::Emergency;
        title = "Sump Level Critical!";
        if (this->_useUltrasonic || this->_useEtape) {
            message = "CRITICAL! Sump Water Level is " + wls +
                      " inches. Attention is required immediately. The high water "
                      "float has been toggled.";
        } else {
            message = "CRITICAL! The high water float has been toggled. "
                      "Attention required immediately.";
        }
        return 0;
      }
  } else if ( !this->_useUltrasonic && !this->_useEtape ) {
      priority = Priority::Standard;
      title = "Sump Level is Normal.";
      message = message + " The high water float has not been toggled yet.";
      return 0;
  }

  //...Continue to water level sensors
  if(this->_useEtape || this->_useUltrasonic){

    double waterLevelHigh,waterLevelCritical;

    if(this->_useEtape){
        waterLevelCritical = SensorLevel::eTapeEmergencyLevel;
        waterLevelHigh = SensorLevel::eTapeHighLevel;
    } else {
        waterLevelCritical = SensorLevel::ultrasonicEmergencyLevel;
        waterLevelHigh = SensorLevel::ultrasonicHighLevel;
    }

    if ( waterLevel >= waterLevelCritical ) {
      priority = Priority::Emergency;
      title = "Sump Level is Critical!";
      message = "Sump pump should be checked immediately. Water level is " +
                wls + " inches.";
      if(this->_useFloat)
          message = message + " The high water float has not been toggled yet.";
      return 0;
    } else if (waterLevel >= waterLevelHigh ) {
      priority = Priority::High;
      title = "Sump Level is Abnormal";
      message = "Sump pump should be checked immediately. Water level is " +
                wls + " inches.";
      if(this->_useFloat)
          message = message + " The high water float has not been toggled yet.";
      return 0;
    } else {
      priority = Priority::Standard;
      title = "Sump Level is Normal";
      message = "Sump level is normal. No need for action at this time. Water "
                "level is " + wls + " inches.";
      if(this->_useFloat)
          message = message + " The high water float has not been toggled.";
      return 0;
    }

  }
  
  priority = Priority::Standard;
  title = "Sump Level is Unknown";
  message = "No sensors are active.";
  
  return 0;
}

void Monitor::endMonitor() {
  emit finished();
  return;
}
