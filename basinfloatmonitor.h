#ifndef BASINFLOATMONITOR_H
#define BASINFLOATMONITOR_H

#include <QObject>

class BasinFloatMonitor : public QObject
{
    Q_OBJECT
public:
    explicit BasinFloatMonitor(QObject *parent = 0);

    bool getFloatStatus(int &ierr);

signals:

public slots:

private:
    int _floatPin = 4;

};

#endif // BASINFLOATMONITOR_H
