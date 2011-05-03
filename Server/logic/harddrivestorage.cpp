#include "harddrivestorage.h"

#include <QTimer>
#include <QImage>
#include <QDir>
#include <QDateTime>
#include <QTimerEvent>
#include <QSettings>
#include <QVariant>
#include <QDebug>

HardDriveStorage::HardDriveStorage(QObject *_parent) :
    QObject(_parent),
    m_directoryPath("./")
{
    loadFromFile();
}

HardDriveStorage::~HardDriveStorage()
{

}

void HardDriveStorage::saveImage(const QImage& _img)
{
    QDir dir(m_directoryPath);
    if (!dir.exists()) {
	qWarning() << Q_FUNC_INFO << " Directory path doesn't exists";
	return;
    }
    QString fileName = dir.absolutePath().append("/").append(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz")).append(".png");
    //QDateTime dateTime = QDateTime::currentDateTime();
    //fileName.append(dateTime.
    _img.save(fileName, "PNG");
}

void HardDriveStorage::loadFromFile()
{
    QSettings settings;
    QVariant var;
    settings.beginGroup(name());
    var = settings.value("directory_path", QVariant(m_directoryPath));
    setDirectoryPath(var.toString());
    settings.endGroup();
}

void HardDriveStorage::saveToFile()
{
    QSettings settings;
    settings.beginGroup(name());
    settings.setValue("directory_path", QVariant(m_directoryPath));
    settings.endGroup();
}


