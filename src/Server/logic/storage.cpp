#include "storage.h"

#include <QSettings>
#include <QDebug>
#include <QTimer>

Storage::Storage(KinectPluginLoader* _pluginLoader, IKinectStorage* _storageImpl, QObject *parent) :
    QObject(parent),
    m_kinect(0),
    m_kinectPluginLoader(_pluginLoader),
    m_firstDelay(500),
    m_repeatableDelay(2),
    m_storageActive(false),
    m_nextImageLaunch(false),
    m_firstImageLaunch(false),
    m_firstImageTook(false),
    m_moveDetected(false),
    m_nextImageTimerId(0),
    m_imageData(0),
    m_imageWidth(-1),
    m_imageHeight(-1),
    m_storageImplementation(_storageImpl)
{
    loadFromFile();
    connect(m_kinectPluginLoader, SIGNAL(newKinectEngine(IKinect*)), this, SLOT(onKinectChanged(IKinect*)));
    onKinectChanged(m_kinectPluginLoader->plugin());
}

void Storage::moveDetected(quint8 *_data, int _width, int _height)
{
//    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(_data)
    if (!m_storageActive)
	return;
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

void Storage::timerEvent(QTimerEvent *_evt)
{
    if (_evt->timerId() == m_nextImageTimerId) {
	saveNextImage();
    }
}

void Storage::saveFirstImage()
{
    m_firstImageTook = true;
    saveImage();
}

void Storage::saveNextImage()
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

void Storage::saveImage()
{
    if (!m_kinect)
	return;
    memcpy(m_imageData, m_kinect->rgbImage(), m_imageWidth*m_imageHeight*3);
    QImage image(m_imageData, m_imageWidth, m_imageHeight, QImage::Format_RGB888);
    m_storageImplementation->saveImage(image);
}

void Storage::loadFromFile()
{
    QSettings settings;
    QVariant var;
    settings.beginGroup(m_storageImplementation->name());
    var = settings.value("first_delay", QVariant(m_firstDelay));
    setFirstDelay(var.toInt());
    var = settings.value("next_delay", QVariant(m_repeatableDelay));
    setRepeatableDelay(var.toDouble());
    var = settings.value("storage_active", QVariant(m_storageActive));
    setStorageActive(var.toBool());
    settings.endGroup();
}

void Storage::saveToFile()
{
    QSettings settings;
    settings.beginGroup(m_storageImplementation->name());
    settings.setValue("first_delay", QVariant(m_firstDelay));
    settings.setValue("next_delay", QVariant(m_repeatableDelay));
    settings.setValue("storage_active", QVariant(m_storageActive));
    settings.endGroup();
}

void Storage::onKinectChanged(IKinect *_kinect)
{
    qDebug() << Q_FUNC_INFO;
    if (!_kinect)
	return;
    if (m_kinect)
	m_kinect->removeKinectObserver(*this);
    m_kinect = _kinect;
    m_kinect->addKinectObserver(*this);
}
