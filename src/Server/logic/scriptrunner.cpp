#include "scriptrunner.h"
#include "kinectglobal.h"
#include <QProcess>
#include <QSettings>
#include <QFile>

ScriptRunner::ScriptRunner(QObject* parent) :
    QObject(parent)
{
    BEGIN;
    loadFromFile();
    END;
}

void ScriptRunner::saveImage(const QImage &_image)
{
    BEGIN;
    Q_UNUSED(_image)
    QFile file(m_appPath);
    if (file.exists())
	QProcess::startDetached(m_appPath);
    END;
}

void ScriptRunner::saveToFile()
{
    BEGIN;
    QSettings settings;
    settings.beginGroup(name());
    settings.setValue("app_path", QVariant(m_appPath));
    settings.endGroup();
    END;
}

void ScriptRunner::loadFromFile()
{
    BEGIN;
    QSettings settings;
    QVariant var;
    settings.beginGroup(name());
    var = settings.value("app_path", QVariant(m_appPath));
    setAppPath(var.toString());
    settings.endGroup();
    END;
}
