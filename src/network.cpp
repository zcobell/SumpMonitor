
#include "network.h"

bool Network::isUp() { return Network::_isUp(QString("http://www.google.com")); }

bool Network::isUp(QString networkAddress) {
  return Network::_isUp(networkAddress);
}

bool Network::_isUp(QString networkAddress) {
  QUrl requestUrl(networkAddress);
  QNetworkAccessManager nam;
  QNetworkRequest req(requestUrl);
  QNetworkReply *reply = nam.get(req);
  QEventLoop loop;
  connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();
  if (reply->bytesAvailable())
    return true;
  else
    return false;
}
