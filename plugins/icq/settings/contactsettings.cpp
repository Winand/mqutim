/*
    ContactSettings

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

#include "contactsettings.h"
#include <QSettings>

ContactSettings::ContactSettings(const QString &profile_name, QWidget *parent)
    : QWidget(parent),
    m_profile_name(profile_name)
{
	ui.setupUi(this);
	changed = false;
	loadSettings();
	connect( ui.xstatusBox, SIGNAL(stateChanged(int)),
				this, SLOT(widgetStateChanged()));
	connect( ui.birthBox, SIGNAL(stateChanged(int)),
				this, SLOT(widgetStateChanged()));
	connect( ui.authBox, SIGNAL(stateChanged(int)),
				this, SLOT(widgetStateChanged()));
	connect( ui.visBox, SIGNAL(stateChanged(int)),
				this, SLOT(widgetStateChanged()));
	connect( ui.invisBox, SIGNAL(stateChanged(int)),
				this, SLOT(widgetStateChanged()));
	connect( ui.ignoreBox, SIGNAL(stateChanged(int)),
				this, SLOT(widgetStateChanged()));
	connect( ui.xstatusTextBox, SIGNAL(stateChanged(int)),
				this, SLOT(widgetStateChanged()));
}

ContactSettings::~ContactSettings()
{

}

void ContactSettings::loadSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	settings.beginGroup("contacts");
	ui.xstatusBox->setChecked(settings.value("xstaticon", true).toBool());
	ui.birthBox->setChecked(settings.value("birthicon", true).toBool());
	ui.authBox->setChecked(settings.value("authicon", true).toBool());
	ui.visBox->setChecked(settings.value("visicon", true).toBool());
	ui.invisBox->setChecked(settings.value("invisicon", true).toBool());
	ui.ignoreBox->setChecked(settings.value("ignoreicon", true).toBool());
	ui.xstatusTextBox->setChecked(settings.value("xstattext", true).toBool());
	settings.endGroup();
}

void ContactSettings::saveSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	settings.beginGroup("contacts");
	settings.setValue("xstaticon", ui.xstatusBox->isChecked());
	settings.setValue("birthicon", ui.birthBox->isChecked());
	settings.setValue("authicon", ui.authBox->isChecked());
	settings.setValue("visicon", ui.visBox->isChecked());
	settings.setValue("invisicon", ui.invisBox->isChecked());
	settings.setValue("ignoreicon", ui.ignoreBox->isChecked());
	settings.setValue("xstattext", ui.xstatusTextBox->isChecked());
	settings.endGroup();
	if ( changed )
		emit settingsSaved();
	changed = false;
}
