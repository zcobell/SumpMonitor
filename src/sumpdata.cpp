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
