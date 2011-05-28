#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "harddrivestorage.h"
#include "kinectpluginloader.h"
#include "ikinect.h"
#include "storage.h"
#include "picasastorage.h"
#include "scriptrunner.h"

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Storage* _hardDriveStorage, Storage* _picasaStorage, Storage* _scriptRunner, KinectPluginLoader* _kinectPluginLoader, QWidget *_parent = 0);
    ~SettingsDialog();

private slots:
    void on_storageActive_toggled(bool checked);

    void on_pathChooser_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_picasaStorageActive_toggled(bool checked);

    void onKinectPluginChange(int index);

    void on_scriptLauncherActive_toggled(bool checked);

    void on_scriptPathChooser_clicked();

private:
    Ui::SettingsDialog *ui;
    IKinect* m_kinect;
    Storage* m_hardDriveStorage;
    Storage* m_picasaStorage;
    Storage* m_scriptRunner;
    KinectPluginLoader* m_kinectPluginLoader;

    int m_currentPluginIndex;
};

#endif // SETTINGSDIALOG_H
