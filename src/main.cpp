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
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTimer>
#include <stdio.h>

int main(int argc, char *argv[]) {

  bool isSingleSet, isContinuousSet, isQuietSet;
  bool isNotifySet, isPostSet, isFloatSet, isEtapeSet, isUltrasonicSet;
  bool writeNetcdfOutput;

  QString netcdfFilename;

  int defaultPollingInterval = 60;
  int defaultSamplingValue = 100;

  QCoreApplication a(argc, argv);

  QCoreApplication::setApplicationName("SumpMonitor");
  QCoreApplication::setApplicationVersion(GIT_VERSION);
  QCommandLineParser parser;

  QTextStream out(stdout, QIODevice::WriteOnly);

  //...Set up the command line parser
  parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
  parser.setApplicationDescription("Real Time Sump Pump Monitoring Using "
                                   "Raspberry Pi \n\nAuthor: Zach Cobell");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption quietOption(QStringList() << "q"
                                               << "quiet",
                                 "Do not write unnessary outout to the scree");
  QCommandLineOption singleOption(
      QStringList() << "single",
      "Run the monitoring rountine only a single time");
  QCommandLineOption continuousOption(
      QStringList() << "continuous",
      "Run the monitoring routine continuously as defined by <interval>");
  QCommandLineOption intervalOption(
      QStringList() << "interval",
      "Interval in seconds to monitor status when running in continuous mode "
      "[default=" +
          QString::number(defaultPollingInterval) + "]",
      "seconds");
  QCommandLineOption notifyOption(
      QStringList() << "push", "Use the push notification system via PushOver");
  QCommandLineOption postSqlOption(QStringList() << "sql",
                                   "Post data to a SQL web server");
  QCommandLineOption samplingOption(
      QStringList() << "samples",
      "Number of samples to use to reduce sensor noise [default=" +
          QString::number(defaultSamplingValue) + "]",
      "n");
  QCommandLineOption ultrasonicOption(
      QStringList() << "ultrasonic",
      "Use the ultrasonic sensor to track the water level");
  QCommandLineOption etapeOption(
      QStringList() << "etape",
      "Use the eTape sensor to track the water level");
  QCommandLineOption floatOption(
      QStringList() << "float",
      "Use the float sensor to determine when water has exceeded a threshold");
  QCommandLineOption notificationHourOption(
      QStringList() << "time",
      "Hour of the day to send a daily status confirmation. Useful in "
      "determining if the system has gone offline [default=8]",
      "1-24");
  QCommandLineOption netcdfOption(QStringList() << "netcdf","Write a netcdf output file with the data","filename");

  intervalOption.setDefaultValue(QString::number(defaultPollingInterval));
  samplingOption.setDefaultValue(QString::number(defaultSamplingValue));

  notificationHourOption.setDefaultValue("8");

  parser.addOption(quietOption);
  parser.addOption(singleOption);
  parser.addOption(continuousOption);
  parser.addOption(intervalOption);
  parser.addOption(notifyOption);
  parser.addOption(postSqlOption);
  parser.addOption(samplingOption);
  parser.addOption(floatOption);
  parser.addOption(ultrasonicOption);
  parser.addOption(etapeOption);
  parser.addOption(notificationHourOption);
  parser.addOption(netcdfOption);

  //...Process command line arguments
  parser.process(a);

  //...Check for no command line arguments. Exit here.
  if (argc == 1) {
    out << "ERROR: No arguments detected!\n\n";
    out.flush();
    parser.showHelp(1);
  }

  //...Find out which options were selected
  isSingleSet = parser.isSet(singleOption);
  isContinuousSet = parser.isSet(continuousOption);
  isQuietSet = parser.isSet(quietOption);
  isNotifySet = parser.isSet(notifyOption);
  isPostSet = parser.isSet(postSqlOption);
  isFloatSet = parser.isSet(floatOption);
  isEtapeSet = parser.isSet(etapeOption);
  isUltrasonicSet = parser.isSet(ultrasonicOption);
  writeNetcdfOutput = parser.isSet(netcdfOption);
  if(writeNetcdfOutput)
      netcdfFilename = parser.value(netcdfOption);
  else
      netcdfFilename = QString();

  int nsamples = parser.value(samplingOption).toInt();

  if (isEtapeSet && isUltrasonicSet) {
    out << "ERROR: You may only use the ultrasonic sensor or the eTape sensor.";
    out.flush();
    return 1;
  }

  int hour = parser.value(notificationHourOption).toInt();

  if (isSingleSet && isContinuousSet) {
    out << "ERROR: Only one operation mode may be selected. Either single or "
           "continuous\n";
    out.flush();
    return 1;
  }

  if (!isSingleSet && !isContinuousSet) {
    out << "ERROR: No operation mode selected (single or continuous)\n";
    out.flush();
    return 1;
  }

  if (isSingleSet) {
    Monitor *sumpMonitor =
        new Monitor(0, nsamples, false, isQuietSet, isNotifySet, isPostSet,
                    isUltrasonicSet, isFloatSet, isEtapeSet, hour, writeNetcdfOutput,
                    netcdfFilename, &a);
    QObject::connect(sumpMonitor, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, sumpMonitor, SLOT(run()));
    return a.exec();
  }

  if (isContinuousSet) {
    int interval = parser.value(intervalOption).toInt();
    Monitor *sumpMonitor = new Monitor(interval, nsamples, true, isQuietSet,
                                       isNotifySet, isPostSet, isUltrasonicSet,
                                       isFloatSet, isEtapeSet, hour, writeNetcdfOutput,
                                       netcdfFilename, &a);
    QObject::connect(sumpMonitor, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, sumpMonitor, SLOT(run()));
    return a.exec();
  }
}
