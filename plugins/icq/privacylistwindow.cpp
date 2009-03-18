/*
    privacyListWindow

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


#include "privacylistwindow.h"
#include "icqpluginsystem.h"

privacyListWindow::privacyListWindow(const QString &uin, const QString &profile_name ,QWidget *parent)
    : QWidget(parent)
    , accountUin(uin)
    , m_profile_name(profile_name)
{
	ui.setupUi(this);
	setWindowTitle(tr("Privacy lists"));
	setWindowIcon(IcqPluginSystem::instance().getIcon("privacylist"));
	ui.visibleTreeWidget->setColumnWidth(2,22);
	ui.visibleTreeWidget->setColumnWidth(3,22);
	ui.visibleTreeWidget->setColumnWidth(1,200);

	ui.invisibleTreeWidget->setColumnWidth(2,22);
	ui.invisibleTreeWidget->setColumnWidth(3,22);
	ui.invisibleTreeWidget->setColumnWidth(1,200);

	ui.ignoreTreeWidget->setColumnWidth(2,22);
	ui.ignoreTreeWidget->setColumnWidth(3,22);
	ui.ignoreTreeWidget->setColumnWidth(1,200);
	createLists();
}

privacyListWindow::~privacyListWindow() {
    //
}

void privacyListWindow::createLists()
{
	IcqPluginSystem &ips = IcqPluginSystem::instance();
        QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+accountUin, "contactlist");
	ui.visibleTreeWidget->clear();

	QStringList visibleList = contacts.value("list/visible").toStringList();

	foreach(const QString &uin, visibleList)
	{
		QTreeWidgetItem *buddy = new QTreeWidgetItem(ui.visibleTreeWidget);
		buddy->setText(0,uin);
		buddy->setText(1, contacts.value(uin + "/nickname", "").toString());
		buddy->setIcon(2, ips.getIcon("contactinfo"));
		buddy->setIcon(3, ips.getIcon("delete_user"));
	}

	ui.invisibleTreeWidget->clear();

	QStringList invisibleList = contacts.value("list/invisible").toStringList();

	foreach(const QString &uin, invisibleList)
	{
		QTreeWidgetItem *buddy = new QTreeWidgetItem(ui.invisibleTreeWidget);
		buddy->setText(0,uin);
		buddy->setText(1, contacts.value(uin + "/nickname", "").toString());
		buddy->setIcon(2, ips.getIcon("contactinfo"));
		buddy->setIcon(3, ips.getIcon("delete_user"));
	}

	ui.ignoreTreeWidget->clear();

	QStringList ignoreList = contacts.value("list/ignore").toStringList();

	foreach(const QString &uin, ignoreList)
	{
		QTreeWidgetItem *buddy = new QTreeWidgetItem(ui.ignoreTreeWidget);
		buddy->setText(0,uin);
		buddy->setText(1, contacts.value(uin + "/nickname", "").toString());
		buddy->setIcon(2, ips.getIcon("contactinfo"));
		buddy->setIcon(3, ips.getIcon("delete_user"));
	}

}

void privacyListWindow::setOnline(bool online)
{

	ui.ignoreTreeWidget->setColumnHidden(2, !online);
	ui.visibleTreeWidget->setColumnHidden(2, !online);
	ui.invisibleTreeWidget->setColumnHidden(2, !online);

	ui.ignoreTreeWidget->setColumnHidden(3, !online);
	ui.visibleTreeWidget->setColumnHidden(3, !online);
	ui.invisibleTreeWidget->setColumnHidden(3, !online);
}

void privacyListWindow::on_visibleTreeWidget_itemClicked(QTreeWidgetItem * item, int column)
{
	if ( column == 2)
		emit openInfo(item->text(0), item->text(1), "", "");
	if ( column == 3)
	{
		emit deleteFromPrivacyList(item->text(0), 0);
		delete item;
	}
}

void privacyListWindow::on_invisibleTreeWidget_itemClicked(QTreeWidgetItem * item, int column)
{
	if ( column == 2)
	{
			emit openInfo(item->text(0), item->text(1), "", "");

	}
	if ( column == 3)
	{
		emit deleteFromPrivacyList(item->text(0), 1);
		delete item;
	}
}

void privacyListWindow::on_ignoreTreeWidget_itemClicked(QTreeWidgetItem * item, int column)
{
	if ( column == 2)
			emit openInfo(item->text(0), item->text(1), "", "");
	if ( column == 3)
	{
		emit deleteFromPrivacyList(item->text(0), 2);
		delete item;
	}
}
