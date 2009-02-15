/*
Copyright (c) 2008 by Denis Daschenko <daschenko@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#include "jAddContact.h"

jAddContact::jAddContact(const QStringList &groups, const QString& jid, const QString& name, QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	ui.groupName->addItems(groups);
        ui.jidName->setText(jid);
        ui.nickName->setText(name);
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_DeleteOnClose, true);
}

jAddContact::~jAddContact()
{
}

void jAddContact::setIcons(QIcon search, QIcon info, QIcon apply)
{
	ui.buttonInfo->setIcon(info);
	ui.buttonFind->setIcon(search);
	ui.buttonAdd->setIcon(apply);
}

void jAddContact::on_buttonAdd_clicked()
{
	QString group = ui.groupName->currentText();
	if (group == tr("<no group>"))
		group = "";
	emit addContact(ui.jidName->text(), ui.nickName->text(), group, ui.authorizationCheck->isChecked());
	close();
}

void jAddContact::on_buttonCancel_clicked()
{
	close();
}

void jAddContact::on_buttonInfo_clicked()
{
	emit showInformation(ui.jidName->text());
}

void jAddContact::on_buttonFind_clicked()
{
	emit searchUser();
}

void jAddContact::on_groupText_changed(QString text)
{
        ui.buttonAdd->setEnabled(text != tr("Services"));
}
