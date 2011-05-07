#include "alarmtrayicon.h"

#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    QApplication::setApplicationName("KinectAlarm");
    QApplication::setOrganizationName("KinectPWR");

    AlarmTrayIcon alarmTrayIcon;
    alarmTrayIcon.show();

    return a.exec();
}
