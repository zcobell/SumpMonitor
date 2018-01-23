#ifndef NETWORK_H
#define NETWORK_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QtCore>

class Network : public QObject {
  Q_OBJECT

public:
  static bool isUp();

  static bool isUp(QString networkAddress);

private:
  static bool _isUp(QString networkAddress);
};

#endif //NETWORK_H
