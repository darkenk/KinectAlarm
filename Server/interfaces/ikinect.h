#ifndef IKINECT_H
#define IKINECT_H

#include <QtGlobal>

class IKinect {
public:
    virtual quint8* depthImage() = 0;
    virtual quint8* rgbImage() = 0;
    virtual quint32 rgbImageHeight() = 0;
    virtual quint32 rgbImageWidth() = 0;
    virtual quint32 depthImageWidth() = 0;
    virtual quint32 depthImageHeight() = 0;

    virtual bool startGenerating() = 0;
    virtual void pauseGenerating() = 0;
    virtual bool initialize() = 0;

};

#endif // IKINECT_H
