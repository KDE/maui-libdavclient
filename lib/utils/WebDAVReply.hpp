#ifndef UTILS_WEBDAVREPLY_HPP
#define UTILS_WEBDAVREPLY_HPP

#include <QList>
#include <QNetworkReply>
#include <QObject>
#include <QSslError>

#include "../dto/WebDAVItem.hpp"

class WebDAVReply : public QObject {
  Q_OBJECT

 public:
  void sendTestConnectionResponseSignal(bool isSuccess);
  void sendListDirResponseSignal(QNetworkReply* listDirReply,
                                 QList<WebDAVItem> items);
  void sendDownloadResponseSignal(QNetworkReply* downloadReply);
  void sendDownloadProgressResponseSignal(qint64 bytesReceived,
                                          qint64 bytesTotal);
  void sendUploadFinishedResponseSignal(QNetworkReply* uploadReply);
  void sendDirCreatedResponseSignal(QNetworkReply* createDirReply);
  void sendCopyResponseSignal(QNetworkReply* copyReply);
  void sendMoveResponseSignal(QNetworkReply* moveReply);
  void sendRemoveResponseSignal(QNetworkReply* removeReply);
  void sendError(QNetworkReply::NetworkError err);

 signals:
  void testConnectionResponse(bool isSuccess);
  void listDirResponse(QNetworkReply* listDirReply, QList<WebDAVItem> items);
  void downloadResponse(QNetworkReply* downloadReply);
  void downloadProgressResponse(qint64 bytesReceived, qint64 bytesTotal);
  void uploadFinished(QNetworkReply* uploadReply);
  void createDirFinished(QNetworkReply* createDirReply);
  void copyFinished(QNetworkReply* copyReply);
  void moveFinished(QNetworkReply* moveReply);
  void removeFinished(QNetworkReply* removeReply);
  void error(QNetworkReply::NetworkError err);
};

#endif
