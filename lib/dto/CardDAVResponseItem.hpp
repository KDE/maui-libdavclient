#ifndef DTO_CARDDAVRESPONSEITEM_HPP
#define DTO_CARDDAVRESPONSEITEM_HPP

#include <QString>
#include <QUrl>

#include "Contact.hpp"

class CardDAVResponseItem {
 public:
  CardDAVResponseItem();
  CardDAVResponseItem(bool isCollection, bool isAddressBook, bool isContact,
                      QUrl href);

  bool isCollection();
  bool isAddressBook();
  bool isContact();

  Contact *getContact();
  QUrl getHref();

  void setIsCollection(bool isCollection);
  void setIsAddressBook(bool isAddressBook);
  void setIsContact(bool isContact);
  void setContact(Contact *contact);
  void setHref(QUrl href);

 private:
  Contact *contact = nullptr;
  QUrl href;

  bool flagIsCollection = false;
  bool flagIsAddressBook = false;
  bool flagIsContact = false;
};

#endif
