#ifndef SENDNOTIFICATION_H
#define SENDNOTIFICATION_H

#include <QObject>
#include <QDateTime>
#include <QNetworkAccessManager>

#define PRIORITY_LOWEST    -2
#define PRIORITY_LOW       -1
#define PRIORITY_STANDARD   0
#define PRIORITY_HIGH       1
#define PRIORITY_EMERGENCY  2

#define EMERGENCY_RETRY     600
#define EMERGENCY_EXPIRE    86400

class Notifier : public QObject
{
    Q_OBJECT
public:
    explicit Notifier(QObject *parent = 0);
    ~Notifier();

    int sendMessage(int priority, QString title, QString message);

    int sendRestartMessage();

signals:
    void finished();

public slots:
    void onPostAnswer(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QDateTime              lastNotification,lastHighNotification,lastCriticalNotification;
    QDateTime              nextNotification,nextHighNotification,nextCriticalNotification;
    int                    notificationHour = 19;

    int _sendMessage(int priority, QString title, QString message);


};

#endif // SENDNOTIFICATION_H
