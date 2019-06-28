#include "XMLHelper.hpp"

#include <QDebug>
#include <QtXml/QDomDocument>

QList<WebDAVItem> XMLHelper::parseListDirResponse(WebDAV *webdavClient,
                                                  QByteArray xml) {
  QList<WebDAVItem> items;
  QString webdavNS = "DAV:";
  QDomDocument doc;
  doc.setContent(xml, true);

  QDomNodeList responses = doc.elementsByTagNameNS(webdavNS, "response");

  for (int i = 0; i < responses.length(); i++) {
    QDomElement response = responses.at(i).toElement();

    QString href =
        response.elementsByTagNameNS(webdavNS, "href").at(0).toElement().text();
    QString creationDate =
        response.elementsByTagNameNS(webdavNS, "creationdate")
            .at(0)
            .toElement()
            .text();
    QString lastModified =
        response.elementsByTagNameNS(webdavNS, "getlastmodified")
            .at(0)
            .toElement()
            .text();
    QString displayName = response.elementsByTagNameNS(webdavNS, "displayname")
                              .at(0)
                              .toElement()
                              .text();
    QString contentType =
        response.elementsByTagNameNS(webdavNS, "getcontenttype")
            .at(0)
            .toElement()
            .text();
    QString contentLength =
        response.elementsByTagNameNS(webdavNS, "getcontentlength")
            .at(0)
            .toElement()
            .text();
    bool isCollection;

    if (response.elementsByTagNameNS(webdavNS, "resourcetype")
            .at(0)
            .toElement()
            .elementsByTagNameNS(webdavNS, "collection")
            .size() == 1) {
      isCollection = true;
    } else {
      isCollection = false;
    }

    items.append(WebDAVItem(webdavClient, href, creationDate, lastModified,
                            displayName, contentType, contentLength,
                            isCollection));
  }

  return items;
}

QList<CardDAVResponseItem *> XMLHelper::parseCardDAVMultiStatusResponse(
    QString responseXml) {
  QString webdavNS = "DAV:";
  QString cardDavNS = "urn:ietf:params:xml:ns:carddav";
  QList<CardDAVResponseItem *> responseTagList;

  QDomDocument doc;
  doc.setContent(responseXml, true);

  QDomNodeList responses = doc.elementsByTagNameNS(webdavNS, "response");

  for (int i = 0; i < responses.length(); i++) {
    CardDAVResponseItem *item = new CardDAVResponseItem();
    QDomElement response = responses.at(i).toElement();
    QString href =
        response.elementsByTagNameNS(webdavNS, "href").at(0).toElement().text();

    item->setHref(QUrl(href));

    if (response.elementsByTagNameNS(webdavNS, "resourcetype")
            .at(0)
            .toElement()
            .elementsByTagNameNS(webdavNS, "collection")
            .size() == 1) {
      item->setIsCollection(true);
    } else {
      item->setIsCollection(false);
    }

    if (response.elementsByTagNameNS(webdavNS, "resourcetype")
            .at(0)
            .toElement()
            .elementsByTagNameNS(cardDavNS, "addressbook")
            .size() == 1) {
      item->setIsAddressBook(true);
    } else {
      item->setIsAddressBook(false);
    }

    if (response.elementsByTagNameNS(cardDavNS, "address-data").size() == 1) {
      QString vCard, eTag;

      item->setIsContact(true);

      vCard = response.elementsByTagNameNS(cardDavNS, "address-data")
                  .at(0)
                  .toElement()
                  .text();
      eTag = response.elementsByTagNameNS(webdavNS, "getetag")
                 .at(0)
                 .toElement()
                 .text();

      item->setContact(new Contact(vCard, eTag, QUrl(href)));
    } else {
      item->setIsContact(false);
    }

    responseTagList.push_front(item);
  }

  return responseTagList;
}
