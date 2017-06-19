#ifndef POSTSQLDATA_H
#define POSTSQLDATA_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include "tokens.h"

class PostSQLData : public QObject
{
    Q_OBJECT
public:
    explicit PostSQLData(QObject *parent = 0);
    ~PostSQLData();

signals:

public slots:
    void postData(double waterlevel, bool floatstatus);

private:

    int initDatabase();
    int closeDatabase();
    QSqlDatabase database;

};

#endif // POSTSQLDATA_H
