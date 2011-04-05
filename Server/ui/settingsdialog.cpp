#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

SettingsDialog::SettingsDialog(IKinect* _kinect, HardDriveStorage* _hardDriveStorage, QWidget *_parent) :
    QDialog(_parent),
    ui(new Ui::SettingsDialog),
    m_kinect(_kinect),
    m_storage(_hardDriveStorage)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_storageActive_toggled(bool checked)
{
    ui->firstImageSpinBox->setEnabled(checked);
    ui->label->setEnabled(checked);
    ui->label_2->setEnabled(checked);
    ui->label_3->setEnabled(checked);
    ui->label_4->setEnabled(checked);
    ui->label_5->setEnabled(checked);
    ui->nextImageSpinBox->setEnabled(checked);
    ui->path->setEnabled(checked);
    ui->pathChooser->setEnabled(checked);
}

void SettingsDialog::on_pathChooser_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose directory"), ui->path->text());
    qDebug() << Q_FUNC_INFO << dir;
    if (!dir.isEmpty()) {
	ui->path->setText(dir);
    }
}

void SettingsDialog::on_buttonBox_accepted()
{
    QDir dir(ui->path->text());
    if (!dir.exists()) {
	QMessageBox::warning(this, tr("Error"), tr("Specified image path is invalid"));
	return;
    }
    m_storage->setDirectoryPath(ui->path->text());
    m_storage->setFirstDelay(ui->firstImageSpinBox->value());
    m_storage->setRepeatableDelay(ui->nextImageSpinBox->value());
    close();
}

void SettingsDialog::on_buttonBox_rejected()
{
    close();
}
