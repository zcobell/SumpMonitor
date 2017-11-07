#ifndef SENDNOTIFICATION_H
#define SENDNOTIFICATION_H

#include <QDateTime>
#include <QNetworkAccessManager>
#include <QObject>

class Notifier : public QObject {
  Q_OBJECT
public:
  explicit Notifier(int notificationHour, QObject *parent = 0);
  ~Notifier();

  int sendMessage(int priority, QString title, QString message);

  int sendRestartMessage();

signals:
  void finished();

public slots:
  void onPostAnswer(QNetworkReply *reply);

private:
  QNetworkAccessManager *_networkManager;
  QDateTime _lastNotification, _lastHighNotification, _lastCriticalNotification;
  QDateTime _nextNotification, _nextHighNotification, _nextCriticalNotification;
  int _notificationHour;

  int _sendMessage(int priority, QString title, QString message);
};

#endif // SENDNOTIFICATION_H
