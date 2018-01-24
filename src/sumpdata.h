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
#ifndef SUMPDATA_H
#define SUMPDATA_H

#include <QDateTime>
#include <QObject>

class SumpData : public QObject {
  Q_OBJECT
public:
  explicit SumpData(QObject *parent = nullptr);

  explicit SumpData(QDateTime time, double waterlevel, bool floatstatus,
                    QObject *parent = nullptr);

  ~SumpData();

  double waterLevel() const;
  void setWaterLevel(double level);

  bool floatStatus() const;
  void setFloatStatus(bool status);

  QDateTime time() const;
  void setTime(QDateTime time);

private:
  double m_waterLevel;
  bool m_floatStatus;
  QDateTime m_time;
};

#endif // SUMPDATA_H
