#ifndef POSTSQLDATA_H
#define POSTSQLDATA_H

#include <QObject>
#include <QSqlDatabase>

class PostSQLData : public QObject
{
    Q_OBJECT
public:
    explicit PostSQLData(QObject *parent = 0);
    ~PostSQLData();

signals:

public slots:
    void postData(double waterlevel, bool floatstatus);
    int initDatabase();
    int closeDatabase();

private:

    int checkDatabaseConnection();
    QSqlDatabase database;

};

#endif // POSTSQLDATA_H
