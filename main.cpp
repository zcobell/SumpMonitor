#include <QCoreApplication>
#include <QTimer>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <stdio.h>
#include "monitor.h"

int main(int argc, char *argv[])
{
    
    bool isSingleSet,isContinuousSet,isQuietSet;
    bool isNotifySet,isPostSet,useUltrasonic,useFloat,useEtape;

    int defaultPollingInterval = 60;
    int defaultAveragingValue  = 5;

    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("SumpMonitor");
    QCoreApplication::setApplicationVersion(GIT_VERSION);
    QCommandLineParser parser;

    QTextStream out(stdout,QIODevice::WriteOnly);

    //...Set up the command line parser
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.setApplicationDescription(
        "Real Time Sump Pump Monitoring Using Raspberry Pi \n\nAuthor: Zach Cobell");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption quietOption(QStringList() << "q" << "quiet",
        "Do not write unnessary outout to the scree");
    QCommandLineOption singleOption(QStringList() << "s" << "single",
        "Run the monitoring rountine only a single time");
    QCommandLineOption continuousOption(QStringList() << "c" << "continuous",
        "Run the monitoring routine continuously as defined by <interval>");
    QCommandLineOption intervalOption(QStringList() << "i" << "interval",
        "Interval in seconds to monitor status when running in continuous mode [default="+
        QString::number(defaultPollingInterval)+"]","seconds");
    QCommandLineOption notifyOption(QStringList() << "n" << "notify",
        "Use the push notification system via PushOver");
    QCommandLineOption postSqlOption(QStringList() << "p" << "post",
        "Post data to a SQL web server");
    QCommandLineOption averagingOption(QStringList() << "a" << "average",
        "Average a number of measurements of water level to reduce noise [default="+
        QString::number(defaultAveragingValue)+"]","n");
    QCommandLineOption ultrasonicOption(QStringList() << "u" << "ultrasonic",
        "Use the ultrasonic sensor to track the water level [default=no]","yes/no");
    QCommandLineOption etapeOption(QStringList() << "e" << "etape", 
        "Use the eTape sensor to track the water level [default=yes]","yes/no");
    QCommandLineOption floatOption(QStringList() << "f" << "float",
        "Use the float sensor to determine when water has exceeded a threshold [default=yes]","yes/no");
    QCommandLineOption notificationHourOption(QStringList() << "t" << "time",
        "Hour of the day to send a daily status confirmation. Useful in determining if the system has gone offline [default=8]","1-24");
    
    intervalOption.setDefaultValue(QString::number(defaultPollingInterval));
    averagingOption.setDefaultValue(QString::number(defaultAveragingValue));
    ultrasonicOption.setDefaultValue("no");
    floatOption.setDefaultValue("yes");
    etapeOption.setDefaultValue("yes");

    notificationHourOption.setDefaultValue("8");

    parser.addOption(quietOption);
    parser.addOption(singleOption);
    parser.addOption(continuousOption);
    parser.addOption(intervalOption);
    parser.addOption(notifyOption);
    parser.addOption(postSqlOption);
    parser.addOption(averagingOption);
    parser.addOption(ultrasonicOption);
    parser.addOption(floatOption);
    parser.addOption(etapeOption);
    parser.addOption(notificationHourOption);

    //...Process command line arguments
    parser.process(a);

    //...Check for no command line arguments. Exit here.
    if(argc==1)
    {
        out << "ERROR: No arguments detected!\n\n";
        out.flush();
        parser.showHelp(1);
    }
    
    //...Find out which options were selected
    isSingleSet     = parser.isSet(singleOption);
    isContinuousSet = parser.isSet(continuousOption);
    isQuietSet      = parser.isSet(quietOption);
    isNotifySet     = parser.isSet(notifyOption);
    isPostSet       = parser.isSet(postSqlOption);

    int navg        = parser.value(averagingOption).toInt();

    if(parser.value(ultrasonicOption)=="yes")
        useUltrasonic = true;
    else
        useUltrasonic = false;

    if(parser.value(floatOption)=="yes")
        useFloat = true;
    else
        useFloat = false;

    if(parser.value(etapeOption)=="yes")
        useEtape = true;
    else
        useEtape = false;

    int hour = parser.value(notificationHourOption).toInt();

    if(isSingleSet&&isContinuousSet)
    {
        out << "ERROR: Only one operation mode may be selected. Either single or continuous\n";
        out.flush();
        return 1;
    }

    if(!isSingleSet&&!isContinuousSet)
    {
        out << "ERROR: No operation mode selected (single or continuous)\n";
        out.flush();
        return 1;
    }

    if(useUltrasonic && useEtape)
    {
        out << "ERROR: You may only use the ETape or the Ultrasonic sensor.\n";
        out.flush();
        return 1;
    }

    if(isSingleSet)
    {
        Monitor *sumpMonitor = new Monitor(0,navg,false,isQuietSet,isNotifySet,isPostSet,
                                           useUltrasonic,useFloat,useEtape,hour,&a);
        QObject::connect(sumpMonitor,SIGNAL(finished()),&a,SLOT(quit()));
        QTimer::singleShot(0,sumpMonitor,SLOT(run()));
        return a.exec();
    }

    if(isContinuousSet)
    {
        int interval = parser.value(intervalOption).toInt();
        Monitor *sumpMonitor = new Monitor(interval,navg,true,isQuietSet,isNotifySet,isPostSet,
                                           useUltrasonic,useFloat,useEtape,hour,&a);
        QObject::connect(sumpMonitor,SIGNAL(finished()),&a,SLOT(quit()));
        QTimer::singleShot(0,sumpMonitor,SLOT(run()));
        return a.exec();
    }


}
