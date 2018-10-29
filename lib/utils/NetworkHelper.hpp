#ifndef UTILS_NETWORKHELPER_HPP
#define UTILS_NETWORKHELPER_HPP

#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>

class NetworkHelper : public QObject {
  Q_OBJECT

 private:
  QString host;
  QString username;
  QString password;
  QNetworkAccessManager* networkManager;

  void setRequestAuthHeader(QNetworkRequest* request);

 public:
  NetworkHelper(QString host, QString username, QString password);

  QNetworkReply* makeRequest(QString method, QString path,
                             QMap<QString, QString> headers);
};

#endif
