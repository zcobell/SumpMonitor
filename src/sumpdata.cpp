#include "sumpdata.h"

SumpData::SumpData(QObject *parent) : QObject(parent) {
  this->m_waterLevel = 0.0;
  this->m_floatStatus = false;
  this->m_time = QDateTime();
}

SumpData::SumpData(QDateTime time, double waterlevel, bool floatstatus,
                   QObject *parent)
    : QObject(parent) {
  this->m_waterLevel = waterlevel;
  this->m_floatStatus = floatstatus;
  this->m_time = time;
  return;
}

SumpData::~SumpData() {}

void SumpData::setWaterLevel(double level) {
  this->m_waterLevel = level;
  return;
}

double SumpData::waterLevel() const { return this->m_waterLevel; }

void SumpData::setFloatStatus(bool status) { this->m_floatStatus = status; }

bool SumpData::floatStatus() const { return this->m_floatStatus; }

QDateTime SumpData::time() const { return this->m_time; }

void SumpData::setTime(QDateTime time) { this->m_time = time; }
