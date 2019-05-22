#ifndef CARDDAV_HPP
#define CARDDAV_HPP

#include <QObject>
#include <QString>

#include "utils/CardDAVReply.hpp"
#include "utils/NetworkHelper.hpp"
#include "utils/XMLHelper.hpp"

class CardDAV : public QObject {
  Q_OBJECT

 public:
  CardDAV(QString host, QString username, QString password);
  ~CardDAV();

  CardDAVReply* testConnection();

  CardDAVReply* listAllContacts();
  CardDAVReply* createContact(QString uid, QString vCard,
                              bool shouldOverwrite = false);
  CardDAVReply* updateContact(QUrl href, QString vCard, QString etag);
  CardDAVReply* deleteContact(QUrl href);

 private:
  NetworkHelper* networkHelper;
  XMLHelper* xmlHelper;
  QString host;
  QString username;
  QString password;
};

#endif
