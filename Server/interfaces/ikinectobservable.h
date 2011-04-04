#ifndef IKINECTSUBJECT_H
#define IKINECTSUBJECT_H

#include <QtGlobal>

class IKinectObserver;

class IKinectObservable {
public:
    virtual void addKinectObserver(IKinectObserver& _observer) = 0;
    virtual bool removeKinectObserver(IKinectObserver& _observer) = 0;

protected:
    virtual void notifyAll(quint8* _data, int _width = 640, int _height = 480) = 0;

};

#endif // IKINECTSUBJECT_H
