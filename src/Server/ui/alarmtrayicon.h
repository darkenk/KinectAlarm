#ifndef ALARMTRAYICON_H
#define ALARMTRAYICON_H

#include "ikinect.h"
#include "harddrivestorage.h"
#include "storage.h"
#include "picasastorage.h"
#include "kinectpluginloader.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QWidget>
#include <QPointer>

#define KinectAlarmDebug

class AlarmTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    AlarmTrayIcon(QObject *_parent = 0);
    virtual ~AlarmTrayIcon();

signals:

public slots:

private slots:
    void onQuitAction();
#ifdef KinectAlarmDebug
    void onDebugAction();
#endif
    void onSettingsAction();
    void onStartAction();
    void onKinectPluginChange(IKinect* _kinect);

private:
    void loadSettings();

private:
    IKinect* m_kinect;
    QMenu* m_contextMenu;
    QAction* m_quitAction;
#ifdef KinectAlarmDebug
    QAction* m_debugAction;
    QPointer<QWidget> m_debugWindow;
#endif
    QAction* m_settingsAction;
    QAction* m_startAction;
    QPointer<QWidget> m_settingsWindow;

    bool m_kinectStarted;
    KinectPluginLoader* m_kinectPluginLoader;
    Storage* m_hardDriveStorage;
    Storage* m_picasaStorage;


};

#endif // ALARMTRAYICON_H
