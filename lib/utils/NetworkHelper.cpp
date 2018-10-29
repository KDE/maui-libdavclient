#include <QByteArray>
#include <QMap>
#include <QNetworkAccessManager>
#include <QString>
#include <QUrl>

#include "NetworkHelper.hpp"

NetworkHelper::NetworkHelper(QString host, QString username, QString password) {
  this->host = host;
  this->username = username;
  this->password = password;

  this->networkManager = new QNetworkAccessManager(this);
}

QNetworkReply* NetworkHelper::makeRequest(QString method, QString path,
                                          QMap<QString, QString> headers) {
  QUrl url(this->host + "/" + path);
  QNetworkRequest request(url);
  QMapIterator<QString, QString> headersIterator(headers);

  this->setRequestAuthHeader(&request);

  while (headersIterator.hasNext()) {
    headersIterator.next();
    request.setRawHeader(
        QByteArray::fromStdString(headersIterator.key().toStdString()),
        QByteArray::fromStdString(headersIterator.value().toStdString()));
  }

  QNetworkReply* reply = this->networkManager->sendCustomRequest(
      request, QByteArray::fromStdString(method.toStdString()));

  return reply;
}

void NetworkHelper::setRequestAuthHeader(QNetworkRequest* request) {
  QString authData = this->username + ":" + this->password;
  QByteArray data = authData.toLocal8Bit().toBase64();
  QString headerData = "Basic " + data;

  request->setRawHeader("Authorization", headerData.toLocal8Bit());
}
