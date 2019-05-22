#include "Contact.hpp"

Contact::Contact(QString vCard, QString eTag, QUrl href) {
  this->vCard = vCard;
  this->eTag = eTag;
  this->href = href;
}

QString Contact::getVcard() { return this->vCard; }

QString Contact::getEtag() { return this->eTag; }

QUrl Contact::getHref() { return this->href; }
