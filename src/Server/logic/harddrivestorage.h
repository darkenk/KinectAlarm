#ifndef HARDDRIVESTORAGE_H
#define HARDDRIVESTORAGE_H

#include "ikinectobserver.h"
#include "ikinectobservable.h"
#include "ikinect.h"
#include "kinectpluginloader.h"
#include "ikinectstorage.h"

#include <QObject>

class HardDriveStorage : public QObject, public IKinectStorage
{
    Q_OBJECT
public:
    HardDriveStorage(QObject *_parent = 0);
    virtual ~HardDriveStorage();

    void setDirectoryPath(const QString& _path) { m_directoryPath = _path; }
    QString directoryPath() { return m_directoryPath; }

    QString name() { return "HardDriveStorage"; }
    void saveImage(const QImage& _image);

    void saveToFile();
    void loadFromFile();

signals:

private:
    QString m_directoryPath;

};

#endif // HARDDRIVESTORAGE_H

