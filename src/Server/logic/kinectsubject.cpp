#include "kinectsubject.h"

KinectSubject::KinectSubject()
{
}

KinectSubject::~KinectSubject()
{
}

void KinectSubject::addKinectObserver(IKinectObserver &_observer)
{
    m_observersList.append(&_observer);
}

bool KinectSubject::removeKinectObserver(IKinectObserver &_observer)
{
    return m_observersList.removeOne(&_observer);
}

void KinectSubject::notifyAll(quint8 *_data, int _width, int _height)
{
    foreach (IKinectObserver *observer, m_observersList) {
	observer->moveDetected(_data, _width, _height);
    }
}
