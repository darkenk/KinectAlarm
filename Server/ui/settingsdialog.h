#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "harddrivestorage.h"
#include "ikinect.h"

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(IKinect* _kinect, HardDriveStorage* _hardDriveStorage, QWidget *_parent = 0);
    ~SettingsDialog();

private slots:
    void on_storageActive_toggled(bool checked);

    void on_pathChooser_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SettingsDialog *ui;
    IKinect* m_kinect;
    HardDriveStorage* m_storage;
};

#endif // SETTINGSDIALOG_H
