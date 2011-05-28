#ifndef IKINECT_H
#define IKINECT_H

#include "ikinectobservable.h"

#include <QtGlobal>
#include <QtPlugin>



/**
  * Information how to write plugins :
  * http://doc.qt.nokia.com/latest/plugins-howto.html
  * You need to set KINECT_ALARM_PLUGINS_PATH variable to directory,
  * where the implemented plugin is. You can do this via:
  * export KINECT_ALARM_PLUGINS_PATH=/path/to/directory/plugin
  * after this run kinect alarm in the same terminal session typing:
  * kinectalarm
  */
/** Function from IKinectObservable implement in the same way
  * as it is done in OpenNIObject. You can copy paste this code
  */
class IKinect : public IKinectObservable  {
public: 
    virtual ~IKinect() {}

    /** returns depth image */
    virtual quint8* depthImage() = 0;
    /** returns rgb image */
    virtual quint8* rgbImage() = 0;
    /** returns rgb image height */
    virtual quint32 rgbImageHeight() = 0;
    /** returns rgb image width */
    virtual quint32 rgbImageWidth() = 0;
    /** returns depth image width */
    virtual quint32 depthImageWidth() = 0;
    /** returns depth image height */
    virtual quint32 depthImageHeight() = 0;

    /** Function start generating data (read data from kinect and process
      * them. This function is called, when user clicks "start" in menu */
    virtual bool startGenerating() = 0;
    /** Pause generating data. This function is called, when user clicks
      * "pause" in menu */
    virtual void pauseGenerating() = 0;
    /** returns whether kinect plugin is generating data */
    virtual bool isRunning() = 0;
    /** initialize plugin (all resources needed by plugin) */
    virtual bool initialize() = 0;
    /** frees resources allocated by plugin */
    virtual void deinitialize() = 0;

    /** unique name of plugin */
    virtual QString pluginName() = 0;

};

Q_DECLARE_INTERFACE(IKinect, "org.pwr.kinect.IKinect/1.0")

#endif // IKINECT_H
