/*
    networkSettings

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


#include "networksettings.h"

networkSettings::networkSettings(const QString &profile_name, QWidget *parent)
    : QWidget(parent),
    m_profile_name(profile_name)
{
	ui.setupUi(this);
	changed = false;
	
/*	connect( ui.typeBox, SIGNAL(currentIndexChanged(int)),
                                this, SLOT(proxyTypeChanged(int)));*/
	
	loadSettings();
	
	connect (ui.hostEdit, SIGNAL(textChanged(const QString&)),
			this, SLOT(widgetStateChanged()));
	connect (ui.portBox, SIGNAL(valueChanged(int)),
				this, SLOT(widgetStateChanged()));
	connect (ui.keepAliveBox, SIGNAL(stateChanged(int)),
				this, SLOT(widgetStateChanged()));
	connect( ui.secureBox, SIGNAL(stateChanged(int)),
				this, SLOT(widgetStateChanged()));
/*	connect (ui.typeBox , SIGNAL(currentIndexChanged(int)),
						this, SLOT(widgetStateChanged()));
	connect (ui.proxyHostEdit, SIGNAL(textChanged(const QString&)),
				this, SLOT(widgetStateChanged()));
	connect (ui.proxyPortBox, SIGNAL(valueChanged(int)),
					this, SLOT(widgetStateChanged()));
	connect (ui.authBox, SIGNAL(stateChanged(int)),
					this, SLOT(widgetStateChanged()));
	connect (ui.userNameEdit , SIGNAL(textChanged(const QString&)),
					this, SLOT(widgetStateChanged()));
	connect (ui.userPasswordEdit , SIGNAL(textChanged(const QString&)),
                                                this, SLOT(widgetStateChanged()));
	connect( ui.useProxyBox, SIGNAL(stateChanged(int)),
                                        this, SLOT(widgetStateChanged()));
	connect( ui.listenPortBox, SIGNAL(valueChanged(int)),
                                        this, SLOT(widgetStateChanged()));*/
	
}

networkSettings::~networkSettings()
{

}

void networkSettings::loadSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	ui.secureBox->setChecked(settings.value("connection/md5", true).toBool());
	ui.hostEdit->setText(settings.value("connection/host", "login.icq.com").toString());
	ui.portBox->setValue(settings.value("connection/port", 5190).toInt());
/*	ui.typeBox->setCurrentIndex(settings.value("proxy/proxyType", 0).toInt());
	ui.proxyHostEdit->setText(settings.value("proxy/host", "").toString());
	ui.proxyPortBox->setValue(settings.value("proxy/port", 1).toInt());
	ui.authBox->setChecked(settings.value("proxy/auth", false).toBool());
	if ( ui.authBox->isChecked() )
	{
		ui.userNameEdit->setEnabled(true);
		ui.userPasswordEdit->setEnabled(true);
	}
	ui.userNameEdit->setText(settings.value("proxy/user", "").toString());
        ui.userPasswordEdit->setText(settings.value("proxy/pass", "").toString());*/
	ui.keepAliveBox->setChecked(settings.value("connection/alive", true).toBool());
/*	ui.useProxyBox->setChecked(settings.value("connection/useproxy", false).toBool());
        ui.listenPortBox->setValue(settings.value("connection/listen", 5191).toUInt());*/
}

void networkSettings::saveSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	settings.setValue("connection/md5", ui.secureBox->isChecked());
	if ( ui.hostEdit->text().trimmed() != "" )
				settings.setValue("connection/host", ui.hostEdit->text());
	else
		settings.remove("connection/host");
	settings.setValue("connection/port", ui.portBox->value());
/*	settings.setValue("proxy/proxyType", ui.typeBox->currentIndex());
			
	if ( ui.proxyHostEdit->isEnabled() && ui.proxyHostEdit->text().trimmed() != "" )
		settings.setValue("proxy/host", ui.proxyHostEdit->text());
	else
		settings.remove("proxy/host");
			
	if ( ui.proxyPortBox->isEnabled() )
		settings.setValue("proxy/port", ui.proxyPortBox->value());
	else
		settings.remove("proxy/port");
	if ( ui.authBox->isChecked() )
	{
		settings.setValue("proxy/auth", true);
		settings.setValue("proxy/user", ui.userNameEdit->text());
		settings.setValue("proxy/pass", ui.userPasswordEdit->text());
	} else {
		settings.remove("proxy/auth");
		settings.remove("proxy/user");
		settings.remove("proxy/pass");
        }*/
			
	settings.setValue("connection/alive", ui.keepAliveBox->isChecked());
/*	settings.setValue("connection/useproxy", ui.useProxyBox->isChecked());
        settings.setValue("connection/listen", ui.listenPortBox->value());*/
	if ( changed )
		emit settingsSaved();
	changed = false;
}

/*void networkSettings::proxyTypeChanged(int index)
{
	if ( index )
	{
		ui.proxyHostEdit->setEnabled(true);
		ui.proxyPortBox->setEnabled(true);
		ui.authBox->setEnabled(true);
	} else {
		ui.proxyHostEdit->setEnabled(false);
		ui.proxyPortBox->setEnabled(false);
		ui.authBox->setEnabled(false);
		ui.authBox->setChecked(false);
	}
}*/
