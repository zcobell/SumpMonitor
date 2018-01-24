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
#include "notifier.h"
#include "sumpmonitor.h"
#include "tokens.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

Notifier::Notifier(int notificationHour, QObject *parent) : QObject(parent) {
  this->_networkManager = new QNetworkAccessManager(this);
  connect(this->_networkManager, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(onPostAnswer(QNetworkReply *)));

  //...Set a current time that we know we are past as the initial
  //   last notification so we'll always get the first one pushed
  //   to cell phone
  this->_notificationHour = notificationHour;
  this->_lastNotification =
      QDateTime(QDate(2010, 1, 1), QTime(this->_notificationHour, 0, 0));
  this->_lastHighNotification = this->_lastNotification;
  this->_lastCriticalNotification = this->_lastNotification;

  this->_nextNotification = this->_lastNotification;
  this->_nextHighNotification = this->_lastNotification;
  this->_nextCriticalNotification = this->_lastNotification;
}

Notifier::~Notifier() {
  disconnect(this->_networkManager, SIGNAL(finished(QNetworkReply *)), this,
             SLOT(onPostAnswer(QNetworkReply *)));
  this->_networkManager->deleteLater();
}

int Notifier::sendRestartMessage() {
  QUrl url("https://api.pushover.net/1/messages.json");
  QUrlQuery postData;

  postData.addQueryItem("token", APP_TOKEN);
  postData.addQueryItem("user", USER_TOKEN);
  postData.addQueryItem("priority", QString::number(0));
  postData.addQueryItem("title", "Sump Monitor Restarted");
  postData.addQueryItem("message",
                        QString("Sump Monitor was restarted at " +
                                QDateTime::currentDateTime().toString()));

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");
  this->_networkManager->post(request,
                              postData.toString(QUrl::FullyEncoded).toUtf8());
  return 0;
}

int Notifier::sendMessage(int priority, QString title, QString message) {
  if (priority < 1) {
    if (QDateTime::currentDateTime() > this->_nextNotification) {
      this->_lastNotification = this->_nextNotification;
      this->_nextNotification = QDateTime(QDate::currentDate().addDays(1),
                                          QTime(this->_notificationHour, 0, 0));
      this->_sendMessage(priority, title, message);
    } else {
      qDebug() << "INFO: " << QDateTime::currentDateTime() 
               << "Notification supressed until " << this->_nextNotification;
    }
  } else if (priority == 1) {
    if (QDateTime::currentDateTime() > this->_nextHighNotification) {
      this->_lastHighNotification = QDateTime::currentDateTime();
      this->_nextHighNotification = QDateTime::currentDateTime().addSecs(21600);
      this->_sendMessage(priority, title, message);
    } else {
      qDebug() << "INFO: " << QDateTime::currentDateTime() 
               << "High notification supressed until "
               << this->_nextHighNotification;
    }
  } else if (priority == 2) {
    if (QDateTime::currentDateTime() > this->_nextCriticalNotification) {
      this->_lastCriticalNotification = QDateTime::currentDateTime();
      this->_nextCriticalNotification =
          QDateTime::currentDateTime().addSecs(3600);
      this->_sendMessage(priority, title, message);
    } else {
      qDebug() << "INFO: " << QDateTime::currentDateTime() 
               << "Critical notification supressed until "
               << this->_nextCriticalNotification;
    }
  }
  return 0;
}

int Notifier::_sendMessage(int priority, QString title, QString message) {
  QUrl url("https://api.pushover.net/1/messages.json");
  QUrlQuery postData;

  postData.addQueryItem("token", APP_TOKEN);
  postData.addQueryItem("user", USER_TOKEN);
  postData.addQueryItem("priority", QString::number(priority));
  postData.addQueryItem("title", title);
  postData.addQueryItem("message", message);

  if (priority == 2) {
    postData.addQueryItem("retry", QString::number(Emergency::Retry));
    postData.addQueryItem("expire", QString::number(Emergency::Expire));
  }

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");
  this->_networkManager->post(request,
                              postData.toString(QUrl::FullyEncoded).toUtf8());
  return 0;
}

void Notifier::onPostAnswer(QNetworkReply *reply) {
  Q_UNUSED(reply);
  emit finished();
  return;
}
