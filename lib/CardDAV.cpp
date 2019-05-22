#include "CardDAV.hpp"

CardDAV::CardDAV(QString host, QString username, QString password) {
  this->host = host;
  this->username = username;
  this->password = password;

  this->networkHelper = new NetworkHelper(host, username, password);
  this->xmlHelper = new XMLHelper();
}

CardDAV::~CardDAV() { delete this->networkHelper; }

CardDAVReply* CardDAV::testConnection() {
  CardDAVReply* reply = new CardDAVReply();
  QMap<QString, QString> headers;
  QNetworkReply* testReply;

  testReply = this->networkHelper->makeRequest("PROPFIND", headers);

  this->connect(testReply, &QNetworkReply::finished, [=]() {
    if (testReply->error()) {
      reply->sendTestConnectionResponseSignal(false);
      reply->sendError(testReply->error());
    }
    bool found = false;
    QList<CardDAVResponseItem*> responseList =
        this->xmlHelper->parseCardDAVMultiStatusResponse(testReply->readAll());
    QString host(this->host);
    QUrl hostUrl(host.replace("://", "").replace(0, host.indexOf('/'), ""));

    for (CardDAVResponseItem* item : responseList) {
      //      qDebug() << item->getHref().matches(hostUrl,
      //      QUrl::StripTrailingSlash)
      //               << item->isAddressBook();

      if (item->getHref().matches(hostUrl, QUrl::StripTrailingSlash) &&
          item->isAddressBook()) {
        found = true;

        break;
      }
    }

    if (found) {
      reply->sendTestConnectionResponseSignal(true);
    } else {
      reply->sendTestConnectionResponseSignal(false);
    }
  });

  this->connect(
      testReply,
      QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
      [=](QNetworkReply::NetworkError err) {
        reply->sendTestConnectionResponseSignal(false);
        reply->sendError(err);
      });

  return reply;
}

CardDAVReply* CardDAV::listAllContacts() {
  CardDAVReply* reply = new CardDAVReply();
  QMap<QString, QString> headers;
  QNetworkReply* listReply;
  QString requestXml =
      "<card:addressbook-query xmlns:d=\"DAV:\" xmlns:card=\"urn:ietf:params:xml:ns:carddav\"> \
                            <d:prop> \
                                <card:address-data /> \
                                <d:getetag /> \
                            </d:prop> \
                        </card:addressbook-query>";

  headers.insert("Depth", "1");
  headers.insert("Content-Type", "application/xml");

  listReply = this->networkHelper->makeRequest("REPORT", headers, requestXml);

  this->connect(listReply, &QNetworkReply::finished, [=]() {
    QList<CardDAVResponseItem*> responses =
        this->xmlHelper->parseCardDAVMultiStatusResponse(listReply->readAll());
    QList<Contact*> contacts;

    for (CardDAVResponseItem* response : responses) {
      if (response->isContact()) {
        contacts.push_front(response->getContact());
      }
    }

    reply->sendListContactsResponseSignal(contacts);
  });

  this->connect(
      listReply,
      QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
      [=](QNetworkReply::NetworkError err) { reply->sendError(err); });

  return reply;
}

CardDAVReply* CardDAV::createContact(QString uid, QString vCard,
                                     bool shouldOverwrite) {
  CardDAVReply* reply = new CardDAVReply();
  QMap<QString, QString> headers;
  QNetworkReply* createReply;
  QUrl contactUrl(this->host + "/" + uid + ".vcf");

  if (!shouldOverwrite) {
    headers.insert("If-None-Match", "*");
  }

  createReply =
      this->networkHelper->makeRequest("PUT", contactUrl, headers, vCard);

  this->connect(createReply, &QNetworkReply::finished, [=]() {
    if (createReply->error()) {
      reply->sendError(createReply->error());
    } else {
      QMap<QString, QString> getContactHeaders;
      QNetworkReply* getContactReply = this->networkHelper->makeRequest(
          "GET", contactUrl.path(), getContactHeaders, "");
      this->connect(getContactReply, &QNetworkReply::finished, [=]() {
        if (getContactReply->error()) {
          reply->sendError(getContactReply->error());
        } else {
          QString vCard = getContactReply->readAll();
          QString etag(getContactReply->rawHeader("ETag"));

          reply->sendCreateContactResponseSignal(
              new Contact(vCard, etag, QUrl(contactUrl.path())));
        }
      });

      this->connect(
          getContactReply,
          QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
          [=](QNetworkReply::NetworkError err) { reply->sendError(err); });
    }
  });

  this->connect(
      createReply,
      QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
      [=](QNetworkReply::NetworkError err) { reply->sendError(err); });

  return reply;
}

CardDAVReply* CardDAV::updateContact(QUrl href, QString vCard, QString etag) {
  CardDAVReply* reply = new CardDAVReply();
  QMap<QString, QString> headers;
  QNetworkReply* updateReply;

  headers.insert("If-Match", etag);

  updateReply = this->networkHelper->makeRequest("PUT", href, headers, vCard);

  this->connect(updateReply, &QNetworkReply::finished, [=]() {
    if (updateReply->error()) {
      reply->sendError(updateReply->error());
    } else {
      QMap<QString, QString> getContactHeaders;
      QNetworkReply* getContactReply =
          this->networkHelper->makeRequest("GET", href, getContactHeaders, "");
      this->connect(getContactReply, &QNetworkReply::finished, [=]() {
        if (getContactReply->error()) {
          reply->sendError(getContactReply->error());
        } else {
          QString vCard = getContactReply->readAll();
          QString etag(getContactReply->rawHeader("ETag"));

          reply->sendUpdateContactResponseSignal(
              new Contact(vCard, etag, href));
        }
      });

      this->connect(
          getContactReply,
          QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
          [=](QNetworkReply::NetworkError err) { reply->sendError(err); });
    }
  });

  this->connect(
      updateReply,
      QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
      [=](QNetworkReply::NetworkError err) {
        qDebug() << updateReply->readAll();
        reply->sendError(err);
      });

  return reply;
}

CardDAVReply* CardDAV::deleteContact(QUrl href) {
  CardDAVReply* reply = new CardDAVReply();
  QMap<QString, QString> headers;
  QNetworkReply* deleteReply =
      this->networkHelper->makeRequest("DELETE", href, headers, "");

  this->connect(deleteReply, &QNetworkReply::finished, [=]() {
    if (deleteReply->error()) {
      reply->sendError(deleteReply->error());
    } else {
      reply->sendDeleteContactResponseSignal();
    }
  });

  this->connect(
      deleteReply,
      QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
      [=](QNetworkReply::NetworkError err) { reply->sendError(err); });

  return reply;
}
