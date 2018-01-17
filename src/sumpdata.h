#ifndef SUMPDATA_H
#define SUMPDATA_H

#include <QObject>
#include <QDateTime>

class SumpData : public QObject {
    Q_OBJECT
public:
    explicit SumpData(QObject *parent = nullptr);

    explicit SumpData(QDateTime time, double waterlevel, bool floatstatus, QObject *parent = nullptr);
    
    ~SumpData();

    double waterLevel() const;
    void setWaterLevel(double level);

    bool floatStatus() const;
    void setFloatStatus(bool status);

    QDateTime time() const;
    void setTime(QDateTime time);

private:

    double m_waterLevel;
    bool   m_floatStatus;
    QDateTime m_time;

};

#endif // SUMPDATA_H
