#ifndef WEBDAV_HPP
#define WEBDAV_HPP

#include <QIODevice>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>
#include <QString>

#include "utils/NetworkHelper.hpp"
#include "utils/WebDAVReply.hpp"
#include "utils/XMLHelper.hpp"

enum ListDepthEnum { Zero, One, Two, Infinity };

class WebDAV : public QObject {
  Q_OBJECT

 public:
  WebDAV(QString host, QString username, QString password);

  WebDAVReply* listDir(QString path = "/");
  WebDAVReply* listDir(QString path, ListDepthEnum depth);

  WebDAVReply* downloadFrom(QString path);
  WebDAVReply* downloadFrom(QString path, qint64 startByte, qint64 endByte);

  WebDAVReply* uploadTo(QString path, QString filename, QIODevice* file);

  WebDAVReply* createDir(QString path, QString dirName);

  WebDAVReply* copy(QString source, QString destination);

  WebDAVReply* move(QString source, QString destination,
                    bool overwrite = false);

  WebDAVReply* remove(QString path);

  ~WebDAV();

 private:
  NetworkHelper* networkHelper;
  XMLHelper* xmlHelper;

  void errorReplyHandler(WebDAVReply* reply, QNetworkReply::NetworkError err);
};

#endif
