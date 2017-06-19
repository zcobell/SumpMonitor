#ifndef POSTSQLDATA_H
#define POSTSQLDATA_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>

#define SERVER "***REMOVED***"
#define DBNAME "***REMOVED***"
#define USERNAME "***REMOVED***"
#define PASSWORD "***REMOVED***"

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
