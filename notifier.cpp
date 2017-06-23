#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "notifier.h"
#include "tokens.h"

Notifier::Notifier(int notificationHour, QObject *parent) : QObject(parent)
{
    this->_networkManager = new QNetworkAccessManager(this);
    connect(this->_networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onPostAnswer(QNetworkReply*)));

    //...Set a current time that we know we are past as the initial
    //   last notification so we'll always get the first one pushed
    //   to cell phone
    this->_notificationHour = notificationHour;
    this->_lastNotification = QDateTime(QDate(2010,1,1),QTime(this->_notificationHour,0,0));
    this->_lastHighNotification = this->_lastNotification;
    this->_lastCriticalNotification = this->_lastNotification;

    this->_nextNotification = this->_lastNotification;
    this->_nextHighNotification = this->_lastNotification;
    this->_nextCriticalNotification = this->_lastNotification;

}

Notifier::~Notifier()
{
    disconnect(this->_networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onPostAnswer(QNetworkReply*)));
    this->_networkManager->deleteLater();
}

int Notifier::sendRestartMessage()
{
    QUrl url("https://api.pushover.net/1/messages.json");
    QUrlQuery postData;

    postData.addQueryItem("token",APP_TOKEN);
    postData.addQueryItem("user",USER_TOKEN);
    postData.addQueryItem("priority",QString::number(0));
    postData.addQueryItem("title","Sump Monitor Restarted");
    postData.addQueryItem("message",QString("Sump Monitor was restarted at "+QDateTime::currentDateTime().toString()));

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    this->_networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    return 0;
}

int Notifier::sendMessage(int priority, QString title, QString message)
{
    if(priority<1)
    {
        if(QDateTime::currentDateTime()>this->_nextNotification)
        {
            this->_lastNotification = this->_nextNotification;
            this->_nextNotification = QDateTime(QDate::currentDate().addDays(1),QTime(this->_notificationHour,0,0));
            this->_sendMessage(priority,title,message);
        }
        else
        {
            qDebug() << "Notification supressed until " << this->_nextNotification;
        }
    }
    else if(priority==1)
    {
        if(QDateTime::currentDateTime()>this->_nextHighNotification)
        {
            this->_lastHighNotification = QDateTime::currentDateTime();
            this->_nextHighNotification = QDateTime::currentDateTime().addSecs(21600);
            this->_sendMessage(priority,title,message);
        }
        else
        {
            qDebug() << "High notification supressed until " << this->_nextHighNotification;
        }
    }
    else if(priority==2)
    {
        if(QDateTime::currentDateTime()>this->_nextCriticalNotification)
        {
            this->_lastCriticalNotification = QDateTime::currentDateTime();
            this->_nextCriticalNotification = QDateTime::currentDateTime().addSecs(3600);
            this->_sendMessage(priority,title,message);
        }
        else
        {
            qDebug() << "Critical notification supressed until " << this->_nextCriticalNotification;
        }
    }
    return 0;
}

int Notifier::_sendMessage(int priority, QString title, QString message)
{
    QUrl url("https://api.pushover.net/1/messages.json");
    QUrlQuery postData;

    postData.addQueryItem("token",APP_TOKEN);
    postData.addQueryItem("user",USER_TOKEN);
    postData.addQueryItem("priority",QString::number(priority));
    postData.addQueryItem("title",title);
    postData.addQueryItem("message",message);

    if(priority==2)
    {
        postData.addQueryItem("retry",QString::number(EMERGENCY_RETRY));
        postData.addQueryItem("expire",QString::number(EMERGENCY_EXPIRE));
    }

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    this->_networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    return 0;
}

void Notifier::onPostAnswer(QNetworkReply *reply)
{
    emit finished();
    return;
}
