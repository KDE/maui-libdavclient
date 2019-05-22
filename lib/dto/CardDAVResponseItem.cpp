#include "CardDAVResponseItem.hpp"

CardDAVResponseItem::CardDAVResponseItem() {}

CardDAVResponseItem::CardDAVResponseItem(bool isCollection, bool isAddressBook,
                                         bool isContact, QUrl href) {
  this->flagIsCollection = isCollection;
  this->flagIsAddressBook = isAddressBook;
  this->flagIsContact = isContact;
  this->href = href;
}

bool CardDAVResponseItem::isCollection() { return this->flagIsCollection; }

bool CardDAVResponseItem::isAddressBook() { return this->flagIsAddressBook; }

bool CardDAVResponseItem::isContact() { return this->flagIsContact; }

Contact *CardDAVResponseItem::getContact() { return this->contact; }

QUrl CardDAVResponseItem::getHref() { return this->href; }

void CardDAVResponseItem::setIsCollection(bool isCollection) {
  this->flagIsCollection = isCollection;
}

void CardDAVResponseItem::setIsAddressBook(bool isAddressBook) {
  this->flagIsAddressBook = isAddressBook;
}

void CardDAVResponseItem::setIsContact(bool isContact) {
  this->flagIsContact = isContact;
}

void CardDAVResponseItem::setContact(Contact *contact) {
  this->contact = contact;
}

void CardDAVResponseItem::setHref(QUrl href) { this->href = href; }
