#include "harddrivestorage.h"

#include <QTimer>
#include <QImage>
#include <QDir>
#include <QDateTime>
#include <QTimerEvent>
#include <QDebug>

HardDriveStorage::HardDriveStorage(IKinect* _kinect, QObject *parent) :
    QObject(parent),
    m_kinect(_kinect),
    m_directoryPath("./"),
    m_firstDelay(500),
    m_repeatableDelay(2),
    m_nextImageLaunch(false),
    m_firstImageLaunch(false),
    m_firstImageTook(false),
    m_moveDetected(false),
    m_imageData(NULL),
    m_imageWidth(-1),
    m_imageHeight(-1)
{
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
