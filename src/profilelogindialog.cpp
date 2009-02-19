/*
    ProfileLoginDialog

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "profilelogindialog.h"
#include <QKeyEvent>
#include <QSoftMenuBar>

ProfileLoginDialog::ProfileLoginDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
  ui.nameGroup->setFocusProxy(ui.nameComboBox);
	setWindowTitle(tr("Log in"));
	ui.deleteButton->setIcon(QIcon(":/icons/crystal_project/delete_user.png"));
	m_new_registered_profile = false;
	loadData();
}

ProfileLoginDialog::~ProfileLoginDialog()
{

}

void ProfileLoginDialog::enableOrDisableSignIn()
{
  if (!ui.nameComboBox->currentText().isEmpty() && !ui.passwordEdit->text().isEmpty())
    QSoftMenuBar::setLabel(this, Qt::Key_Back, QString::null, tr("Sign in"));
  else
    QSoftMenuBar::setLabel(this, Qt::Key_Back, QSoftMenuBar::NoLabel);
}

void ProfileLoginDialog::signIn()
{
	saveData();
	m_current_profile = ui.nameComboBox->currentText();
	accept();
}

void ProfileLoginDialog::saveData()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim", "qutimsettings");
	settings.setValue("general/showstart", ui.showDialogBox->isChecked());
	
	//Save new profile or existed profile's changed options
	
	//add new profile to profile list
	QStringList profiles = settings.value("profiles/list").toStringList();
	QString current_profile(ui.nameComboBox->currentText());

	//save current acount settings
	saveProfileSettings(current_profile);

	if( !profiles.contains(current_profile) )
	{
		profiles<<current_profile;
		profiles.sort();
		settings.setValue("profiles/list", profiles);
		m_new_registered_profile = true;
	}

	//set last profile on application run
	settings.setValue("profiles/last", profiles.indexOf(current_profile));
}

void ProfileLoginDialog::saveProfileSettings(const QString &profile_name)
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+profile_name, "profilesettings");
	settings.setValue("main/name",profile_name);
	settings.setValue("main/password",ui.passwordEdit->text());
	settings.setValue("main/savepass", ui.savePasswordBox->isChecked());
}

void ProfileLoginDialog::loadData()
{
	//loading profiles
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim", "qutimsettings");
	ui.nameComboBox->addItems(settings.value("profiles/list").toStringList());
	ui.nameComboBox->setCurrentIndex(settings.value("profiles/last", 0).toInt());
	ui.showDialogBox->setChecked(settings.value("general/showstart", false).toBool());
//	ui.deleteButton->setEnabled(ui.accountBox->count());
	loadProfileSettings(ui.nameComboBox->currentText());
}

void ProfileLoginDialog::loadProfileSettings(const QString &profile_name)
{
	if(ui.nameComboBox->currentText() != "")
	{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+profile_name, "profilesettings");
		if(profile_name == settings.value("main/name").toString() )
		{
			ui.savePasswordBox->setChecked(settings.value("main/savepass", true).toBool());
			if ( ui.savePasswordBox->isChecked() )
			{
				ui.passwordEdit->setText(settings.value("main/password").toString());
			}
			else
			{
				ui.passwordEdit->clear();
			}
//			ui.deleteButton->setEnabled(true);
		} else {
			ui.nameComboBox->lineEdit()->clear();
		}
	}
}

void ProfileLoginDialog::on_nameComboBox_editTextChanged(const QString &profile_name)
{
	enableOrDisableSignIn(); 
	
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim", "qutimsettings");
	QStringList accountList = settings.value("profiles/list").toStringList();
	if ( accountList.contains(profile_name) )
		loadProfileSettings(profile_name);
	else
	{
		ui.passwordEdit->clear();
		ui.savePasswordBox->setChecked(true);
//		ui.deleteButton->setEnabled(false);
	}
}

void ProfileLoginDialog::on_deleteButton_clicked()
{
	QString profile_name(ui.nameComboBox->currentText());
	QMessageBox msgBox(QMessageBox::NoIcon, tr("Delete profile"),
			tr("Delete %1 profile?").arg(profile_name), QMessageBox::Yes | QMessageBox::No, this);
	switch( msgBox.exec() )
	{
	case QMessageBox::Yes:
		removeProfile(profile_name);
		break;
	case QMessageBox::No:

		break;
	default:
		break;
	}
}

void ProfileLoginDialog::removeProfile(const QString &profile_name)
{
	ui.nameComboBox->removeItem(ui.nameComboBox->currentIndex());
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim", "qutimsettings");
	QStringList profiles_list = settings.value("profiles/list").toStringList();
	profiles_list.removeAll(profile_name);

	//delete from profile list
	if( profiles_list.count() )
		settings.setValue("profiles/list", profiles_list);
	else
		settings.remove("profiles/list");

	if(ui.nameComboBox->count() >= 0)
		settings.setValue("profiles/last", 0);
	else
		settings.remove("profiles/last");

	if( !ui.nameComboBox->count() ) ui.deleteButton->setEnabled(false);

        QSettings dir_settings_path(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+profile_name, "profilesettings");
	QDir profile_dir(dir_settings_path.fileName());
	profile_dir.cdUp();

	//delete profile directory
	if( profile_dir.exists() )
		removeProfileDir(profile_dir.path());
}

void ProfileLoginDialog::removeProfileDir(const QString &path)
{
	//recursively delete all files in directory
	QFileInfo fileInfo(path);
	if( fileInfo.isDir() )
	{
		QDir dir( path );
		QFileInfoList fileList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
		for (int i = 0; i < fileList.count(); i++)
			removeProfileDir(fileList.at(i).absoluteFilePath());
		dir.rmdir(path);
	} else {
		QFile::remove(path);
	}
}

void ProfileLoginDialog::keyPressEvent(QKeyEvent *ev)
{
  if (ev->key()==Qt::Key_Back)
  {
    if (!ui.nameComboBox->currentText().isEmpty() && !ui.passwordEdit->text().isEmpty())
      signIn();
    return;
  }
  QDialog::keyPressEvent(ev);
}
