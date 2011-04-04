#ifndef OPENNIOBJECT_H
#define OPENNIOBJECT_H

#include <kinectsubject.h>
#include <interfaces/ikinectobserver.h>
#include <interfaces/ikinect.h>

#include <XnCppWrapper.h>
#include <opencv/cv.h>
#include <QObject>
#include <QMutex>
#include <QThread>
#include <QTimer>

class OpenNIObject : public QThread, public KinectSubject, public IKinect
{
    Q_OBJECT
public:
    ~OpenNIObject();

    static OpenNIObject* instance();

    quint8* depthImage();
    quint8* rgbImage();
    quint32 rgbImageWidth();
    quint32 rgbImageHeight();
    quint32 depthImageWidth();
    quint32 depthImageHeight();

    bool startGenerating();
    void pauseGenerating();
    bool initialize();


signals:

public slots:

private slots:
    void onDelayFinished();
    void onFetchDataTimer();

protected:
    void run();

private:
    void createDepthImage(quint8* _depthImage, quint16* _mask, quint16 _width, quint16 _height);

    Q_DISABLE_COPY(OpenNIObject)
    OpenNIObject(QObject *parent = 0);
    XnStatus m_xnStatus;
    xn::Context* m_context;
    xn::DepthGenerator* m_depthGenerator;
    xn::ImageGenerator* m_imageGenerator;
    xn::SceneAnalyzer* m_sceneAnalyzer;
    QList<IKinectObserver*> m_kinectObserverList;
    quint8* m_depthImage;
    quint8* m_rgbImage;
    quint16* m_sceneImage;
    QMutex m_mutex;
    xn::ImageMetaData* m_imageMetaData;
    xn::DepthMetaData* m_depthMetaData;
    xn::SceneMetaData* m_sceneMetaData;


    static OpenNIObject* m_instance;

    bool m_moveDetected;
    bool m_nextShot;
    quint16 m_imageWidth;
    quint16 m_imageHeight;

    QTimer* m_timer;
    bool m_initialized;

};

#endif // OPENNIOBJECT_H
