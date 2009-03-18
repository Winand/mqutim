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
	setAttribute(Qt::WA_QuitOnClose, false);
}

addBuddyDialog::~addBuddyDialog() {
    //
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
