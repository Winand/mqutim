/*
    icqSettings

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

#include <QDebug>
#include "icqsettings.h"

icqSettings::icqSettings(const QString &profile_name,
		QWidget *parent)
    : QWidget(parent),
    m_profile_name(profile_name)
{
	ui.setupUi(this);
	changed = false;
	QRegExp rx("[ABCDEFabcdef0123456789]{32,32}");
	QValidator *validator = new QRegExpValidator(rx, this);
	ui.capEdit1->setValidator(validator);
	ui.capEdit2->setValidator(validator);
	ui.capEdit3->setValidator(validator);
	
	loadSettings();
	
	connect( ui.autoBox, SIGNAL(stateChanged(int)),
			this, SLOT(widgetStateChanged()));
	connect( ui.saveStatusOnExitBox, SIGNAL(stateChanged(int)),
				this, SLOT(widgetStateChanged()));
	connect( ui.reconnectBox, SIGNAL(stateChanged(int)),
					this, SLOT(widgetStateChanged()));
	connect( ui.clientComboBox, SIGNAL(currentIndexChanged(int)),
						this, SLOT(widgetStateChanged()));
	connect( ui.clientComboBox, SIGNAL(currentIndexChanged(int)),
							this, SLOT(clientIndexChanged(int)));
	connect( ui.codepageBox, SIGNAL(currentIndexChanged(int)),
							this, SLOT(widgetStateChanged()));
	connect( ui.protocolNumBox, SIGNAL(valueChanged(int)),
						this, SLOT(widgetStateChanged()));
	connect( ui.capEdit1, SIGNAL(textEdited ( const QString & )),
						this, SLOT(widgetStateChanged()));
	connect( ui.capEdit2, SIGNAL(textEdited ( const QString & )),
						this, SLOT(widgetStateChanged()));
	connect( ui.capEdit3, SIGNAL(textEdited ( const QString & )),
						this, SLOT(widgetStateChanged()));
	connect(ui.mainIconRadio,SIGNAL(toggled(bool)),
				this, SLOT(widgetStateChanged()));
	connect(ui.xIconRadio,SIGNAL(toggled(bool)),
				this, SLOT(widgetStateChanged()));
	connect(ui.lastIconRadio,SIGNAL(toggled(bool)),
				this, SLOT(widgetStateChanged()));
}

icqSettings::~icqSettings()
{

}

void icqSettings::loadSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	bool autoC = settings.value("connection/auto", true).toBool();
	ui.autoBox->setChecked(autoC);
        if ( autoC ) ui.saveStatusOnExitBox->setChecked(settings.value("connection/statonexit", true).toBool());
	ui.reconnectBox->setChecked(settings.value("connection/reconnect", true).toBool());
	
	settings.beginGroup("clientid");
	
	ui.clientComboBox->setCurrentIndex(settings.value("index", 0).toUInt());
	ui.protocolNumBox->setValue(settings.value("protocol", 1).toUInt());

	ui.capEdit1->setText(settings.value("cap1").toString());
	ui.capEdit2->setText(settings.value("cap2").toString());
	ui.capEdit3->setText(settings.value("cap3").toString());


	
	settings.endGroup();
	
	
	int codepageIndex = ui.codepageBox->findText(settings.value("general/codepage", "Windows-1251").toString());
	if (codepageIndex > -1 )
		ui.codepageBox->setCurrentIndex(codepageIndex);
	else 
		ui.codepageBox->setCurrentIndex(47);
	
	int index = settings.value("main/staticon", 0).toInt();
	
	switch(index)
	{
	case 0:
		ui.mainIconRadio->setChecked(true);
		break;
	case 1:
		ui.xIconRadio->setChecked(true);
		break;
	case 2:
		ui.lastIconRadio->setChecked(true);
		break;
	default:
		ui.mainIconRadio->setChecked(true);
	}
}

void icqSettings::saveSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	bool autoC = ui.autoBox->isChecked();
	settings.setValue("connection/auto", autoC);
        if ( autoC ) settings.setValue("connection/statonexit", ui.saveStatusOnExitBox->isChecked());
        else settings.remove("connection/statonexit");
	settings.setValue("connection/reconnect", ui.reconnectBox->isChecked());
	
	if ( ui.mainIconRadio->isChecked())
		settings.setValue("main/staticon", 0);
	else if ( ui.xIconRadio->isChecked())
		settings.setValue("main/staticon", 1);
	else if ( ui.lastIconRadio->isChecked())
		settings.setValue("main/staticon", 2);
	
	settings.beginGroup("clientid");
	
	settings.setValue("index", ui.clientComboBox->currentIndex());
	settings.setValue("protocol", ui.protocolNumBox->value());
	settings.setValue("cap1", ui.capEdit1->text());
	settings.setValue("cap2", ui.capEdit2->text());
	settings.setValue("cap3", ui.capEdit3->text());
	
	settings.endGroup();
	
	settings.setValue("general/codepage", ui.codepageBox->currentText());
	
	if ( changed )
		emit settingsSaved();
	changed = false;
}

void icqSettings::clientIndexChanged(int index)
{
	if ( index == ui.clientComboBox->count() - 1)
		ui.protocolNumBox->setEnabled(true);
	else
		ui.protocolNumBox->setEnabled(false);
}
