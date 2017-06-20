#include <QCoreApplication>
#include <QTimer>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <stdio.h>
#include "monitor.h"

int main(int argc, char *argv[])
{
    
    bool isSingleSet,isContinuousSet,isVerboseSet;
    bool isNotifySet,isPostSet;

    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("SumpMonitor");
    QCoreApplication::setApplicationVersion(GIT_VERSION);
    QCommandLineParser parser;

    QTextStream out(stdout,QIODevice::WriteOnly);

    //...Set up the command line parser
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.setApplicationDescription("Real Time Sump Pump Monitoring Using Raspberry Pi \nAuthor: Zach Cobell");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption singleOption(QStringList() << "s" << "single","Run the monitoring rountine only a single time");
    QCommandLineOption continuousOption(QStringList() << "c" << "continuous","Run the monitoring routine continuously as defined by <interval>");
    QCommandLineOption intervalOption(QStringList() << "i" << "interval","Interval in seconds to monitor status when running in continuous mode [default=60]","seconds");
    QCommandLineOption verboseOption(QStringList() << "q" << "verbose","Write verbose output to screen");
    QCommandLineOption notifyOption(QStringList() << "n" << "notify","Use the push notification system via PushOver");
    QCommandLineOption postSqlOption(QStringList() << "p" << "post","Use the post option to post data to a web SQL server");
    QCommandLineOption averagingOption(QStringList() << "a" << "average","Average a number of measurements of water level to reduce noise [default=5]","n");
    
    intervalOption.setDefaultValue("60");
    averagingOption.setDefaultValue("5");

    parser.addOption(singleOption);
    parser.addOption(continuousOption);
    parser.addOption(intervalOption);
    parser.addOption(verboseOption);
    parser.addOption(notifyOption);
    parser.addOption(postSqlOption);
    parser.addOption(averagingOption);

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
    isVerboseSet    = parser.isSet(verboseOption);
    isNotifySet     = parser.isSet(notifyOption);
    isPostSet       = parser.isSet(postSqlOption);

    int navg        = parser.value(averagingOption).toInt();

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

    if(isSingleSet)
    {
        Monitor *sumpMonitor = new Monitor(0,navg,false,isVerboseSet,isNotifySet,isPostSet,&a);
        QObject::connect(sumpMonitor,SIGNAL(finished()),&a,SLOT(quit()));
        QTimer::singleShot(0,sumpMonitor,SLOT(run()));
        return a.exec();
    }

    if(isContinuousSet)
    {
        int interval = parser.value(intervalOption).toInt();
        Monitor *sumpMonitor = new Monitor(interval,navg,true,isVerboseSet,isNotifySet,isPostSet,&a);
        QObject::connect(sumpMonitor,SIGNAL(finished()),&a,SLOT(quit()));
        QTimer::singleShot(0,sumpMonitor,SLOT(run()));
        return a.exec();
    }


}
