#ifndef IKINECTOBSERVER_H
#define IKINECTOBSERVER_H

#include <QtGlobal>

class IKinectObserver {
public:
    virtual void moveDetected(quint8* _data, int _width = 640, int _height = 480 ) = 0;
};

#endif // IOBSERVER_H
