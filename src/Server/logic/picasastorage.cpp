#include "picasastorage.h"

#include "kinectglobal.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QByteArray>
#include <QSslConfiguration>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QDomDocument>
#include <QFile>
#include <QDateTime>
#include <QSettings>
#include <QBuffer>

PicasaStorage::PicasaStorage(QObject *parent) :
    QObject(parent),
    m_accessManager(new QNetworkAccessManager),
    m_authRequest(0),
    m_albumRequest(0),
    m_createAlbumRequest(0),
    m_sendImageRequest(0)
{
    BEGIN
    loadFromFile();
    connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), SLOT(onReplyFinished(QNetworkReply*)));
    connect(m_accessManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)), SLOT(onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
    m_accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    END
}

PicasaStorage::~PicasaStorage()
{
}

void PicasaStorage::onReplyFinished(QNetworkReply *_reply)
{
    BEGIN;
    qDebug() << _reply->error() << _reply->errorString();
    QByteArray content;
    if (_reply->error() == QNetworkReply::NoError) {
	content = _reply->readAll();
	foreach (QByteArray a , _reply->rawHeaderList()) {
	    qDebug() << a << _reply->rawHeader(a);
	}
	qDebug() << "AllContent\n" << content << "\nContentFinished";
    } else {
	emit statusMessage("Error", "Problem with reply");
    }
    if ((m_authRequest) && (_reply->request().url() == m_authRequest->url())) {
	qDebug() << "Auth request" << _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (_reply->error() == QNetworkReply::NoError) {
	    QString s(content);
	    s.remove(0, s.indexOf("Auth=")-1);
	    s.remove("Auth=");
	    s = s.trimmed();
	    m_authHeader = QString("GoogleLogin auth=").append(s);
	    qDebug() << m_authHeader;
	    requestAlbums();
	} else {
	    //TODO: improve this status chceck. add more sophisticated communicates
	    if (_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 202) {
		emit statusMessage("Error", "Authorization failed. Please check login and password are correct");
	    } else {
		emit statusMessage("Error", "Authorization failed.");
	    }
	}
	delete m_authRequest;
	m_authRequest = 0;
    } else if ((m_albumRequest) && (_reply->request().url() == m_albumRequest->url())) {
	qDebug() << "Album Request";
	if (_reply->error() == QNetworkReply::NoError) {

	    if (!checkKinectFolderExistence(content)) {
		qDebug() << "requestCreatingKinectAlarm";
		requestCreatingKinectAlarmAlbum();
	    } else {
		saveToFile();
	    }
	} else {
	    //TODO: emit signal if something was wrong
	}
	delete m_albumRequest;
	m_albumRequest = 0;
    } else if ((m_sendImageRequest) && (_reply->request().url() == m_sendImageRequest->url())) {
	qDebug() << "SendRequest";
	INFO(_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
	//TODO: should i check if something goes wrong? probably yes, but i'm too lazy
	if (_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404 ) {
	    requestCreatingKinectAlarmAlbum();
	}
	delete m_sendImageRequest;
	m_sendImageRequest = 0;
	if (!m_pendingData.isEmpty()) {
	    QString name = m_pendingData.at(0).first;
	    QByteArray data = m_pendingData.at(0).second;
	    requestSendImage(data, name);
	    m_pendingData.removeFirst();
	}
    } else if ((m_createAlbumRequest) && _reply->request().url() == m_createAlbumRequest->url()) {
	INFO("createAlbum");
	INFO(_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute));
	checkKinectFolderExistence(content);
	delete m_createAlbumRequest;
	m_createAlbumRequest = 0;


    }

    _reply->deleteLater();
    END
}

void PicasaStorage::requestAuth()
{
    BEGIN;
    if (m_authRequest)
	return;
    m_authRequest = new QNetworkRequest(QUrl("https://www.google.com/accounts/ClientLogin"));
    m_authRequest->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    QByteArray data("accountType=GOOGLE&Email=");
    data.append(m_login);
    data.append("&Passwd=");
    data.append(m_password);
    data.append("&service=lh2&source=KinectAlarm");
    m_accessManager->post(*m_authRequest,data);
    END
}

void PicasaStorage::onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility _accessible)
{
    BEGIN
    qDebug() << " Accesibility " << _accessible;
    END
}

void PicasaStorage::requestAlbums()
{
    BEGIN;
    if (m_albumRequest)
	return;
    m_albumRequest = new QNetworkRequest(QUrl("http://picasaweb.google.com/data/feed/api/user/default"));
    m_albumRequest->setRawHeader(QByteArray("GData-Version"), QByteArray("2"));
    m_albumRequest->setRawHeader(QByteArray("Authorization"), QByteArray(m_authHeader.toAscii()));
    m_accessManager->get(*m_albumRequest);
    END;
}

bool PicasaStorage::checkKinectFolderExistence(QString _xml)
{
    BEGIN;
    QDomDocument xmlDoc;
    xmlDoc.setContent(_xml);
    qDebug() << xmlDoc.toString();
    QDomNodeList list = xmlDoc.elementsByTagName("entry");
    for (int i = 0; i < list.size(); i++) {
	if (list.at(i).isElement()) {
	    QDomElement elem = list.at(i).toElement();
	    if (elem.elementsByTagName("title").at(0).toElement().text() == "KinectAlarm") {
		m_kinectAlbumAddress = QString("https://picasaweb.google.com/data/feed/api/user/default/albumid/").append(elem.elementsByTagName("gphoto:id").at(0).toElement().text());
		return true;
	    }
	}
    }
    END;
    return false;
}

void PicasaStorage::requestSendImage(const QByteArray& _data, const QString& _imgName)
{
    BEGIN
    if (m_sendImageRequest) {
	if (m_pendingData.size() == 10) {
	    m_pendingData.removeFirst();
	}
	m_pendingData.append(qMakePair(_imgName, _data));
	return;
    }
    m_sendImageRequest = new QNetworkRequest(QUrl(m_kinectAlbumAddress));
    m_sendImageRequest->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    m_sendImageRequest->setHeader(QNetworkRequest::ContentLengthHeader, _data.length());
    m_sendImageRequest->setRawHeader(QByteArray("GData-Version"), QByteArray("2"));
    m_sendImageRequest->setRawHeader(QByteArray("Authorization"), QByteArray(m_authHeader.toAscii()));
    m_sendImageRequest->setRawHeader(QByteArray("Slug"), QByteArray(_imgName.toAscii()));
    m_accessManager->post(*m_sendImageRequest, _data);
    END

}

void PicasaStorage::requestCreatingKinectAlarmAlbum()
{
    BEGIN;
    if (m_createAlbumRequest)
	return;
    m_createAlbumRequest = new QNetworkRequest(QUrl("https://picasaweb.google.com/data/feed/api/user/default"));
    m_createAlbumRequest->setRawHeader(QByteArray("GData-Version"), QByteArray("2"));
    m_createAlbumRequest->setRawHeader(QByteArray("Authorization"), QByteArray(m_authHeader.toAscii()));
    m_createAlbumRequest->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/atom+xml"));
    QString xmlQuery( \
    "<?xml version='1.0' encoding='utf-8'?>\n" \
    "<entry xmlns='http://www.w3.org/2005/Atom' " \
	"xmlns:media='http://search.yahoo.com/mrss/' " \
	"xmlns:gphoto='http://schemas.google.com/photos/2007'>\n" \
    "  <title type='text'>KinectAlarm</title>\n" \
    "  <summary type='text'></summary>\n" \

    "  <gphoto:access>private</gphoto:access>\n" \
    "  <gphoto:timestamp>");
    xmlQuery.append(QString::number(QDateTime::currentMSecsSinceEpoch()));
    xmlQuery.append( \
    "</gphoto:timestamp>\n" \
    "  <media:group>\n" \
    "    <media:keywords>kinect, alarm</media:keywords>\n" \
    "  </media:group>\n" \
    "  <category scheme='http://schemas.google.com/g/2005#kind' " \
	"term='http://schemas.google.com/photos/2007#album'>" \
    "  </category>\n" \
    "</entry>\n" );
    m_accessManager->post(*m_createAlbumRequest, QByteArray(xmlQuery.toUtf8()));
    qDebug() << xmlQuery;
    END;
}

void PicasaStorage::saveToFile()
{
    QSettings settings;
    settings.beginGroup(name());
    settings.setValue("login", QVariant(m_login));
    //settings.setValue("password", QVariant(m_password));
    settings.setValue("auth_header", QVariant(m_authHeader));
    settings.setValue("kinect_album", QVariant(m_kinectAlbumAddress));
    settings.endGroup();
}

void PicasaStorage::loadFromFile()
{
    QSettings settings;
    QVariant var;
    settings.beginGroup(name());
    var = settings.value("login", QVariant(m_login));
    setLogin(var.toString());
    //var = settings.value("password", QVariant(m_password));
    //setPassword(var.toString());
    var = settings.value("auth_header", QVariant(m_authHeader));
    m_authHeader = var.toString();
    var = settings.value("kinect_album", QVariant(m_kinectAlbumAddress));
    m_kinectAlbumAddress = var.toString();
    settings.endGroup();
}

void PicasaStorage::saveImage(const QImage &_image)
{
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    _image.save(&buffer, "PNG"); // writes image into ba in PNG format
    requestSendImage(ba, QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz").append(".png"));
}



