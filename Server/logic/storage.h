#ifndef STORAGE_H
#define STORAGE_H

#include "ikinectstorage.h"
#include "ikinectobservable.h"
#include "ikinectobserver.h"
#include "ikinect.h"
#include "kinectpluginloader.h"

#include <QObject>
#include <QTimerEvent>

class Storage : public QObject, public IKinectObserver
{
    Q_OBJECT
public:
    Storage(KinectPluginLoader* _pluginLoader, IKinectStorage* _sotrageImpl, QObject *parent = 0);

    void setFirstDelay(int _msec) { m_firstDelay = _msec; }
    int firstDelay() { return m_firstDelay; }

    void setRepeatableDelay(double _sec) { m_repeatableDelay = _sec; }
    double repeatableDelay() { return m_repeatableDelay; }

    void loadFromFile();
    void saveToFile();

    void setStorageActive(bool _active) { m_storageActive = _active; if (m_kinect) {if (_active)  m_kinect->addKinectObserver(*this); else m_kinect->removeKinectObserver(*this);} }
    bool storageActive() { return m_storageActive; }

    IKinectStorage* storageImpl() { return m_storageImplementation; }

signals:

protected:
    void moveDetected(quint8 *_data, int _width, int _height);
    void timerEvent(QTimerEvent *_evt);

public slots:
    void onKinectChanged(IKinect* _kinect);

private slots:
    void saveFirstImage();
    void saveNextImage();

private:
    void saveImage();
    IKinect* m_kinect;
    KinectPluginLoader* m_kinectPluginLoader;
    QString m_directoryPath;
    int m_firstDelay;
    double m_repeatableDelay;
    bool m_storageActive;

    bool m_nextImageLaunch;
    bool m_firstImageLaunch;
    bool m_firstImageTook;
    bool m_moveDetected;

    int m_nextImageTimerId;

    quint8* m_imageData;
    int m_imageWidth;
    int m_imageHeight;

    IKinectStorage* m_storageImplementation;

};

#endif // STORAGE_H
