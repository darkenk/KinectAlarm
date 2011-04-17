#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>

SettingsDialog::SettingsDialog(HardDriveStorage* _hardDriveStorage, KinectPluginLoader* _kinectPluginLoader, QWidget *_parent) :
    QDialog(_parent),
    ui(new Ui::SettingsDialog),
    m_kinect(_kinectPluginLoader->plugin()),
    m_storage(_hardDriveStorage),
    m_kinectPluginLoader(_kinectPluginLoader)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->path->setText(m_storage->directoryPath());
    ui->firstImageSpinBox->setValue(m_storage->firstDelay());
    ui->nextImageSpinBox->setValue(m_storage->repeatableDelay());
    ui->storageActive->setCheckState(m_storage->storageActive() ? Qt::Checked : Qt::Unchecked);
    QList<QString> plugins = m_kinectPluginLoader->pluginsList();
    ui->pluginComboBox->insertItems(0, plugins);

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
    m_kinect = m_kinectPluginLoader->setPlugin(ui->pluginComboBox->currentText());
    m_kinectPluginLoader->saveSettings();
    m_storage->setStorageActive((bool)ui->storageActive->checkState());
    if (m_storage->storageActive()) {
	QDir dir(ui->path->text());
	if (!dir.exists()) {
	    QMessageBox::warning(this, tr("Error"), tr("Specified image path is invalid"));
	    return;
	}
	m_storage->setDirectoryPath(ui->path->text());
	m_storage->setFirstDelay(ui->firstImageSpinBox->value());
	m_storage->setRepeatableDelay(ui->nextImageSpinBox->value());
    }
    m_storage->saveToFile();
    close();
}

void SettingsDialog::on_buttonBox_rejected()
{
    close();
}
