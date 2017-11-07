#include "postsqldata.h"
#include "tokens.h"
#include <QDebug>
#include <QtSql>

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

void PostSQLData::postData(double waterlevel, bool floatstatus) {
  QString sqlString, sqlData;
  QString wl, fs;
  int ierr;

  ierr = this->checkDatabaseConnection();
  if (ierr == 1) {
    ierr = this->closeDatabase();
    ierr = this->initDatabase();
    if (ierr != 0)
      exit(EXIT_FAILURE);
    return;
  }

  wl.sprintf("%0.4f", waterlevel);

  if (floatstatus)
    fs = "TRUE";
  else
    fs = "FALSE";

  sqlData = "(NULL, NOW(), '" + wl + "', " + fs + ")";

  sqlString = QString("INSERT INTO `***REMOVED***`.`sumpData` "
                      "(`id`, `time`, `waterlevel`, `floatstatus`) VALUES" +
                      sqlData + ";");

  this->database.exec(sqlString);
  if (this->database.lastError().type() != QSqlError::NoError) {
    qCritical() << "ERROR: Could not post data to server.";
    exit(EXIT_FAILURE);
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
