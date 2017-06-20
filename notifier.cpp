#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "notifier.h"
#include "tokens.h"

Notifier::Notifier(QObject *parent) : QObject(parent)
{
    this->networkManager = new QNetworkAccessManager(this);
    connect(this->networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onPostAnswer(QNetworkReply*)));

    //...Set a current time that we know we are past as the initial
    //   last notification so we'll always get the first one pushed
    //   to cell phone
    this->lastNotification = QDateTime(QDate(2010,1,1),QTime(this->notificationHour,0,0));
    this->lastHighNotification = this->lastNotification;
    this->lastCriticalNotification = this->lastNotification;

    this->nextNotification = this->lastNotification;
    this->nextHighNotification = this->lastNotification;
    this->nextCriticalNotification = this->lastNotification;

}

Notifier::~Notifier()
{
    this->networkManager->deleteLater();
    disconnect(this->networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onPostAnswer(QNetworkReply*)));
}

#if NOTIFIER_NOSEND == 1
int Notifier::sendRestartMessage()
{
    return 0;
}

int Notifier::sendMessage(int priority, QString title, QString message)
{
    return 0;
}
#else
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
    this->networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    return 0;
}

int Notifier::sendMessage(int priority, QString title, QString message)
{
    if(priority<1)
    {
        if(QDateTime::currentDateTime()>this->nextNotification)
        {
            this->lastNotification = this->nextNotification;
            this->nextNotification = QDateTime(QDate::currentDate().addDays(1),QTime(this->notificationHour,0,0));
            this->_sendMessage(priority,title,message);
        }
        else
        {
            qDebug() << "Notification supressed until " << this->nextNotification;
        }
    }
    else if(priority==1)
    {
        if(QDateTime::currentDateTime()>this->nextHighNotification)
        {
            this->lastHighNotification = QDateTime::currentDateTime();
            this->nextHighNotification = QDateTime::currentDateTime().addSecs(21600);
            this->_sendMessage(priority,title,message);
        }
        else
        {
            qDebug() << "High notification supressed until " << this->nextHighNotification;
        }
    }
    else if(priority==2)
    {
        if(QDateTime::currentDateTime()>this->nextCriticalNotification)
        {
            this->lastCriticalNotification = QDateTime::currentDateTime();
            this->nextCriticalNotification = QDateTime::currentDateTime().addSecs(3600);
            this->_sendMessage(priority,title,message);
        }
        else
        {
            qDebug() << "Critical notification supressed until " << this->nextCriticalNotification;
        }
    }
    return 0;
}
#endif

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
    this->networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    return 0;
}

void Notifier::onPostAnswer(QNetworkReply *reply)
{
    emit finished();
    return;
}
