#ifndef KINECT_H
#define KINECT_H

#include <ikinectobserver.h>
#include <ikinect.h>

#include <libfreenect.h>
#include <QObject>
#include <QMutex>
#include <QThread>

class Kinect : public QThread, public IKinect
{
    Q_OBJECT
public:
    Kinect(QObject *parent = 0);
    ~Kinect();

    //static Kinect* instance();

    static void video_cb(freenect_device *dev, void *rgb, uint32_t timestamp);
    static void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp);

    void setLedColor(freenect_led_options ledColor);

    quint8* depthImage();
    quint8* rgbImage();
    quint32 rgbImageWidth();
    quint32 rgbImageHeight();
    quint32 depthImageWidth();
    quint32 depthImageHeight();

    bool startGenerating();
    void pauseGenerating();
    bool initialize();
    void deinitialize();

    void addKinectObserver(IKinectObserver &_observer);
    bool removeKinectObserver(IKinectObserver &_observer);
    QString pluginName();


signals:

public slots:

protected:
    void run();
    void notifyAll(quint8 *_data, int _width, int _height);

private:
    void videoCallback(freenect_device *dev, void *rgb, uint32_t timestamp);
    void depthCallback(freenect_device *dev, void *depth, uint32_t timestamp);

    Q_DISABLE_COPY(Kinect)

    freenect_context* m_ctx;
    freenect_device* m_dev;
    QMutex m_mutex;
    uint8_t* m_backVideoBuffer;
    uint8_t* m_midVideoBuffer;
    uint8_t* m_frontVideoBuffer;

    uint8_t* m_midDepth;
    uint8_t* m_frontDepth;

    bool m_die;

    QList< IKinectObserver* > m_observersList;

    static Kinect* m_instance;
};

#endif // KINECT_H
