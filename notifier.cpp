#include "notifier.h"

#include <QDebug>

Notifier::Notifier(QObject *parent) : QObject(parent)
{
    this->networkManager = new QNetworkAccessManager(this);
    connect(this->networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onPostAnswer(QNetworkReply*)));
}

Notifier::~Notifier()
{
    this->networkManager->deleteLater();
    disconnect(this->networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onPostAnswer(QNetworkReply*)));
}

int Notifier::sendMessage(int priority, QString title, QString message)
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

}

void Notifier::onPostAnswer(QNetworkReply *reply)
{
    emit finished();
    return;
}
