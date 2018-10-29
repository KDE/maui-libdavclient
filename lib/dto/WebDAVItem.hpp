#ifndef DTO_WEBDAVITEM_HPP
#define DTO_WEBDAVITEM_HPP

#include <QDateTime>
#include <QString>

class WebDAVClient;
class WebDAVReply;

class WebDAVItem {
 public:
  WebDAVItem(WebDAVClient* webdavClient, QString href, QString creationDate,
             QString lastModified, QString displayName, QString contentType,
             QString contentLength, bool isCollection);

  bool isCollection();
  bool isFile();

  WebDAVReply* download();
  WebDAVReply* listDir();
  void upload();

  QString toString();

  QString getHref();
  QDateTime getCreationDate();
  QString getLastModified();
  QString getDisplayName();
  QString getContentType();
  int getContentLength();

  static WebDAVItem fromHref();

 private:
  WebDAVClient* webdavClient;
  QString href;
  QDateTime creationDate;
  QString lastModified;
  QString displayName;
  QString contentType;
  int contentLength;

  bool flagIsCollection;
};

#endif
