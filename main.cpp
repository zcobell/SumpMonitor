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
    QCoreApplication::setApplicationVersion("0.0.1");
    QCommandLineParser parser;

    QTextStream out(stdout,QIODevice::WriteOnly);

    //...Set up the command line parser
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.setApplicationDescription("Real Time Sump Pump Monitoring Using Raspberry Pi \nAuthor: Zach Cobell");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption singleOption({{"s","single"},"Run the monitoring rountine only a single time"});
    QCommandLineOption continuousOption({{"c","continuous"},"Run the monitoring routine continuously as defined by <interval>"});
    QCommandLineOption intervalOption({{"i","interval"},"Interval in seconds to monitor status when running in continuous mode","seconds"});
    QCommandLineOption verboseOption({{"q","verbose"},"Write verbose output to screen"});
    QCommandLineOption notifyOption({{"n","notify"},"Use the push notification system via PushOver"});
    QCommandLineOption postSqlOption({{"p","post"},"Use the post option to post data to a web SQL server"});
    
    intervalOption.setDefaultValue("60");

    parser.addOption(singleOption);
    parser.addOption(continuousOption);
    parser.addOption(intervalOption);
    parser.addOption(verboseOption);
    parser.addOption(notifyOption);
    parser.addOption(postSqlOption);

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
        Monitor *sumpMonitor = new Monitor(0,false,isVerboseSet,isNotifySet,isPostSet,&a);
        QObject::connect(sumpMonitor,SIGNAL(finished()),&a,SLOT(quit()));
        QTimer::singleShot(0,sumpMonitor,SLOT(run()));
        return a.exec();
    }

    if(isContinuousSet)
    {
        int interval = parser.value(intervalOption).toInt();
        Monitor *sumpMonitor = new Monitor(interval,true,isVerboseSet,isNotifySet,isPostSet,&a);
        QObject::connect(sumpMonitor,SIGNAL(finished()),&a,SLOT(quit()));
        QTimer::singleShot(0,sumpMonitor,SLOT(run()));
        return a.exec();
    }


}
