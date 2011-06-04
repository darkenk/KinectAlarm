#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QProcess>

#include "../kinectglobal.h"

SettingsDialog::SettingsDialog(Storage* _hardDriveStorage, Storage* _picasaStorage, Storage* _scriptRunner, KinectPluginLoader* _kinectPluginLoader, QWidget *_parent) :
    QDialog(_parent),
    ui(new Ui::SettingsDialog),
    m_kinect(_kinectPluginLoader->plugin()),
    m_hardDriveStorage(_hardDriveStorage),
    m_picasaStorage(_picasaStorage),
    m_scriptRunner(_scriptRunner),
    m_kinectPluginLoader(_kinectPluginLoader),
    m_currentPluginIndex(-1)

{
    BEGIN;
    ui->setupUi(this);

    ui->warning->setVisible(false);
    setAttribute(Qt::WA_DeleteOnClose);

    // hdd storage part
    ui->path->setText(dynamic_cast<HardDriveStorage*>(m_hardDriveStorage->storageImpl())->directoryPath());
    ui->firstImageSpinBox->setValue(m_hardDriveStorage->firstDelay());
    ui->nextImageSpinBox->setValue(m_hardDriveStorage->repeatableDelay());
    ui->storageActive->setCheckState(m_hardDriveStorage->storageActive() ? Qt::Checked : Qt::Unchecked);
    QList<QString> plugins = m_kinectPluginLoader->pluginsList();


    ui->pluginComboBox->insertItems(0, plugins);
    if (plugins.length()) {
	if (m_kinectPluginLoader->plugin()) {
	    m_currentPluginIndex = plugins.indexOf(m_kinectPluginLoader->plugin()->pluginName());
	} else {
	    m_currentPluginIndex = 0;
	}
	ui->pluginComboBox->setCurrentIndex(m_currentPluginIndex);
	connect(ui->pluginComboBox, SIGNAL(currentIndexChanged(int)), SLOT(onKinectPluginChange(int)));
    } else {
	ui->pluginComboBox->setDisabled(true);
    }

    // picasa storage part
    ui->picasaStorageActive->setCheckState(m_picasaStorage->storageActive() ? Qt::Checked : Qt::Unchecked);
    ui->picasaFirstImageSpinBox->setValue(m_picasaStorage->firstDelay());
    ui->picasaNextImageSpinBox->setValue(m_picasaStorage->repeatableDelay());
    PicasaStorage* ps = dynamic_cast<PicasaStorage*>(m_picasaStorage->storageImpl());
    ui->picasaLogin->setText(ps->login());
    ui->picasaPassword->setText(ps->password());

    // script runner part
    ui->scriptLauncherActive->setCheckState(m_scriptRunner->storageActive() ? Qt::Checked : Qt::Unchecked);
    ui->firstLaunchSpinBox->setValue(m_scriptRunner->firstDelay());
    ui->nextLaunchSpinBox->setValue(m_scriptRunner->repeatableDelay());
    ui->scriptPath->setText(dynamic_cast<ScriptRunner*>(m_scriptRunner->storageImpl())->appPath());

    QSettings settings;
    settings.beginGroup("Global");
    QVariant var = settings.value("start_after_launch", QVariant(false));
    ui->launchKinectAfterStart->setChecked(var.toBool());
    settings.endGroup();
    END;

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_storageActive_toggled(bool checked)
{
    BEGIN;
    ui->firstImageSpinBox->setEnabled(checked);
    ui->label->setEnabled(checked);
    ui->label_2->setEnabled(checked);
    ui->label_3->setEnabled(checked);
    ui->label_4->setEnabled(checked);
    ui->label_5->setEnabled(checked);
    ui->nextImageSpinBox->setEnabled(checked);
    ui->path->setEnabled(checked);
    ui->pathChooser->setEnabled(checked);
    END;
}

void SettingsDialog::on_pathChooser_clicked()
{
    BEGIN;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose directory"), ui->path->text());
    INFO(dir);
    if (!dir.isEmpty()) {
	ui->path->setText(dir);
    }
    END;
}

void SettingsDialog::on_buttonBox_accepted()
{
    BEGIN;
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

    m_scriptRunner->setStorageActive((bool)ui->scriptLauncherActive->checkState());
    ScriptRunner* sr = dynamic_cast<ScriptRunner*>(m_scriptRunner->storageImpl());
    if (m_scriptRunner->storageActive()) {
	m_scriptRunner->setFirstDelay(ui->firstLaunchSpinBox->value());
	m_scriptRunner->setRepeatableDelay(ui->nextLaunchSpinBox->value());
	sr->setAppPath(ui->scriptPath->text());

    }
    m_scriptRunner->saveToFile();
    sr->saveToFile();


    QSettings settings;
    settings.beginGroup("Global");
    QVariant var(ui->launchKinectAfterStart->isChecked());
    settings.setValue("start_after_launch", var); //read data is done in alarmtrayicons' ctor
    settings.endGroup();
    if (m_currentPluginIndex != ui->pluginComboBox->currentIndex()) {
	QProcess::startDetached(QApplication::applicationFilePath());
	QApplication::quit();
    }
    close();
    END;
}

void SettingsDialog::on_buttonBox_rejected()
{
    BEGIN;
    close();
    END;
}

void SettingsDialog::on_picasaStorageActive_toggled(bool checked)
{
    BEGIN;
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
    END;
}

void SettingsDialog::onKinectPluginChange(int index)
{
    BEGIN;
    ui->warning->setVisible(index != m_currentPluginIndex);
    END;
}

void SettingsDialog::on_scriptLauncherActive_toggled(bool checked)
{
    BEGIN;
    ui->label_13->setEnabled(checked);
    ui->label_14->setEnabled(checked);
    ui->label_15->setEnabled(checked);
    ui->label_16->setEnabled(checked);
    ui->label_17->setEnabled(checked);
    ui->nextLaunchSpinBox->setEnabled(checked);
    ui->firstLaunchSpinBox->setEnabled(checked);
    ui->scriptPath->setEnabled(checked);
    ui->scriptPathChooser->setEnabled(checked);

    END;
}

void SettingsDialog::on_scriptPathChooser_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Choose script to run"), ui->scriptPath->text());
    INFO(file);
    if (!file.isEmpty()) {
	ui->scriptPath->setText(file);
    }
}
