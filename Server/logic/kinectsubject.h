#ifndef KINECTSUBJECT_H
#define KINECTSUBJECT_H

#include <interfaces/ikinectobservable.h>
#include <interfaces/ikinectobserver.h>

#include <QList>

class KinectSubject : public IKinectObservable
{
public:
    KinectSubject();
    virtual ~KinectSubject();

    void addKinectObserver(IKinectObserver &_observer);
    bool removeKinectObserver(IKinectObserver &_observer);

protected:
    void notifyAll(quint8 *_data, int _width, int _height);

private:
    QList< IKinectObserver* > m_observersList;

};

#endif // KINECTSUBJECT_H
