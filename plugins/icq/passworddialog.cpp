/*
    passwordDialog

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

#include <QDesktopWidget>

#include "passworddialog.h"

passwordDialog::passwordDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	resetSettings();
	setFixedSize(size());
	connect( ui.passwordEdit, SIGNAL(textChanged ( const QString &) ),
			this, SLOT(okEnable(const QString &)));
	
	connect( ui.saveBox, SIGNAL(stateChanged(int)),
				this, SLOT(savePass(int)));
	
}

passwordDialog::~passwordDialog()
{

}

void passwordDialog::rellocateDialogToCenter(QWidget *widget)
{
	QDesktopWidget desktop;
	// Get current screen num
	int curScreen = desktop.screenNumber(widget);
	// Get available geometry of the screen
	QRect screenGeom = desktop.availableGeometry(curScreen);
	// Let's calculate point to move dialog
	QPoint moveTo(screenGeom.left(), screenGeom.top());
	
	moveTo.setX(moveTo.x() + screenGeom.width() / 2);
	moveTo.setY(moveTo.y() + screenGeom.height() / 2);
	
	moveTo.setX(moveTo.x() - this->size().width() / 2);
	moveTo.setY(moveTo.y() - this->size().height() / 2);
	
	this->move(moveTo);
}


void passwordDialog::okEnable(const QString &text)
{
	ui.saveButton->setEnabled(text != "");
	password = text;
}

void passwordDialog::resetSettings()
{
	ui.passwordEdit->clear();
	ui.saveBox->setChecked(false);
	savePassword = false;
}

void passwordDialog::setTitle(const QString &account)
{
	setWindowTitle(tr("Enter %1 password").arg(account));
}

