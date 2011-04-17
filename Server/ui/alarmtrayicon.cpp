#include "alarmtrayicon.h"
#include "settingsdialog.h"

#ifdef KinectAlarmDebug
#include "mainwindow.h"
#endif

#include <QApplication>
#include <QSettings>
#include <QDebug>

AlarmTrayIcon::AlarmTrayIcon(QObject *_parent) :
    QSystemTrayIcon(_parent),
    //m_kinect(OpenNIObject::instance()),
    m_contextMenu(new QMenu),
    m_quitAction(new QAction(tr("&Quit"), this)),
#ifdef KinectAlarmDebug
    m_debugAction(new QAction(tr("&Debug"), this)),
#endif
    m_settingsAction(new QAction(tr("&Settings"), this)),
    m_startAction(new QAction(tr("S&tart"), this)),
    m_kinectStarted(false),
    m_kinectPluginLoader(new KinectPluginLoader(this)),
    m_hardDriveStorage(new HardDriveStorage(m_kinectPluginLoader,this))
{
    connect(m_kinectPluginLoader, SIGNAL(newKinectEngine(IKinect*)), this, SLOT(onKinectPluginChange(IKinect*)));
    onKinectPluginChange(m_kinectPluginLoader->plugin());
//    QList<QString> plugins = m_kinectPluginLoader->loadPlugins();
//    if (!plugins.isEmpty()) {
//	m_pluginName = plugins.at(0);
//    } else {
//	//TODO: error message and quit the app
//    }



    setIcon(QIcon(":/KinectServer/home1.png"));
#ifdef KinectAlarmDebug
    connect(m_debugAction, SIGNAL(triggered()), this, SLOT(onDebugAction()));
    m_contextMenu->addAction(m_debugAction);
#endif
    connect(m_quitAction, SIGNAL(triggered()), this, SLOT(onQuitAction()));
    connect(m_settingsAction, SIGNAL(triggered()), this, SLOT(onSettingsAction()));
    connect(m_startAction, SIGNAL(triggered()), this, SLOT(onStartAction()));
    m_contextMenu->addAction(m_startAction);
    m_contextMenu->addAction(m_settingsAction);
    m_contextMenu->addAction(m_quitAction);

    setContextMenu(m_contextMenu);
    setToolTip(tr("Kinect Alarm Fuck Yea"));
}

AlarmTrayIcon::~AlarmTrayIcon()
{

}

void AlarmTrayIcon::onQuitAction()
{
    qDebug() << Q_FUNC_INFO;
    QApplication::quit();
}

#ifdef KinectAlarmDebug
void AlarmTrayIcon::onDebugAction()
{
    qDebug() << Q_FUNC_INFO;
    if (!m_debugWindow) {
	m_debugWindow = new MainWindow(m_kinect);
	m_debugWindow->show();
    } else {
	m_debugWindow->show();
	m_debugWindow->activateWindow();
    }
}
#endif

void AlarmTrayIcon::onSettingsAction()
{
    qDebug() << Q_FUNC_INFO;
    if (!m_settingsWindow) {
	m_settingsWindow = new SettingsDialog(m_hardDriveStorage, m_kinectPluginLoader);
	m_settingsWindow->show();
    } else {
	m_settingsWindow->show();
	m_settingsWindow->activateWindow();
    }

}

void AlarmTrayIcon::onStartAction()
{
    if (m_kinectStarted) {
	m_startAction->setText(tr("S&tart"));
	m_kinect->pauseGenerating();
    } else {
	m_startAction->setText(tr("S&top"));
	m_kinect->startGenerating();
    }
    m_kinectStarted = !m_kinectStarted;
}

void AlarmTrayIcon::onKinectPluginChange(IKinect* _kinect)
{
    m_kinect = _kinect;
}
