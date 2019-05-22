#ifndef TEST_TESTLIBDAVCLIENT
#define TEST_TESTLIBDAVCLIENT

#include <QCoreApplication>
#include <QList>
#include <QObject>
#include <QTest>

#include <libdavclient/CardDAV.hpp>
#include <libdavclient/WebDAVClient.hpp>
#include <libdavclient/dto/WebDAVItem.hpp>
#include <libdavclient/utils/Environment.hpp>
#include <libdavclient/utils/WebDAVReply.hpp>

class TestLibDAVClient : public QObject {
  Q_OBJECT

 private:
  QCoreApplication *app;
  WebDAVClient *client;
  CardDAV *cardDAVClient;
  QString uid;

  void listDirOutputHandler(WebDAVReply *reply) {
    connect(reply, &WebDAVReply::listDirResponse,
            [=](QNetworkReply *listDirReply, QList<WebDAVItem> items) {
              qDebug() << "URL :" << listDirReply->url();
              qDebug() << "Received List of" << items.length() << "items";
              qDebug() << endl << "---------------------------------------";

              for (WebDAVItem item : items) {
                qDebug().noquote() << endl << item.toString();
                qDebug() << endl << "---------------------------------------";
              }

              QCoreApplication::exit(0);
            });
    connect(reply, &WebDAVReply::error, [=](QNetworkReply::NetworkError err) {
      qDebug() << err;
      QCoreApplication::exit(1);
    });
  }

 private slots:
  void initTestCase() {
    int argc = 1;
    char *argv[] = {"test"};

    this->uid = QUuid::createUuid().toString().mid(1, 36);
    this->app = new QCoreApplication(argc, argv);
    this->client =
        new WebDAVClient(Environment::get("LIBDAVCLIENT_WEBDAV_TEST_HOST"),
                         Environment::get("LIBDAVCLIENT_WEBDAV_TEST_USER"),
                         Environment::get("LIBDAVCLIENT_WEBDAV_TEST_PASSWORD"));
    this->cardDAVClient =
        new CardDAV(Environment::get("LIBDAVCLIENT_CARDDAV_TEST_HOST"),
                    Environment::get("LIBDAVCLIENT_CARDDAV_TEST_USER"),
                    Environment::get("LIBDAVCLIENT_CARDDAV_TEST_PASSWORD"));
  }

  void testListDir() {
    this->listDirOutputHandler(this->client->listDir(
        Environment::get("LIBDAVCLIENT_WEBDAV_TEST_PATH")));
    this->app->exec();
  }

  void testListDirWithDepth() {
    this->listDirOutputHandler(this->client->listDir(
        Environment::get("LIBDAVCLIENT_WEBDAV_TEST_PATH"), ListDepthEnum::One));

    this->app->exec();
  }

  void testDownload() {
    QString url = "/remote.php/webdav/Nextcloud%20Manual.pdf";

    WebDAVReply *reply = this->client->downloadFrom(url);

    connect(reply, &WebDAVReply::downloadResponse, [=](QNetworkReply *reply) {
      if (!reply->error()) {
        qDebug() << "\nDownload Success"
                 << "\nURL  :" << reply->url() << "\nSize :" << reply->size();
      } else {
        qDebug() << "ERROR(DOWNLOAD)" << reply->error();
      }
      QCoreApplication::exit(0);
    });
    connect(reply, &WebDAVReply::downloadProgressResponse,
            [=](qint64 bytesReceived, qint64 bytesTotal) {
              int percent = ((float)bytesReceived / bytesTotal) * 100;

              qDebug() << "\nReceived : " << bytesReceived
                       << "\nTotal    : " << bytesTotal
                       << "\nPercent  : " << percent;
            });
    connect(reply, &WebDAVReply::error, [=](QNetworkReply::NetworkError err) {
      qDebug() << "ERROR" << err;
      QCoreApplication::exit(1);
    });

    this->app->exec();
  }

  void testUpload() {
    QString url = Environment::get("LIBDAVCLIENT_WEBDAV_TEST_PATH");
    QFile file("/home/anupam/libwebdav/lib/WebDAVClient.cpp");
    file.open(QIODevice::ReadOnly);

    WebDAVReply *reply = this->client->uploadTo(url, "tttt.cpp", &file);

    connect(reply, &WebDAVReply::uploadFinished, [=](QNetworkReply *reply) {
      if (!reply->error()) {
        qDebug() << "\nUpload Success"
                 << "\nURL  :" << reply->url() << "\nSize :" << reply->size();
      } else {
        qDebug() << "ERROR(UPLOAD)" << reply->error();
      }
      QCoreApplication::exit(0);
    });

    connect(reply, &WebDAVReply::error, [=](QNetworkReply::NetworkError err) {
      qDebug() << "ERROR" << err;
      QCoreApplication::exit(1);
    });

    this->app->exec();
  }

  void testCreateDir() {
    QString dirName = QDate::currentDate().toString(Qt::DateFormat::ISODate);
    WebDAVReply *reply = this->client->createDir(
        Environment::get("LIBDAVCLIENT_WEBDAV_TEST_PATH"), dirName);

    connect(reply, &WebDAVReply::createDirFinished, [=](QNetworkReply *reply) {
      if (!reply->error()) {
        qDebug() << "\nDir Created"
                 << "\nURL  :" << reply->url();
      } else {
        qDebug() << "ERROR(CREATE DIR)" << reply->error();
      }
      QCoreApplication::exit(0);
    });

    connect(reply, &WebDAVReply::error, [=](QNetworkReply::NetworkError err) {
      qDebug() << "ERROR" << err;
      QCoreApplication::exit(1);
    });

    this->app->exec();
  }

  void testCopyDir() {
    QString dirName = QDate::currentDate().toString(Qt::DateFormat::ISODate);
    WebDAVReply *reply = this->client->copy(
        Environment::get("LIBDAVCLIENT_WEBDAV_TEST_PATH") + "/tttt.cpp",
        Environment::get("LIBDAVCLIENT_WEBDAV_TEST_PATH") + "/" + dirName +
            "/tttt-copy.cpp");

    connect(reply, &WebDAVReply::copyFinished, [=](QNetworkReply *reply) {
      if (!reply->error()) {
        qDebug() << "\nItem Copied"
                 << "\nURL  :" << reply->url();
      } else {
        qDebug() << "ERROR(COPY)" << reply->error();
      }
      QCoreApplication::exit(0);
    });

    connect(reply, &WebDAVReply::error, [=](QNetworkReply::NetworkError err) {
      qDebug() << "ERROR" << err;
      QCoreApplication::exit(1);
    });

    this->app->exec();
  }

  void testMoveDir() {
    QString dirName = QDate::currentDate().toString(Qt::DateFormat::ISODate);
    WebDAVReply *reply = this->client->move(
        Environment::get("LIBDAVCLIENT_WEBDAV_TEST_PATH") + "/tttt.cpp",
        Environment::get("LIBDAVCLIENT_WEBDAV_TEST_PATH") + "/" + dirName +
            "/tttt.cpp",
        true);

    connect(reply, &WebDAVReply::moveFinished, [=](QNetworkReply *reply) {
      if (!reply->error()) {
        qDebug() << "\nItem Moved"
                 << "\nURL  :" << reply->url();
      } else {
        qDebug() << "ERROR(MOVE DIR)" << reply->error();
      }
      QCoreApplication::exit(0);
    });

    connect(reply, &WebDAVReply::error, [=](QNetworkReply::NetworkError err) {
      qDebug() << "ERROR" << err;
      QCoreApplication::exit(1);
    });

    this->app->exec();
  }

  void testRemove() {
    QString dirName = QDate::currentDate().toString(Qt::DateFormat::ISODate);

    WebDAVReply *reply =
        this->client->remove(Environment::get("LIBDAVCLIENT_WEBDAV_TEST_PATH") +
                             "/" + dirName + "/tttt.cpp");

    connect(reply, &WebDAVReply::removeFinished, [=](QNetworkReply *reply) {
      if (!reply->error()) {
        qDebug() << "\nItem Removed"
                 << "\nURL  :" << reply->url();
      } else {
        qDebug() << "ERROR(REMOVE File)" << reply->error();
      }
      QCoreApplication::exit(0);
    });

    connect(reply, &WebDAVReply::error, [=](QNetworkReply::NetworkError err) {
      qDebug() << "ERROR" << err;
      QCoreApplication::exit(1);
    });

    this->app->exec();
  }

  void testRemoveDir() {
    QString dirName = QDate::currentDate().toString(Qt::DateFormat::ISODate);
    WebDAVReply *reply = this->client->remove(
        Environment::get("LIBDAVCLIENT_WEBDAV_TEST_PATH") + "/" + dirName);

    connect(reply, &WebDAVReply::removeFinished, [=](QNetworkReply *reply) {
      if (!reply->error()) {
        qDebug() << "\nItem Removed"
                 << "\nURL  :" << reply->url();
      } else {
        qDebug() << "ERROR(REMOVE Dir)" << reply->error();
      }
      QCoreApplication::exit(0);
    });

    connect(reply, &WebDAVReply::error, [=](QNetworkReply::NetworkError err) {
      qDebug() << "ERROR" << err;
      QCoreApplication::exit(1);
    });

    this->app->exec();
  }

  ////////////////////////////////////////////////

  void testCarddavConnection() {
    CardDAVReply *reply = this->cardDAVClient->testConnection();
    this->connect(reply, &CardDAVReply::testConnectionResponse,
                  [=](bool isSuccess) {
                    QCoreApplication::exit(0);
                    QCOMPARE(isSuccess, true);
                  });
    this->app->exec();

    CardDAV wrongCardDAVClient(
        "https://cloud.opendesktop.org/remote.php/dav/addressbooks/"
        "ab0027/",
        Environment::get("LIBDAVCLIENT_CARDDAV_TEST_USER"),
        Environment::get("LIBDAVCLIENT_CARDDAV_TEST_PASSWORD"));
    reply = wrongCardDAVClient.testConnection();
    this->connect(reply, &CardDAVReply::testConnectionResponse,
                  [=](bool isSuccess) {
                    QCoreApplication::exit(0);
                    QCOMPARE(isSuccess, false);
                  });
    this->app->exec();
  }

  void testCreateContact() {
    QString vCard =
        "BEGIN:VCARD\nVERSION:3.0\nN:Basak;Anupam;;;Basak\nADR;INTL;PARCEL;"
        "WORK:;;;;;;India\nEMAIL;INTERNET:anupam.basak27@gmail.com\nTEL;WORK:"
        "8981861008\nEND:VCARD";
    CardDAVReply *reply = this->cardDAVClient->createContact(this->uid, vCard);
    this->connect(reply, &CardDAVReply::createContactResponse,
                  [=](Contact *contact) {
                    QCoreApplication::exit(0);

                    qDebug() << "\n\n    Contact Created."
                             << "\n    ETAG :" << contact->getEtag()
                             << "\n    Href:" << contact->getHref()
                             << "\n    vCard :" << contact->getVcard() << "\n";
                  });
    this->connect(reply, &CardDAVReply::error,
                  [=](QNetworkReply::NetworkError err) {
                    QCoreApplication::exit(0);
                    qDebug() << err;
                    QCOMPARE(err, QNetworkReply::NoError);
                  });
    this->app->exec();
  }

  void testUpdateContact() {
    QString vCard =
        "BEGIN:VCARD\nVERSION:3.0\nN:Basak;Probal;;;Basak\nADR;INTL;PARCEL;"
        "WORK:;;;;;;India\nEMAIL;INTERNET:probal31@gmail.com\nTEL;WORK:"
        "8981861008\nEND:VCARD";
    CardDAVReply *reply = this->cardDAVClient->updateContact(
        QUrl(Environment::get("LIBDAVCLIENT_CARDDAV_TEST_HOST") + "/" +
             this->uid + ".vcf"),
        vCard, "*");
    this->connect(reply, &CardDAVReply::updateContactResponse,
                  [=](Contact *contact) {
                    QCoreApplication::exit(0);

                    qDebug() << "\n\n    Contact Updated."
                             << "\n    ETAG :" << contact->getEtag()
                             << "\n    Href:" << contact->getHref()
                             << "\n    vCard:" << contact->getVcard() << "\n";
                  });
    this->connect(reply, &CardDAVReply::error,
                  [=](QNetworkReply::NetworkError err) {
                    QCoreApplication::exit(0);
                    qDebug() << err;
                    QCOMPARE(err, QNetworkReply::NoError);
                  });
    this->app->exec();
  }

  void testDeleteContact() {
    CardDAVReply *reply = this->cardDAVClient->deleteContact(
        QUrl(Environment::get("LIBDAVCLIENT_CARDDAV_TEST_HOST") + "/" +
             this->uid + ".vcf"));
    this->connect(reply, &CardDAVReply::deleteContactResponse, [=]() {
      QCoreApplication::exit(0);

      qDebug() << "Contact Deleted";
    });
    this->connect(reply, &CardDAVReply::error,
                  [=](QNetworkReply::NetworkError err) {
                    QCoreApplication::exit(0);
                    qDebug() << err;
                    QCOMPARE(err, QNetworkReply::NoError);
                  });
    this->app->exec();
  }

  void testListAllContacts() {
    CardDAVReply *reply = this->cardDAVClient->listAllContacts();
    this->connect(reply, &CardDAVReply::listAllContactsResponse,
                  [=](QList<Contact *> contacts) {
                    QCoreApplication::exit(0);

                    qDebug() << "Size :" << contacts.length();

                    for (Contact *contact : contacts) {
                      qDebug() << ", ETag :" << contact->getEtag()
                               << "Contact URL :" << contact->getHref();
                      //                               << ", vCard :" <<
                      //                               contact->getVcard();
                    }
                  });
    this->connect(reply, &CardDAVReply::error,
                  [=](QNetworkReply::NetworkError err) {
                    QCoreApplication::exit(0);
                    qDebug() << err;
                    QCOMPARE(err, QNetworkReply::NoError);
                  });
    this->app->exec();
  }

  void cleanupTestCase() { delete this->app; }
};

QTEST_MAIN(TestLibDAVClient)
#include "TestLibDAVClient.moc"

#endif
