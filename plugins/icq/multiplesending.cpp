/*
    multipleSending

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

#include "treegroupitem.h"
#include "treebuddyitem.h"
#include "icqmessage.h"

#include "multiplesending.h"
#include "icqpluginsystem.h"

multipleSending::multipleSending(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("Send multiple"));
	setWindowIcon(IcqPluginSystem::instance().getIcon("multiple"));
	ui.contactListWidget->header()->hide();
/*	QList<int> listSize;
	listSize.append(408);
	listSize.append(156);
        ui.splitter->setSizes(listSize);*/
	sendTimer = new QTimer(this);
	connect(sendTimer, SIGNAL(timeout()), this, SLOT(sendMessage()));
}

multipleSending::~multipleSending() {
    //
}

void multipleSending::setTreeModel(const QString &uin, const QHash<quint16, treeGroupItem *> *groupList, const QHash<QString, treeBuddyItem *> *buddyList)
{
	rootItem = new QTreeWidgetItem(ui.contactListWidget);
	rootItem->setText(0, uin);
	rootItem->setFlags(rootItem->flags() | Qt::ItemIsUserCheckable);
	rootItem->setCheckState(0, Qt::Unchecked);

	foreach(treeGroupItem *getgroup, *groupList)
	{
		QTreeWidgetItem *group = new QTreeWidgetItem(rootItem);
		group->setText(0,getgroup->name);
		group->setFlags(group->flags() | Qt::ItemIsUserCheckable);
		group->setCheckState(0, Qt::Unchecked);


		foreach(treeBuddyItem *getbuddy, *buddyList)
		{
			if ( getbuddy->groupName == getgroup->name)
			{
				QTreeWidgetItem *buddy = new QTreeWidgetItem(group);
				buddy->setText(0,getbuddy->getName());
//				buddy->setIcon(0, getbuddy-> );
				buddy->setFlags(buddy->flags() | Qt::ItemIsUserCheckable);
				buddy->setCheckState(0, Qt::Unchecked);
                                buddy->setWhatsThis(0,getbuddy->getUin());
			}
		}

		if ( group->childCount() )
			group->setExpanded(true);
	}

	if ( rootItem->childCount() )
		rootItem->setExpanded(true);
}

void multipleSending::on_contactListWidget_itemChanged(QTreeWidgetItem *item, int )
{
	if ( item->childCount() )
	{
		Qt::CheckState checkState = item->checkState(0);
		for ( int i = 0; i < item->childCount(); i++)
		{
			item->child(i)->setCheckState(0,checkState);
		}
	}

}

void multipleSending::on_sendButton_clicked()
{
	if ( !ui.messageEdit->toPlainText().isEmpty())
	{
	ui.sendButton->setEnabled(false);
	ui.stopButton->setEnabled(true);

	for( int i = 0; i < rootItem->childCount(); i++)
	{
		QTreeWidgetItem *group = rootItem->child(i);
		for ( int j = 0; j < group->childCount(); j++)
		{
                        if ( !group->child(j)->whatsThis(0).isEmpty() && group->child(j)->checkState(0))
                                                sendToList<<group->child(j)->whatsThis(0);
		}


	}

	barInterval = sendToList.count();
	if (barInterval)
	{

		sendMessage();
	}
	else
		on_stopButton_clicked();
	}
}

void multipleSending::on_stopButton_clicked()
{
	ui.sendButton->setEnabled(true);
	ui.stopButton->setEnabled(false);
	sendToList.clear();
	sendTimer->stop();
}

void multipleSending::sendMessage()
{
	if ( !ui.messageEdit->toPlainText().isEmpty())
	{
		if ( sendToList.count() )
		{

			messageFormat msg;
			msg.date = QDateTime::currentDateTime();
			msg.fromUin = sendToList.at(0);
			msg.message = ui.messageEdit->toPlainText();
			emit sendMessageToContact(msg);
			sendToList.removeAt(0);
			sendTimer->start(2000);
			if ( !sendToList.count() )
				on_stopButton_clicked();
		} else
			on_stopButton_clicked();
	} else
		on_stopButton_clicked();
}
