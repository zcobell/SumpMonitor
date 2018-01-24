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
