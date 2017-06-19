#include <QCoreApplication>
#include <QTimer>
#include "monitor.h"
#include "postsqldata.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Monitor *sumpMonitor = new Monitor(&a);
    QObject::connect(sumpMonitor,SIGNAL(finished()),&a,SLOT(quit()));
    QTimer::singleShot(0,sumpMonitor,SLOT(run()));
    return a.exec();
}
