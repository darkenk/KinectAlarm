#ifndef IKINECT_H
#define IKINECT_H

#include "ikinectobservable.h"

#include <QtGlobal>
#include <QtPlugin>

class IKinect : public IKinectObservable  {
public: 
    virtual ~IKinect() {}

    virtual quint8* depthImage() = 0;
    virtual quint8* rgbImage() = 0;
    virtual quint32 rgbImageHeight() = 0;
    virtual quint32 rgbImageWidth() = 0;
    virtual quint32 depthImageWidth() = 0;
    virtual quint32 depthImageHeight() = 0;

    virtual bool startGenerating() = 0;
    virtual void pauseGenerating() = 0;
    virtual bool isRunning() = 0;
    virtual bool initialize() = 0;
    virtual void deinitialize() = 0;

    virtual QString pluginName() = 0;

};

Q_DECLARE_INTERFACE(IKinect, "org.pwr.kinect.IKinect/1.0")

#endif // IKINECT_H
