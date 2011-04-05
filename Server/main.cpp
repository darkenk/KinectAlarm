#include "alarmtrayicon.h"

#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    AlarmTrayIcon alarmTrayIcon;
    alarmTrayIcon.show();

    return a.exec();
}
