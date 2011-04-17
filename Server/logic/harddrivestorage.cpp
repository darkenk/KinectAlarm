#include "harddrivestorage.h"

#include <QTimer>
#include <QImage>
#include <QDir>
#include <QDateTime>
#include <QTimerEvent>
#include <QSettings>
#include <QVariant>
#include <QDebug>

HardDriveStorage::HardDriveStorage(KinectPluginLoader* _kinectPluginLoader, QObject *_parent) :
    QObject(_parent),
    m_kinect(0),
    m_kinectPluginLoader(_kinectPluginLoader),
    m_directoryPath("./"),
    m_firstDelay(500),
    m_repeatableDelay(2),
    m_storageActive(false),
    m_nextImageLaunch(false),
    m_firstImageLaunch(false),
    m_firstImageTook(false),
    m_moveDetected(false),
    m_imageData(NULL),
    m_imageWidth(-1),
    m_imageHeight(-1)
{
    loadFromFile();
    connect(_kinectPluginLoader, SIGNAL(newKinectEngine(IKinect*)), this, SLOT(onKinectChanged(IKinect*)));
    onKinectChanged(_kinectPluginLoader->plugin());
}

HardDriveStorage::~HardDriveStorage()
{

}


void HardDriveStorage::moveDetected(quint8 *_data, int _width, int _height)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(_data)
    if ((m_imageWidth != _width) || (m_imageHeight != _height)) {
	m_imageWidth = _width;
	m_imageHeight = _height;
	if (m_imageData) {
	    delete[] m_imageData;
	}
	m_imageData = new quint8[m_imageWidth * m_imageHeight * 3];
    }
    m_moveDetected = true;
    if (!m_firstImageLaunch) {
	m_firstImageLaunch = true;
	QTimer::singleShot(m_firstDelay, this, SLOT(saveFirstImage()));
    } else if (m_firstImageTook) {
	if (!m_nextImageLaunch) {
	    m_nextImageLaunch = true;
	    m_nextImageTimerId = startTimer(m_repeatableDelay*1000);
	}
    }
}

void HardDriveStorage::timerEvent(QTimerEvent *_evt)
{
    if (_evt->timerId() == m_nextImageTimerId) {
	saveNextImage();
    }
}

void HardDriveStorage::saveFirstImage()
{
    m_firstImageTook = true;
    saveImage();
}

void HardDriveStorage::saveNextImage()
{
    if (!m_moveDetected) {
	killTimer(m_nextImageTimerId);
	m_firstImageLaunch = false;
	m_firstImageTook = false;
	m_nextImageLaunch = false;
	return;
    }
    saveImage();
    m_moveDetected = false;
}

void HardDriveStorage::saveImage()
{
    if (!m_kinect)
	return;
    QDir dir(m_directoryPath);
    if (!dir.exists()) {
	qWarning() << Q_FUNC_INFO << " Directory path doesn't exists";
	return;
    }
    memcpy(m_imageData, m_kinect->rgbImage(), m_imageWidth*m_imageHeight*3);
    QImage image(m_imageData, m_imageWidth, m_imageHeight, QImage::Format_RGB888);
    QString fileName = dir.absolutePath().append("/").append(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz")).append(".png");
    //QDateTime dateTime = QDateTime::currentDateTime();
    //fileName.append(dateTime.
    image.save(fileName, "PNG");
}

void HardDriveStorage::loadFromFile()
{
    QSettings settings;
    QVariant var;
    settings.beginGroup("HardDriveStorage");
    var = settings.value("directory_path", QVariant(m_directoryPath));
    setDirectoryPath(var.toString());
    var = settings.value("first_delay", QVariant(m_firstDelay));
    setFirstDelay(var.toInt());
    var = settings.value("next_delay", QVariant(m_repeatableDelay));
    setRepeatableDelay(var.toDouble());
    var = settings.value("storage_active", QVariant(m_storageActive));
    setStorageActive(var.toBool());
    settings.endGroup();
}

void HardDriveStorage::saveToFile()
{
    QSettings settings;
    settings.beginGroup("HardDriveStorage");
    settings.setValue("directory_path", QVariant(m_directoryPath));
    settings.setValue("first_delay", QVariant(m_firstDelay));
    settings.setValue("next_delay", QVariant(m_repeatableDelay));
    settings.setValue("storage_active", QVariant(m_storageActive));
    settings.endGroup();
}

void HardDriveStorage::onKinectChanged(IKinect *_kinect)
{
    qDebug() << Q_FUNC_INFO;
    if (!_kinect)
	return;
    if (m_kinect)
	m_kinect->removeKinectObserver(*this);
    m_kinect = _kinect;
    m_kinect->addKinectObserver(*this);
}
