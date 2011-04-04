#ifndef ALARMTRAYICON_H
#define ALARMTRAYICON_H

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

private:
    QMenu* m_contextMenu;
    QAction* m_quitAction;
#ifdef KinectAlarmDebug
    QAction* m_debugAction;
    QPointer<QWidget> m_debugWindow;
#endif

};

#endif // ALARMTRAYICON_H
