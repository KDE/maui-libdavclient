#include "NetworkHelper.hpp"

NetworkHelper::NetworkHelper(QString host, QString username, QString password) {
  this->host = host;
  this->username = username;
  this->password = password;

  this->networkManager = new QNetworkAccessManager(this);
}

QNetworkReply* NetworkHelper::makeRequest(QString method, QString path,
                                          QMap<QString, QString> headers) {
  QNetworkRequest request(QUrl(this->host + "/" + path));

  this->setRequestAuthHeader(&request);
  this->setRequestHeaders(&request, headers);

  QNetworkReply* reply = this->networkManager->sendCustomRequest(
      request, QByteArray::fromStdString(method.toStdString()));

  return reply;
}

QNetworkReply* NetworkHelper::makePutRequest(QString path,
                                             QMap<QString, QString> headers,
                                             QIODevice* file) {
  QNetworkRequest request(QUrl(this->host + "/" + path));

  this->setRequestAuthHeader(&request);
  this->setRequestHeaders(&request, headers);

  QNetworkReply* reply = this->networkManager->put(request, file);

  return reply;
}

// QNetworkReply* NetworkHelper::makeRequest(QString method,
//                                          QMap<QString, QString> headers) {
//  return this->makeRequest(method, QUrl(this->host), headers, "");
//}

// QNetworkReply* NetworkHelper::makeRequest(QString method,
//                                          QMap<QString, QString> headers,
//                                          QString body) {
//  return this->makeRequest(method, QUrl(this->host), headers, body);
//}

// QNetworkReply* NetworkHelper::makeRequest(QString method, QUrl path,
//                                          QMap<QString, QString> headers,
//                                          QString body) {
//  QUrl url(this->host);
//  QString port = url.port() != -1 ? ":" + QString::number(url.port()) : "";
//  QUrl requestUrl(url.scheme() + "://" + url.host() + port + "/" +
//  path.path());

//  QNetworkRequest request(requestUrl);

//  this->setRequestAuthHeader(&request);
//  this->setRequestHeaders(&request, headers);

//  QNetworkReply* reply = this->networkManager->sendCustomRequest(
//      request, QByteArray::fromStdString(method.toStdString()),
//      QByteArray::fromStdString(body.toStdString()));

//  return reply;
//}

// QNetworkReply* NetworkHelper::makePutRequest(QMap<QString, QString> headers,
//                                             QIODevice* file) {
//  QNetworkRequest request(QUrl(this->host));

//  this->setRequestAuthHeader(&request);
//  this->setRequestHeaders(&request, headers);

//  QNetworkReply* reply = this->networkManager->put(request, file);

//  return reply;
//}

void NetworkHelper::setRequestAuthHeader(QNetworkRequest* request) {
  QString authData = this->username + ":" + this->password;
  QByteArray data = authData.toLocal8Bit().toBase64();
  QString headerData = "Basic " + data;

  request->setRawHeader("Authorization", headerData.toLocal8Bit());
}

void NetworkHelper::setRequestHeaders(QNetworkRequest* request,
                                      QMap<QString, QString> headers) {
  QMapIterator<QString, QString> headersIterator(headers);

  while (headersIterator.hasNext()) {
    headersIterator.next();
    request->setRawHeader(
        QByteArray::fromStdString(headersIterator.key().toStdString()),
        QByteArray::fromStdString(headersIterator.value().toStdString()));
  }
}
