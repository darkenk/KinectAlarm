#ifndef ALARMTRAYICON_H
#define ALARMTRAYICON_H

#include "ikinect.h"
#include "harddrivestorage.h"
#include "storage.h"
#include "picasastorage.h"
#include "scriptrunner.h"
#include "kinectpluginloader.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QWidget>
#include <QPointer>

class AlarmTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    AlarmTrayIcon(QObject *_parent = 0);
    virtual ~AlarmTrayIcon();

private slots:
    void onQuitAction();
    void onDebugAction();
    void onSettingsAction();
    void onStartAction();
    void onKinectPluginChange(IKinect* _kinect);
    void onMessageSignal(QString _type, QString _msg);
    void onKinectPluginRunningChange(bool _running);

private:
    void loadSettings();

private:
    IKinect* m_kinect;
    QMenu* m_contextMenu;
    QAction* m_quitAction;
    QAction* m_previewAction;
    QPointer<QWidget> m_previewWindow;
    QAction* m_settingsAction;
    QAction* m_startAction;
    QPointer<QWidget> m_settingsWindow;

    bool m_kinectStarted;
    KinectPluginLoader* m_kinectPluginLoader;
    Storage* m_hardDriveStorage;
    Storage* m_picasaStorage;
    Storage* m_scriptRunner;


};

#endif // ALARMTRAYICON_H
