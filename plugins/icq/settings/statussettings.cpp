/*
    statusSettings

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


#include "statussettings.h"

statusSettings::statusSettings(const QString &profile_name, QWidget *parent)
    : QWidget(parent),
    m_profile_name(profile_name)
{
	ui.setupUi(this);
	changed = false;
	currentStatusIndex = 0;
	loadSettings();
	connect (ui.webawareBox , SIGNAL(stateChanged(int)),
									this, SLOT(widgetStateChanged()));
	connect (ui.contactToolTip , SIGNAL(stateChanged(int)),
									this, SLOT(widgetStateChanged()));
	connect (ui.notifyBox , SIGNAL(stateChanged(int)),
										this, SLOT(widgetStateChanged()));
	connect (ui.customStatusBox , SIGNAL(stateChanged(int)),
											this, SLOT(widgetStateChanged()));
	connect (ui.messageWinBox , SIGNAL(stateChanged(int)),
												this, SLOT(widgetStateChanged()));
//	connect (ui.statusBox , SIGNAL(currentIndexChanged ( int )),
//												this, SLOT(widgetStateChanged()));
	connect (ui.statusBox , SIGNAL(currentIndexChanged ( int )),
												this, SLOT(statusEditChanged(int)));
	connect (ui.statusEdit , SIGNAL(textChanged ()),
								this, SLOT(widgetStateChanged()));
}

statusSettings::~statusSettings()
{

}

void statusSettings::loadSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	settings.beginGroup("statuses");
	ui.webawareBox->setChecked(settings.value("webaware", false).toBool());
	ui.contactToolTip->setChecked(settings.value("xstattool", true).toBool());
	ui.customStatusBox->setChecked(settings.value("customstat", true).toBool());
	ui.notifyBox->setChecked(settings.value("notify", true).toBool());
	settings.endGroup();
	
	settings.beginGroup("autoreply");
	
	awayDontShow = settings.value("awaydshow", false).toBool();
	awayMessage = settings.value("awaymsg","").toString();
	
	lunchDontShow = settings.value("lunchdshow", false).toBool();
	lunchMessage = settings.value("lunchmsg","").toString();
	
	evilDontShow = settings.value("evildshow", false).toBool();
	evilMessage = settings.value("evilmsg","").toString();
		
	depressionDontShow = settings.value("depressiondshow", false).toBool();
	depressionMessage = settings.value("depressionmsg","").toString();
	
	atHomeDontShow = settings.value("athomedshow", false).toBool();
	atHomeMessage = settings.value("athomemsg","").toString();
		
	atWorkDontShow = settings.value("atworkdshow", false).toBool();
	atWorkMessage = settings.value("atworkmsg","").toString();
		
	naDontShow = settings.value("nadshow", false).toBool();
	naMessage = settings.value("namsg","").toString();	
	
	occupiedDontShow = settings.value("occupieddshow", false).toBool();
	occupiedMessage = settings.value("occupiedmsg","").toString();
	
	dndDontShow = settings.value("dnddshow", false).toBool();
	dndMessage = settings.value("dndmsg","").toString();
	
		
		
	settings.endGroup();
	
	ui.messageWinBox->setChecked(awayDontShow);
	ui.statusEdit->setPlainText(awayMessage);
}

void statusSettings::saveSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	settings.beginGroup("statuses");
	settings.setValue("webaware", ui.webawareBox->isChecked());
	settings.setValue("xstattool", ui.contactToolTip->isChecked());
	settings.setValue("customstat", ui.customStatusBox->isChecked());
	settings.setValue("notify", ui.notifyBox->isChecked());
	settings.endGroup();
	

	switch( currentStatusIndex )
		{
		case 0:
			awayDontShow = ui.messageWinBox->isChecked();
			awayMessage = ui.statusEdit->toPlainText();
			break;
			
		case 1:
			lunchDontShow = ui.messageWinBox->isChecked();
			lunchMessage = ui.statusEdit->toPlainText();
			break;
			
		case 2:
			evilDontShow = ui.messageWinBox->isChecked();
			evilMessage = ui.statusEdit->toPlainText();
			break;
			
		case 3:
			depressionDontShow = ui.messageWinBox->isChecked();
			depressionMessage = ui.statusEdit->toPlainText();
			break;
			
		case 4:
			atHomeDontShow = ui.messageWinBox->isChecked();
			atHomeMessage = ui.statusEdit->toPlainText();
			break;
			
		case 5:
			atWorkDontShow = ui.messageWinBox->isChecked();
			atWorkMessage = ui.statusEdit->toPlainText();
			break;
			
		case 6:
			naDontShow = ui.messageWinBox->isChecked();
			naMessage = ui.statusEdit->toPlainText();
			break;
			
		case 7:
			occupiedDontShow = ui.messageWinBox->isChecked();
			occupiedMessage = ui.statusEdit->toPlainText();
			break;
			
		case 8:
			dndDontShow = ui.messageWinBox->isChecked();
			dndMessage = ui.statusEdit->toPlainText();
			break;
		
		default:
			awayDontShow = ui.messageWinBox->isChecked();
			awayMessage = ui.statusEdit->toPlainText();
		}
	
	
	
	
	
	settings.beginGroup("autoreply");
	
	settings.setValue("awaydshow", awayDontShow );
	settings.setValue("awaymsg", awayMessage.left(1000));
	
	settings.setValue("lunchdshow", lunchDontShow );
	settings.setValue("lunchmsg", lunchMessage.left(1000));
		
	settings.setValue("evildshow", evilDontShow );
	settings.setValue("evilmsg", evilMessage.left(1000));
	
	settings.setValue("depressiondshow", depressionDontShow );
	settings.setValue("depressionmsg", depressionMessage.left(1000));
	
	settings.setValue("athomedshow", atHomeDontShow );
	settings.setValue("athomemsg", atHomeMessage.left(1000));
	
	settings.setValue("atworkdshow", atWorkDontShow );
	settings.setValue("atworkmsg", atWorkMessage.left(1000));
	
	settings.setValue("nadshow", naDontShow );
	settings.setValue("namsg", naMessage.left(1000));
	
	settings.setValue("occupieddshow", occupiedDontShow );
	settings.setValue("occupiedmsg", occupiedMessage.left(1000));
	
	settings.setValue("dnddshow", dndDontShow );
	settings.setValue("dndmsg", dndMessage.left(1000));
	
	
	
	settings.endGroup();
	
	if ( changed )
			emit settingsSaved();
	changed = false;
}

void statusSettings::statusEditChanged(int index)
{
	
	switch( currentStatusIndex )
	{
	case 0:
		awayDontShow = ui.messageWinBox->isChecked();
		awayMessage = ui.statusEdit->toPlainText();
		break;
		
	case 1:
		lunchDontShow = ui.messageWinBox->isChecked();
		lunchMessage = ui.statusEdit->toPlainText();
		break;
		
	case 2:
		evilDontShow = ui.messageWinBox->isChecked();
		evilMessage = ui.statusEdit->toPlainText();
		break;
		
	case 3:
		depressionDontShow = ui.messageWinBox->isChecked();
		depressionMessage = ui.statusEdit->toPlainText();
		break;
		
	case 4:
		atHomeDontShow = ui.messageWinBox->isChecked();
		atHomeMessage = ui.statusEdit->toPlainText();
		break;
		
	case 5:
		atWorkDontShow = ui.messageWinBox->isChecked();
		atWorkMessage = ui.statusEdit->toPlainText();
		break;
		
	case 6:
		naDontShow = ui.messageWinBox->isChecked();
		naMessage = ui.statusEdit->toPlainText();
		break;
		
	case 7:
		occupiedDontShow = ui.messageWinBox->isChecked();
		occupiedMessage = ui.statusEdit->toPlainText();
		break;
		
	case 8:
		dndDontShow = ui.messageWinBox->isChecked();
		dndMessage = ui.statusEdit->toPlainText();
		break;
	
	default:
		awayDontShow = ui.messageWinBox->isChecked();
		awayMessage = ui.statusEdit->toPlainText();
	}
	
	
	switch( index )
	{
	case 0:
		ui.messageWinBox->setChecked(awayDontShow);
		ui.statusEdit->setPlainText(awayMessage);
		break;
	
	case 1:
		ui.messageWinBox->setChecked(lunchDontShow);
		ui.statusEdit->setPlainText(lunchMessage);
		break;
		
	case 2:
		ui.messageWinBox->setChecked(evilDontShow);
		ui.statusEdit->setPlainText(evilMessage);
		break;
		
	case 3:
		ui.messageWinBox->setChecked(depressionDontShow);
		ui.statusEdit->setPlainText(depressionMessage);
		break;
		
	case 4:
		ui.messageWinBox->setChecked(atHomeDontShow);
		ui.statusEdit->setPlainText(atHomeMessage);
		break;
		
	case 5:
		ui.messageWinBox->setChecked(atWorkDontShow);
		ui.statusEdit->setPlainText(atWorkMessage);
		break;
		
	case 6:
		ui.messageWinBox->setChecked(naDontShow);
		ui.statusEdit->setPlainText(naMessage);
		break;
	
	case 7:
		ui.messageWinBox->setChecked(occupiedDontShow);
		ui.statusEdit->setPlainText(occupiedMessage);
		break;
		
	case 8:
		ui.messageWinBox->setChecked(dndDontShow);
		ui.statusEdit->setPlainText(dndMessage);
		break;
		
	default:
		ui.messageWinBox->setChecked(awayDontShow);
		ui.statusEdit->setPlainText(awayMessage);
	}
	
	currentStatusIndex = index;
}
