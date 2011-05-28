#ifndef PICASASTORAGE_H
#define PICASASTORAGE_H

#include "ikinectstorage.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDebug>
#include <QPointer>
#include <QPair>

class PicasaStorage : public QObject, public IKinectStorage
{
    Q_OBJECT
public:
    explicit PicasaStorage(QObject *parent = 0);
    virtual ~PicasaStorage();


    void setLogin(const QString& _login) { m_login = _login; }
    QString login() { return m_login; }

    void setPassword(const QString& _password) { m_password = _password; }
    QString password() { return m_password; }

    void saveToFile();
    void loadFromFile();

    QString name() { return "PicasaStorage"; }
    void saveImage(const QImage &_image);

signals:
    void statusMessage(QString type, QString _msg);

public slots:
    void requestAuth();
    void requestAlbums();
    void requestSendImage(const QByteArray& _data, const QString& _imgName);

private slots:
    void onReplyFinished(QNetworkReply* _reply);
    void onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility _accessible);

private:
    bool checkKinectFolderExistence(QString _xml);
    void requestCreatingKinectAlarmAlbum();
    QNetworkAccessManager* m_accessManager;
    QNetworkRequest* m_authRequest;
    QNetworkRequest* m_albumRequest;
    QNetworkRequest* m_sendImageRequest;
    QNetworkRequest* m_createAlbumRequest;
    QString m_authHeader;
    QString m_kinectAlbumAddress;
    QString m_login;
    QString m_password;

    QList< QPair<QString, QByteArray> > m_pendingData;
    int m_listCounter;

};

#endif // PICASASTORAGE_H
