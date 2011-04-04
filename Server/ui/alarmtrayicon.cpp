#include "alarmtrayicon.h"
#include <QApplication>
#include <QDebug>

#ifdef KinectAlarmDebug
#include "mainwindow.h"
#endif



AlarmTrayIcon::AlarmTrayIcon(QObject *_parent) :
    QSystemTrayIcon(_parent),
    m_contextMenu(new QMenu),
    m_quitAction(new QAction(tr("&Quit"), this))
#ifdef KinectAlarmDebug
    ,m_debugAction(new QAction(tr("&Debug"), this))
#endif
{
    setIcon(QIcon(":/KinectServer/home1.png"));
#ifdef KinectAlarmDebug
    connect(m_debugAction, SIGNAL(triggered()), this, SLOT(onDebugAction()));
    m_contextMenu->addAction(m_debugAction);
#endif
    connect(m_quitAction, SIGNAL(triggered()), this, SLOT(onQuitAction()));
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
	m_debugWindow = new MainWindow();
	m_debugWindow->show();
    } else {
	m_debugWindow->show();
	m_debugWindow->activateWindow();
    }
}
#endif
