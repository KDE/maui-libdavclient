#include "CardDAVReply.hpp"

void CardDAVReply::sendTestConnectionResponseSignal(bool isSuccess) {
  emit testConnectionResponse(isSuccess);
}

void CardDAVReply::sendListContactsResponseSignal(QList<Contact *> contacts) {
  emit listAllContactsResponse(contacts);
}

void CardDAVReply::sendCreateContactResponseSignal(Contact *contact) {
  emit createContactResponse(contact);
}

void CardDAVReply::sendUpdateContactResponseSignal(Contact *contact) {
  emit updateContactResponse(contact);
}

void CardDAVReply::sendDeleteContactResponseSignal() {
  emit deleteContactResponse();
}

void CardDAVReply::sendError(QNetworkReply::NetworkError err) {
  emit error(err);
}
