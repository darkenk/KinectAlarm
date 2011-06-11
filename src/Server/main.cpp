#include "alarmtrayicon.h"

#include <QtGui/QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    QApplication::setApplicationName("KinectAlarm");
    QApplication::setOrganizationName("KinectPWR");

    AlarmTrayIcon alarmTrayIcon;
    alarmTrayIcon.show();
    alarmTrayIcon.showMessage("KinectAlarm", "KinectAlarm has started");

    qDebug() << "applicationStarted";
    return a.exec();
}
