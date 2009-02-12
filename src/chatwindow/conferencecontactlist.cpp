/*
    Conference Contact List

    Copyright (c) 2008 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "conferencecontactlist.h"
#include "src/abstractchatlayer.h"
#include "src/abstractcontextlayer.h"
#include <QEvent>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QKeyEvent>

ConferenceContactListEventEater::ConferenceContactListEventEater() : QObject(0)	
{
	m_contact_list=0;
}

bool ConferenceContactListEventEater::eventFilter(QObject *obj, QEvent *event)
{
	if(!m_contact_list)
		return QObject::eventFilter(obj, event);
	if(event->type() == QEvent::ContextMenu)
	{
                QContextMenuEvent *menu_event = static_cast<QContextMenuEvent *>(event);
                QListView *list_view = qobject_cast<QListView *>(obj->parent());
                if(!list_view)
                    return QObject::eventFilter(obj, event);
                QModelIndex index = list_view->indexAt(menu_event->pos());
		if(index.isValid())
			m_contact_list->sendEventClicked(index,menu_event->globalPos());
	}
	if(event->type() == QEvent::MouseButtonDblClick)
	{
		QMouseEvent *mouse_event = static_cast<QMouseEvent *>(event);
                QListView *list_view = qobject_cast<QListView *>(obj->parent());
                if(!list_view)
                    return QObject::eventFilter(obj, event);
                QModelIndex index = list_view->indexAt(mouse_event->pos());
                if(index.isValid())
                        m_contact_list->sendEventActivated(index);
	}
	if(event->type() == QEvent::KeyPress)
	{
		QKeyEvent *key_event = static_cast<QKeyEvent *>(event);
		if(!key_event->isAutoRepeat())
		{
			if(key_event->key() == Qt::Key_Enter || key_event->key() == Qt::Key_Return)
			{
                                QListView *list_view = qobject_cast<QListView *>(obj);
                                if(!list_view)
                                        return QObject::eventFilter(obj, event);
                                QModelIndexList list = list_view->selectionModel()->selectedIndexes();
				foreach(QModelIndex index, list)
				{
					m_contact_list->sendEventActivated(index);
				}
			}
		}
	}
	return QObject::eventFilter(obj, event);
}

void ConferenceContactListEventEater::itemActivated(const QModelIndex & index)
{
	//AbstractContactList::instance().sendEventActivated(index);
}

ConferenceContactList::ConferenceContactList(const QString &protocol_name, const QString &conference_name, const QString &account_name, QListView *list_view)
: m_protocol_name(protocol_name), m_conference_name(conference_name),
	m_account_name(account_name), m_plugin_system(PluginSystem::instance())
{
	m_list_view=list_view;
	m_item_model = new ConferenceItemModel(this);
}

ConferenceContactList::~ConferenceContactList()
{
	delete m_item_model;
}

void ConferenceContactList::addConferenceItem(const QString &nickname)
{
	m_item_model->addBuddy(nickname);
}

void ConferenceContactList::removeConferenceItem(const QString &nickname)
{
	m_item_model->removeBuddy(nickname);
}

void ConferenceContactList::renameConferenceItem(const QString &nickname, const QString &new_nickname)
{
	m_item_model->renameBuddy(nickname,new_nickname);
}

void ConferenceContactList::setConferenceItemStatus(const QString &nickname, const QIcon &icon, const QString &status, int mass)
{
	m_item_model->setItemStatus(nickname,icon,status,mass);
}

void ConferenceContactList::setConferenceItemIcon(const QString &nickname,	const QIcon &icon, int position)
{
	m_item_model->setItemIcon(nickname,icon,position);
}

void ConferenceContactList::setConferenceItemRole(const QString &nickname, const QIcon &icon, const QString &role, int mass)
{
	m_item_model->setItemRole(nickname,icon,role,mass);
}

QStringList ConferenceContactList::getUsers()
{
	return m_item_model->getUsers();
}

void ConferenceContactList::sendEventActivated(const QModelIndex & index)
{
	if(!index.isValid())
			return;
	ConferenceItem *item = static_cast<ConferenceItem *>(index.internalPointer());
	QString nickname = item->data(Qt::DisplayRole).toString();
	PluginSystem::instance().conferenceItemActivated(m_protocol_name,m_conference_name,m_account_name,nickname);
/*	TreeModelItem item_struct;
	item_struct.m_protocol_name = m_protocol_name;
	item_struct.m_account_name = m_account_name;
	item_struct.m_item_name = m_conference_name+"/"+nickname;
	item_struct.m_item_type = 0;
	AbstractChatLayer::instance().createChat(item_struct);	*/
}

void ConferenceContactList::sendEventClicked(const QModelIndex & index, const QPoint & point)
{
	if(!index.isValid())
			return;
	ConferenceItem *item = static_cast<ConferenceItem *>(index.internalPointer());
	QString nickname = item->data(Qt::DisplayRole).toString();	
	AbstractContextLayer::instance().conferenceItemContextMenu(m_protocol_name, m_conference_name, m_account_name, nickname, point);
}

QString ConferenceContactList::getToolTip(const QString &nickname)
{
	return m_plugin_system.getConferenceItemToolTip(m_protocol_name, m_conference_name, m_account_name, nickname);
}

void ConferenceContactList::nowActive()
{
	m_list_view->setModel(m_item_model);
}
