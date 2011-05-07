#include "kinect.h"
#include <QDebug>

#include "../../kinectglobal.h"

Kinect* Kinect::m_instance = 0;

//Kinect* Kinect::instance()
//{
//    if (!m_instance) {
//	m_instance = new Kinect();
//    }
//    return m_instance;

//}


Kinect::Kinect(QObject *parent) :
    QThread(parent)
{
    if (!m_instance) {
	m_dataProcessor = new DataProcessor();
	connect(m_dataProcessor, SIGNAL(nonZerosPixelsChanged(int)), SLOT(onZerosCount(int)));
	m_die = false;
	m_instance = this;
    }
}

Kinect::~Kinect()
{
    //TODO: check if the memeory is released there
}


void Kinect::setLedColor(freenect_led_options ledColor)
{
    freenect_set_led(m_dev, ledColor);
}

quint8* Kinect::rgbImage()
{
    QMutexLocker locker(&m_mutex);
    return m_midVideoBuffer;
}

quint8* Kinect::depthImage()
{
    QMutexLocker locker(&m_mutex);
    return m_midDepth;
}

void Kinect::videoCallback(freenect_device *dev, void *rgb, uint32_t timestamp)
{
    //qDebug() << Q_FUNC_INFO;
    QMutexLocker locker (&m_mutex);
    m_backVideoBuffer = m_midVideoBuffer;
    freenect_set_video_buffer(dev, m_backVideoBuffer);
    m_midVideoBuffer = (uint8_t*)rgb;
    m_dataProcessor->processImage(m_midVideoBuffer);


}

void Kinect::depthCallback(freenect_device *dev, void *depth, uint32_t timestamp)
{
    QMutexLocker locker(&m_mutex);
    uint16_t* u_depth = (uint16_t*)depth;
    uint8_t val;
    for (int i=0; i < FREENECT_FRAME_PIX; i++) {
	val = u_depth[i];
	m_midDepth[3*i+0] = 128;
	m_midDepth[3*i+1] = val;
	m_midDepth[3*i+2] = 128;
    }


}



void Kinect::video_cb(freenect_device *dev, void *rgb, uint32_t timestamp)
{
    m_instance->videoCallback(dev, rgb, timestamp);
}

void Kinect::depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp)
{
    m_instance->depthCallback(dev, v_depth, timestamp);
}

void Kinect::run()
{
    freenect_set_tilt_degs(m_dev, 0);
    freenect_set_led(m_dev,LED_RED);
    freenect_set_depth_callback(m_dev, depth_cb);
    freenect_set_video_callback(m_dev, video_cb);
    freenect_set_video_format(m_dev, FREENECT_VIDEO_RGB);
    freenect_set_depth_format(m_dev, FREENECT_DEPTH_11BIT);
    freenect_set_video_buffer(m_dev, m_backVideoBuffer);

    freenect_start_depth(m_dev);
    freenect_start_video(m_dev);

    while (!m_die && freenect_process_events(m_ctx) >= 0) {
	    //Throttle the text output
//	    if (accelCount++ >= 2000)
//	    {
//		    accelCount = 0;
//		    freenect_raw_tilt_state* state;
//		    freenect_update_tilt_state(f_dev);
//		    state = freenect_get_tilt_state(f_dev);
//		    double dx,dy,dz;
//		    freenect_get_mks_accel(state, &dx, &dy, &dz);
//		    printf("\r raw acceleration: %4d %4d %4d  mks acceleration: %4f %4f %4f", state->accelerometer_x, state->accelerometer_y, state->accelerometer_z, dx, dy, dz);
//		    fflush(stdout);
//	    }

//	    if (requested_format != current_format) {
//		    freenect_stop_video(f_dev);
//		    freenect_set_video_format(f_dev, requested_format);
//		    freenect_start_video(f_dev);
//		    current_format = requested_format;
//	    }
    }

    freenect_stop_depth(m_dev);
    freenect_stop_video(m_dev);
}

bool Kinect::initialize()
{
    BEGIN;
    Q_ASSERT(freenect_init(&m_ctx, NULL) >= 0);
    freenect_set_log_level(m_ctx, FREENECT_LOG_DEBUG);
    int devices = freenect_num_devices(m_ctx);
    qDebug() << "Number of devices " << devices;

    if (freenect_open_device(m_ctx, &m_dev, 0) < 0) {
	return false;
    }

    m_backVideoBuffer = new uint8_t[640*480*3];
    m_midVideoBuffer = new uint8_t[640*480*3];
    m_frontVideoBuffer = new uint8_t[640*480*3];

    m_midDepth = new uint8_t[640*480*3];
    END;
    return true;
}

void Kinect::deinitialize()
{
    pauseGenerating();
    freenect_close_device(m_dev);
    freenect_shutdown(m_ctx);

    delete m_backVideoBuffer;
    delete m_midVideoBuffer;
    delete m_frontVideoBuffer;
    delete m_midDepth;
}

quint32 Kinect::rgbImageWidth()
{
    return 640;
}

quint32 Kinect::rgbImageHeight()
{
    return 480;
}

quint32 Kinect::depthImageWidth()
{
    return 640;
}

quint32 Kinect::depthImageHeight()
{
    return 480;
}

bool Kinect::startGenerating()
{
    m_die = false;
    start();
    return true;

}


void Kinect::pauseGenerating()
{
    m_die = true;
}

QString Kinect::pluginName()
{
    return "Freenect plugin";
}

void Kinect::addKinectObserver(IKinectObserver &_observer)
{
    m_observersList.append(&_observer);
}

bool Kinect::removeKinectObserver(IKinectObserver &_observer)
{
    return m_observersList.removeOne(&_observer);
}

void Kinect::notifyAll(quint8 *_data, int _width, int _height)
{
    foreach (IKinectObserver *observer, m_observersList) {
	observer->moveDetected(_data, _width, _height);
    }
}

void Kinect::onZerosCount(int zeros)
{
    INFO(zeros);
    if (zeros>40) {
	notifyAll(rgbImage(), rgbImageWidth(), rgbImageHeight());
    }
}

Q_EXPORT_PLUGIN2(freenect_plugin, Kinect)
