/*
    passwordChangeDialog

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


#include "passwordchangedialog.h"

passwordChangeDialog::passwordChangeDialog(const QString &u, const QString &profile_name, QWidget *parent)
    : QDialog(parent)
    , ownerUin(u)
    , m_profile_name(profile_name)
{
	ui.setupUi(this);
}

passwordChangeDialog::~passwordChangeDialog() {
    //
}

void passwordChangeDialog::on_changeButton_clicked()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+ownerUin, "accountsettings");
//	QString pass = settings.value("main/password").toString();
	 const char crypter[] = {0x10,0x67, 0x56, 0x78, 0x85, 0x14, 0x87, 0x11, 0x45,0x45,0x45,0x45,0x45,0x45 };
	QByteArray tmpPass = settings.value("main/password").toByteArray();
	QByteArray roastedPass;
		for ( int i = 0; i < tmpPass.length(); i++ )
			roastedPass[i] = tmpPass.at(i) ^ crypter[i];

	QString curPass = ui.currentEdit->text();
	curPass.truncate(8);

	roastedPass.truncate(8);

	if ( curPass != roastedPass )
	{
		QMessageBox::warning(this, tr("Password error"),
						                   tr("Current password is invalid"));
		return;
	}

	if ( ui.newEdit->text() != ui.lineEdit->text() )
	{
		QMessageBox::warning(this, tr("Password error"),
						                   tr("Confirm password does not match"));
		return;
	}
	
	newPass = ui.newEdit->text();
	newPass.truncate(8);
	QByteArray saveRoastedPass;
	for ( int i = 0; i < newPass.length(); i++ )
		saveRoastedPass[i] = newPass.at(i).unicode() ^ crypter[i];
	settings.setValue("main/password",saveRoastedPass);
	
//	settings.setValue("main/password", newPass);
	accept();
}
