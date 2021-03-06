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
#include "postsqldata.h"
#include <QtSql>
#include "tokens.h"

PostSQLData::PostSQLData(QObject *parent) : QObject(parent) {}

PostSQLData::~PostSQLData() {}

int PostSQLData::initDatabase() {
  this->database = QSqlDatabase::addDatabase("QMYSQL");
  this->database.setHostName(SERVER);
  this->database.setDatabaseName(DBNAME);
  this->database.setUserName(USERNAME);
  this->database.setPassword(PASSWORD);
  if (!this->database.open()) {
    qCritical() << "Error Opening database connection: "
                << this->database.lastError();
    return -1;
  }
  return 0;
}

int PostSQLData::closeDatabase() {
  QString connection = this->database.connectionName();
  this->database.close();
  this->database = QSqlDatabase();
  this->database.removeDatabase(connection);
  return 0;
}

bool PostSQLData::_postData(QDateTime time, double waterlevel,
                            bool floatstatus) {
  QString sqlString, sqlData;
  QString wl, fs, ts;
  int ierr;

  ierr = this->checkDatabaseConnection();
  if (ierr == 1) {
    ierr = this->closeDatabase();
    ierr = this->initDatabase();
    if (ierr != 0) return false;
  }

  wl.sprintf("%0.4f", waterlevel);

  if (floatstatus)
    fs = "TRUE";
  else
    fs = "FALSE";

  ts = time.toString("yyyy-MM-dd hh:mm:ss");

  sqlData = "(NULL, '" + ts + "', '" + wl + "', " + fs + ")";

  sqlString =
      QString("INSERT INTO `" + QString(DBNAME) + "`.`" + QString(TABLENAME) +
              "` "
              "(`id`, `time`, `waterlevel`, `floatstatus`) VALUES" +
              sqlData + ";");

  this->database.exec(sqlString);
  if (this->database.lastError().type() != QSqlError::NoError) {
    return false;
  }

  return true;
}

void PostSQLData::postData(double waterlevel, bool floatstatus) {
  this->_postData(QDateTime::currentDateTime(), waterlevel, floatstatus);
  return;
}

void PostSQLData::postData(QVector<SumpData *> &data) {
  QVector<bool> dataPosted;

  dataPosted.resize(data.size());
  std::fill(dataPosted.begin(), dataPosted.end(), false);

  for (int i = 0; i < data.size(); i++)
    dataPosted[i] =
        this->_postData(data.at(i)->time(), data.at(i)->waterLevel(),
                        data.at(i)->floatStatus());

  for (int i = dataPosted.size() - 1; i >= 0; i--)
    if (dataPosted[i]) {
      delete data.at(i);
      data.removeAt(i);
    }

  return;
}

int PostSQLData::checkDatabaseConnection() {
  this->database.exec("SELECT 1;");

  if (this->database.lastError().type() == QSqlError::NoError)
    return 0;
  else
    return -1;
}
