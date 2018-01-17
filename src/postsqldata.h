#ifndef POSTSQLDATA_H
#define POSTSQLDATA_H

#include "sumpdata.h"
#include <QDateTime>
#include <QObject>
#include <QSqlDatabase>
#include <QVector>

class PostSQLData : public QObject {
  Q_OBJECT
public:
  explicit PostSQLData(QObject *parent = 0);
  ~PostSQLData();

signals:

public slots:
  void postData(double waterlevel, bool floatstatus);
  void postData(QVector<SumpData *> &data);
  int initDatabase();
  int closeDatabase();

private:
  bool _postData(QDateTime time, double waterlevel, bool floatstatus);
  int checkDatabaseConnection();
  QSqlDatabase database;
};

#endif // POSTSQLDATA_H
