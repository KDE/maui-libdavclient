#ifndef DTO_CONTACT_HPP
#define DTO_CONTACT_HPP

#include <QString>
#include <QUrl>

class Contact {
 public:
  Contact(QString vCard, QString eTag, QUrl href);

  QString getVcard();
  QString getEtag();
  QUrl getHref();

 private:
  QString vCard;
  QString eTag;
  QUrl href;
};

#endif
