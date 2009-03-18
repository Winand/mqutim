/*
    acceptAuthDialog

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


#include "acceptauthdialog.h"

acceptAuthDialog::acceptAuthDialog(const QString &u,QWidget *parent)
    : QDialog(parent), uin(u)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	acceptAuth = false;
}

acceptAuthDialog::~acceptAuthDialog() {
    //
}

void acceptAuthDialog::on_acceptButton_clicked()
{
	acceptAuth = true;
	emit sendAuthReqAnswer(true, uin);
	close();
}

void acceptAuthDialog::on_declineButton_clicked()
{
	acceptAuth = false;
	sendAuthReqAnswer(false, uin);
	close();
}
