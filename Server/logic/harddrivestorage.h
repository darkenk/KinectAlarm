#ifndef HARDDRIVESTORAGE_H
#define HARDDRIVESTORAGE_H

#include "ikinectobserver.h"
#include "ikinectobservable.h"
#include "ikinect.h"

#include <QObject>
#include <QString>

class HardDriveStorage : public QObject, public IKinectObserver
{
    Q_OBJECT
public:
    HardDriveStorage(IKinect* _kinect, IKinectObservable* _kinectObservable, QObject *parent = 0);
    virtual ~HardDriveStorage();

    void setDirectoryPath(const QString& _path) { m_directoryPath = _path; }
    QString directoryPath() { return m_directoryPath; }

    void setFirstDelay(int _msec) { m_firstDelay = _msec; }
    int firstDelay() { return m_firstDelay; }

    void setRepeatableDelay(double _sec) { m_repeatableDelay = _sec; }
    double repeatableDelay() { return m_repeatableDelay; }

    void loadFromFile();
    void saveToFile();

    void setStorageActive(bool _active) { m_storageActive = _active; if (_active)  m_kinectObservable->addKinectObserver(*this); else m_kinectObservable->removeKinectObserver(*this); }
    bool storageActive() { return m_storageActive; }


signals:

protected:
    void moveDetected(quint8 *_data, int _width, int _height);
    void timerEvent(QTimerEvent *_evt);

public slots:
private slots:
    void saveFirstImage();
    void saveNextImage();

private:
    void saveImage();
    IKinect* m_kinect;
    IKinectObservable* m_kinectObservable;
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

};

#endif // HARDDRIVESTORAGE_H

