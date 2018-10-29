#ifndef WEBDAVCLIENT_HPP
#define WEBDAVCLIENT_HPP

#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>
#include <QString>

#include "utils/NetworkHelper.hpp"
#include "utils/WebDAVReply.hpp"
#include "utils/XMLHelper.hpp"

enum ListDepthEnum { Zero, One, Two, Infinity };

class WebDAVClient : public QObject {
  Q_OBJECT

 public:
  WebDAVClient(QString host, QString username, QString password);

  WebDAVReply* listDir(QString path = "/");
  WebDAVReply* listDir(QString path, ListDepthEnum depth);

  WebDAVReply* downloadFrom(QString path);
  WebDAVReply* downloadFrom(QString path, qint64 startByte, qint64 endByte);

  void uploadTo(QString path);

  ~WebDAVClient();

 private:
  NetworkHelper* networkHelper;
  XMLHelper* xmlHelper;

  void errorReplyHandler(WebDAVReply* reply, QNetworkReply::NetworkError err);
};

#endif
