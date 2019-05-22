#ifndef UTILS_CARDDAVREPLY_HPP
#define UTILS_CARDDAVREPLY_HPP

#include <QNetworkReply>
#include <QObject>

#include "../dto/CardDAVResponseItem.hpp"

class CardDAVReply : public QObject {
  Q_OBJECT

 public:
  void sendTestConnectionResponseSignal(bool isSuccess);
  void sendListContactsResponseSignal(QList<Contact *> contacts);
  void sendCreateContactResponseSignal(Contact *contact);
  void sendUpdateContactResponseSignal(Contact *contact);
  void sendDeleteContactResponseSignal();
  void sendError(QNetworkReply::NetworkError err);

 signals:
  void testConnectionResponse(bool isSuccess);
  void listAllContactsResponse(QList<Contact *> contacts);
  void createContactResponse(Contact *contact);
  void updateContactResponse(Contact *contact);
  void deleteContactResponse();
  void error(QNetworkReply::NetworkError err);
};

#endif
