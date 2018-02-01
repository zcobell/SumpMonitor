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
#ifndef NETCDFDATA_H
#define NETCDFDATA_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include "sumpdata.h"

class NetcdfData : public QObject {
  Q_OBJECT

public: 
  NetcdfData(QString filename, bool useFloat, bool useEtape, bool useUltrasonic, QObject *parent = 0);

  ~NetcdfData();

  bool initialize();

  bool writeToFile(SumpData *data);

private:

  bool _open();
  bool _close();
  bool _createFile();
  bool _checkFileFormat();
  QString _getCurrentUserName();

  QString m_filename;

  bool m_clobber = true;

  bool m_useFloat;
  bool m_useEtape;
  bool m_useUltrasonic;

  int m_ncid;
  int m_varidetape;
  int m_varidfloat;
  int m_varidultrasonic;
  int m_varidtime;
  int m_dimidtime;

  const QDateTime m_referenceDate = QDateTime(QDate(1970,1,1),QTime(0,0,0));

};

#endif //NETCDFDATA_H
