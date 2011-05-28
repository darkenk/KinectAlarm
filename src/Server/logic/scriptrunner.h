#ifndef SCRIPTRUNNER_H
#define SCRIPTRUNNER_H

#include "ikinectstorage.h"
#include <QObject>

class ScriptRunner : public QObject, public IKinectStorage
{
    Q_OBJECT
public:
    explicit ScriptRunner(QObject *parent = 0);

    void setAppPath(const QString& _appPath) { m_appPath = _appPath; }
    QString appPath() { return m_appPath; }


    QString name() { return "ScriptRunner"; }
    void saveImage(const QImage &_image);

    void saveToFile();
    void loadFromFile();

private:
    QString m_appPath;
};

#endif // SCRIPTRUNNER_H
