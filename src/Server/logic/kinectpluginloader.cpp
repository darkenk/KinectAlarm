#include "kinectpluginloader.h"

#include <QApplication>
#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <QSettings>

#include <QDebug>

#define KINECT_ALARM_PLUGINS_PATH "KINECT_ALARM_PLUGINS_PATH"

KinectPluginLoader::KinectPluginLoader(QObject *parent) :
    QObject(parent),
    m_pluginsNameList(new QList<QString>()),
    m_pluginsMap(new QMap<QString, IKinect*>()),
    m_kinect(0)
{
    loadSettings();
}

KinectPluginLoader::~KinectPluginLoader()
{

}

QList<QString> KinectPluginLoader::pluginsList()
{
    return *m_pluginsNameList;
}

QList<QString> KinectPluginLoader::loadPlugins()
{
    if (m_pluginsNameList->length() > 0)
	return *m_pluginsNameList;

    QList<QString> pluginsPath; // paths, where we need to search plugins
#ifndef QT_NO_DEBUG
    pluginsPath.append(QApplication::applicationDirPath());
#endif
    pluginsPath.append("/usr/share/kinectalarm/plugins");
    pluginsPath.append("/usr/local/share/kinectalarm/plugins");
    QByteArray env = qgetenv(KINECT_ALARM_PLUGINS_PATH);
    if (!env.isEmpty()) {
	pluginsPath.append(QString(env));
    }
    IKinect* kinect;
    foreach(QString path, pluginsPath) {
	QDir dir(path);
	if (!dir.exists())
	    continue;
	QList<QString> files(dir.entryList());
	foreach (QString file, files) {
	    qDebug() << Q_FUNC_INFO << "inside one " << file;
	    if (QLibrary::isLibrary(dir.absoluteFilePath(file))) {
		qDebug() << Q_FUNC_INFO << "inside two " << file;
		QPluginLoader pluginLoader(dir.absoluteFilePath(file));
		QObject* obj = pluginLoader.instance();
		qDebug() << Q_FUNC_INFO << obj;
		kinect = qobject_cast<IKinect*> (pluginLoader.instance());
		if (kinect) {
		    qDebug() << Q_FUNC_INFO << "inside three";
		    m_pluginsMap->insert(kinect->pluginName(), kinect);
		    m_pluginsNameList->append(kinect->pluginName());
		}
	    }
	}
    }
    return *m_pluginsNameList;
}

IKinect* KinectPluginLoader::plugin()
{
    return m_kinect;
}

IKinect* KinectPluginLoader::setPlugin(QString _pluginName)
{
    IKinect* newKinect = m_pluginsMap->value(_pluginName);
    if (!newKinect)
	return newKinect; // return null if no such plugin

    if ((m_kinect) && (m_kinect->pluginName() == _pluginName))
	return m_kinect;

    if (m_kinect)
	m_kinect->deinitialize();

    m_kinect = newKinect;
    emit newKinectEngine(m_kinect);
    if (!m_kinect->initialize()) {
	emit newKinectEngine(NULL);

    }
    return m_kinect;
}

bool KinectPluginLoader::loadSettings()
{
    if (m_pluginsMap->isEmpty()) {
	loadPlugins();
    }
    QSettings settings;
    settings.beginGroup("KinectPluginLoader");
    QVariant var;
    var = settings.value("kinect_plugin_name", QVariant(""));
    setPlugin(var.toString());

    settings.endGroup();
    return true;

}

void KinectPluginLoader::saveSettings()
{
    QSettings settings;
    settings.beginGroup("KinectPluginLoader");
    if (m_kinect)
	settings.setValue("kinect_plugin_name", QVariant(m_kinect->pluginName()));
    settings.endGroup();
}



