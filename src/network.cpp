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

#include "network.h"

bool Network::isUp() {
  return Network::_isUp(QString("http://www.google.com"));
}

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
