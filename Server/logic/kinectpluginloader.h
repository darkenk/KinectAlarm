#ifndef KINECTPLUGINLOADER_H
#define KINECTPLUGINLOADER_H

#include "ikinect.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

class KinectPluginLoader : public QObject
{
    Q_OBJECT
public:
    KinectPluginLoader(QObject *parent = 0);
    virtual ~KinectPluginLoader();

    QList<QString> loadPlugins();
    QList<QString> pluginsList();

    IKinect* plugin();
    IKinect* setPlugin(QString _pluginName);

    bool loadSettings();
    void saveSettings();



signals:
    void newKinectEngine(IKinect* _kinect);

public slots:

private:
    QList<QString>* m_pluginsNameList;
    QMap<QString, IKinect*>*  m_pluginsMap;
    IKinect* m_kinect;


};

#endif // KINECTPLUGINLOADER_H
