#include "openniobject.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QTimerEvent>

OpenNIObject* OpenNIObject::m_instance = NULL;

OpenNIObject::OpenNIObject(QObject *parent) :
    QThread(parent),
    m_xnStatus(XN_STATUS_OK),
    m_context(new xn::Context()),
    m_depthGenerator(new xn::DepthGenerator()),
    m_imageGenerator(new xn::ImageGenerator()),
    m_sceneAnalyzer(new xn::SceneAnalyzer()),
    m_depthImage(NULL),
    m_rgbImage(NULL),
    m_imageMetaData(new xn::ImageMetaData()),
    m_depthMetaData(new xn::DepthMetaData()),
    m_sceneMetaData(new xn::SceneMetaData()),
    m_moveDetected(false),
    m_nextShot(true),
    m_timer(new QTimer),
    m_initialized(false)
{
    m_timer->setSingleShot(false);
    m_timer->setInterval(50);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onFetchDataTimer()));
}



OpenNIObject::~OpenNIObject()
{
    m_context->Shutdown();
    delete m_context;

    if (m_rgbImage) {
	delete[] m_rgbImage;
	m_rgbImage = NULL;
    }
    if (m_depthImage) {
	delete[] m_depthImage;
	m_depthImage = NULL;
    }

    delete m_depthGenerator;
    delete m_imageGenerator;
    delete m_sceneAnalyzer;
    delete m_imageMetaData;
    delete m_depthMetaData;
    delete m_sceneMetaData;
}

OpenNIObject * OpenNIObject::instance()
{
    if (!m_instance) {
	m_instance = new OpenNIObject();
    }
    m_instance;
}

quint8* OpenNIObject::depthImage()
{
    QMutexLocker locker(&m_mutex);
    return m_depthImage;
}

quint8* OpenNIObject::rgbImage()
{
    QMutexLocker locker(&m_mutex);
    return m_rgbImage;
}



bool OpenNIObject::startGenerating()
{
    m_imageGenerator->StartGenerating();
    m_timer->start();
    start();
    return true;
}

void OpenNIObject::pauseGenerating()
{
    m_imageGenerator->StopGenerating();
    m_timer->stop();
    quit();
}

bool OpenNIObject::initialize()
{
    if (m_initialized)
	return true;
    m_initialized = true;
    qDebug() << Q_FUNC_INFO;
    xn::EnumerationErrors errors;
    XnStatus rc;
    rc = m_context->InitFromXmlFile("/home/darken/workspace/qt/KinectOpenNi/Server/config.xml", &errors);
    if (rc == XN_STATUS_NO_NODE_PRESENT) {
	XnChar strError[1024];
	errors.ToString(strError,  1024);
	qDebug() << Q_FUNC_INFO << strError;
    } else  if (rc != XN_STATUS_OK) {
	qDebug() << Q_FUNC_INFO << "Something is wrong";
    }
   rc = m_context->FindExistingNode(XN_NODE_TYPE_DEPTH, *m_depthGenerator);
    if (rc == XN_STATUS_NO_NODE_PRESENT) {
	XnChar strError[1024];
	errors.ToString(strError,  1024);
	qDebug() << Q_FUNC_INFO << strError;
    } else  if (rc != XN_STATUS_OK) {
	qDebug() << Q_FUNC_INFO << "Something is wrong";
    }
    rc = m_context->FindExistingNode(XN_NODE_TYPE_IMAGE, *m_imageGenerator);
    if (rc == XN_STATUS_NO_NODE_PRESENT) {
	XnChar strError[1024];
	errors.ToString(strError,  1024);
	qDebug() << Q_FUNC_INFO << strError;
    } else  if (rc != XN_STATUS_OK) {
	qDebug() << Q_FUNC_INFO << "Something is wrong";
    }
    rc = m_context->FindExistingNode(XN_NODE_TYPE_SCENE, *m_sceneAnalyzer);
    if (rc == XN_STATUS_NO_NODE_PRESENT) {
	XnChar strError[1024];
	errors.ToString(strError,  1024);
	qDebug() << Q_FUNC_INFO << strError;
    } else  if (rc != XN_STATUS_OK) {
	qDebug() << Q_FUNC_INFO << "Something is wrong";
    }
    m_imageGenerator->GetMetaData(*m_imageMetaData);
    m_depthGenerator->GetMetaData(*m_depthMetaData);
    m_imageWidth = m_imageMetaData->XRes();
    m_imageHeight = m_imageMetaData->YRes();
    m_rgbImage = new quint8[m_imageWidth * m_imageHeight * m_imageMetaData->BytesPerPixel()];
    qDebug() << Q_FUNC_INFO << " sceneAnalyzerSize " << m_sceneAnalyzer->GetDataSize();
    qDebug() << Q_FUNC_INFO << " imageGeneratorSize " << m_imageMetaData->DataSize() << (m_imageWidth * m_imageHeight * m_imageMetaData->BytesPerPixel());
    qDebug() << Q_FUNC_INFO << " depthGeneratorSize " << m_depthMetaData->DataSize();
    m_sceneImage = new quint16[m_sceneAnalyzer->GetDataSize()/2];
    m_depthImage = new quint8[m_imageWidth * m_imageHeight * 3];
    return true;
}

void OpenNIObject::run()
{
    qDebug() << "so we starting";
    exec();
    qDebug() << Q_FUNC_INFO << "thread finished";

}

quint32 OpenNIObject::rgbImageWidth()
{
    return m_imageWidth;
}

quint32 OpenNIObject::rgbImageHeight()
{
    return m_imageHeight;
}

quint32 OpenNIObject::depthImageWidth()
{
    return m_imageWidth;
}

quint32 OpenNIObject::depthImageHeight()
{
    return m_imageHeight;
}

void OpenNIObject::createDepthImage(quint8* _depthImage, quint16* _mask, quint16 _width, quint16 _height)
{
    qint32 s, d, m;
    m = _width * _height - 1;
    s = _width * _height * 2 - 1;
    d = _width * _height * 3 - 1;
    for (; d >=0 ; d -= 3, s -= 2, m--) {
	if (_mask[m] > 0) {
	    m_depthImage[d] = 255;
	    m_moveDetected = true;
	} else {
	    m_depthImage[d] = _depthImage[s];
	}

	m_depthImage[d-1] = _depthImage[s-1];
	m_depthImage[d-2] = 0;

    }
}

void OpenNIObject::onDelayFinished()
{
    qDebug() << Q_FUNC_INFO;
    m_nextShot = true;
}

void OpenNIObject::onFetchDataTimer()
{
    m_context->WaitOneUpdateAll(*m_imageGenerator);
    m_mutex.lock();
    m_imageGenerator->GetMetaData(*m_imageMetaData);
    m_depthGenerator->GetMetaData(*m_depthMetaData);
    //qDebug() << Q_FUNC_INFO << m_imageGenerator;
    //memcpy(m_depthImage, m_depthMetaData->Data(), m_depthMetaData->DataSize());
    //memcpy(m_sceneImage, m_sceneAnalyzer->GetLabelMap(), m_sceneAnalyzer->GetDataSize());
    createDepthImage((quint8*)m_depthMetaData->Data(), (quint16*)m_sceneAnalyzer->GetLabelMap(), m_imageWidth, m_imageHeight);
    memcpy(m_rgbImage, m_imageMetaData->Data(), m_imageMetaData->DataSize());
    if (m_moveDetected) {
	m_moveDetected = false;
	if (m_nextShot) {
	    notifyAll(m_rgbImage, m_imageWidth, m_imageHeight);
	    m_nextShot = false;
	    QTimer::singleShot(1000, this, SLOT(onDelayFinished()));
	}
    }
    m_mutex.unlock();
}
