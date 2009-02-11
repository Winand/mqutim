/*
    addBuddyDialog

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


#include "addbuddydialog.h"
#include "icqpluginsystem.h"

addBuddyDialog::addBuddyDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	setWindowIcon(IcqPluginSystem::instance().getIcon("add_user.png"));
	setFixedSize(size());
	setAttribute(Qt::WA_QuitOnClose, false);
	
	move(desktopCenter());
}

addBuddyDialog::~addBuddyDialog()
{

}

void addBuddyDialog::rellocateDialogToCenter(QWidget *widget)
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

QPoint addBuddyDialog::desktopCenter()
{
	QDesktopWidget desktop;
	return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void addBuddyDialog::setContactData(const QString &name, const QStringList &groups)
{
	ui.nickEdit->setText(name);
	ui.groupList->addItems(groups);
}

void addBuddyDialog::setMoveData(const QStringList &groups)
{
	ui.groupList->addItems(groups);
	ui.nickEdit->setEnabled(false);
	ui.addButton->setText(tr("Move"));
}
