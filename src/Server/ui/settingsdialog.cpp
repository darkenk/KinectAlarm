#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>

SettingsDialog::SettingsDialog(Storage* _hardDriveStorage, Storage* _picasaStorage, KinectPluginLoader* _kinectPluginLoader, QWidget *_parent) :
    QDialog(_parent),
    ui(new Ui::SettingsDialog),
    m_kinect(_kinectPluginLoader->plugin()),
    m_hardDriveStorage(_hardDriveStorage),
    m_picasaStorage(_picasaStorage),
    m_kinectPluginLoader(_kinectPluginLoader)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->path->setText(dynamic_cast<HardDriveStorage*>(m_hardDriveStorage->storageImpl())->directoryPath());
    ui->firstImageSpinBox->setValue(m_hardDriveStorage->firstDelay());
    ui->nextImageSpinBox->setValue(m_hardDriveStorage->repeatableDelay());
    ui->storageActive->setCheckState(m_hardDriveStorage->storageActive() ? Qt::Checked : Qt::Unchecked);
    QList<QString> plugins = m_kinectPluginLoader->pluginsList();

    ui->pluginComboBox->insertItems(0, plugins);
    if (plugins.length()) {
	ui->pluginComboBox->setCurrentIndex(plugins.indexOf(m_kinectPluginLoader->plugin()->pluginName()));
    } else {
	ui->pluginComboBox->setDisabled(true);
    }
    ui->picasaStorageActive->setCheckState(m_picasaStorage->storageActive() ? Qt::Checked : Qt::Unchecked);
    ui->picasaFirstImageSpinBox->setValue(m_picasaStorage->firstDelay());
    ui->picasaNextImageSpinBox->setValue(m_picasaStorage->repeatableDelay());
    PicasaStorage* ps = dynamic_cast<PicasaStorage*>(m_picasaStorage->storageImpl());
    ui->picasaLogin->setText(ps->login());
    ui->picasaPassword->setText(ps->password());


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
    m_hardDriveStorage->setStorageActive((bool)ui->storageActive->checkState());
    if (m_hardDriveStorage->storageActive()) {
	QDir dir(ui->path->text());
	if (!dir.exists()) {
	    QMessageBox::warning(this, tr("Error"), tr("Specified image path is invalid"));
	    return;
	}
	dynamic_cast<HardDriveStorage*>(m_hardDriveStorage->storageImpl())->setDirectoryPath(ui->path->text());
	m_hardDriveStorage->setFirstDelay(ui->firstImageSpinBox->value());
	m_hardDriveStorage->setRepeatableDelay(ui->nextImageSpinBox->value());
    }
    m_hardDriveStorage->saveToFile();
    dynamic_cast<HardDriveStorage*>(m_hardDriveStorage->storageImpl())->saveToFile();
    PicasaStorage* ps = dynamic_cast<PicasaStorage*>(m_picasaStorage->storageImpl());
    m_picasaStorage->setStorageActive((bool)ui->picasaStorageActive->checkState());
    if (m_picasaStorage->storageActive()) {
	m_picasaStorage->setFirstDelay(ui->picasaFirstImageSpinBox->value());
	m_picasaStorage->setRepeatableDelay(ui->picasaNextImageSpinBox->value());
	ps->setLogin(ui->picasaLogin->text());
	ps->setPassword(ui->picasaPassword->text());
    }
    m_picasaStorage->saveToFile();
    ps->saveToFile();
    if (ps->password().length() != 0)
	ps->requestAuth();
    close();
}

void SettingsDialog::on_buttonBox_rejected()
{
    close();
}

void SettingsDialog::on_picasaStorageActive_toggled(bool checked)
{
    ui->label_7->setEnabled(checked);
    ui->label_8->setEnabled(checked);
    ui->label_9->setEnabled(checked);
    ui->label_10->setEnabled(checked);
    ui->label_11->setEnabled(checked);
    ui->label_12->setEnabled(checked);
    ui->picasaFirstImageSpinBox->setEnabled(checked);
    ui->picasaNextImageSpinBox->setEnabled(checked);
    ui->picasaLogin->setEnabled(checked);
    ui->picasaPassword->setEnabled(checked);
}
