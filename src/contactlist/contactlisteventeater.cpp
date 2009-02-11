/*****************************************************************************
    Contact List Eater

    Copyright (c) 2008 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "contactlisteventeater.h"
#include "src/abstractcontactlist.h"
#include <QContextMenuEvent>
#include <QTreeView>
#include <QDebug>
#include <QHelpEvent>
#include <QMouseEvent>
#include <QHoverEvent>

ContactListEventEater::ContactListEventEater() : QObject(0)
{
}
bool ContactListEventEater::eventFilter(QObject *obj, QEvent *event)
{
//	qWarning() << event->type();
	if(event->type() == QEvent::ContextMenu)
        {
                QContextMenuEvent *menu_event = static_cast<QContextMenuEvent *>(event);
                QTreeView *tree_view = dynamic_cast<QTreeView *>(obj->parent());
                if(!tree_view)
                    return QObject::eventFilter(obj, event);
                QModelIndex index = tree_view->indexAt(menu_event->pos());
		if(index.isValid())
			AbstractContactList::instance().sendEventClicked(index,menu_event->globalPos());
	}
        if(event->type() == QEvent::MouseButtonDblClick)
        {
		QMouseEvent *mouse_event = static_cast<QMouseEvent *>(event);
                QTreeView *tree_view = dynamic_cast<QTreeView *>(obj->parent());
                if(!tree_view)
                    return QObject::eventFilter(obj, event);
                QModelIndex index = tree_view->indexAt(mouse_event->pos());
                if(index.isValid())
                        AbstractContactList::instance().sendEventActivated(index);
                event->accept();
        }
	if(event->type() == QEvent::KeyPress)
	{
		QKeyEvent *key_event = static_cast<QKeyEvent *>(event);
		if(!key_event->isAutoRepeat())
		{
			if(key_event->key() == Qt::Key_F2)
			{
				AbstractContactList::instance().selectedItemRenamed();
			}
			if(key_event->key() == Qt::Key_Delete)
			{
				AbstractContactList::instance().selectedItemDeleted();				
			}
			if(key_event->key() == Qt::Key_Enter || key_event->key() == Qt::Key_Return)
			{
                                QTreeView *tree_view = dynamic_cast<QTreeView *>(obj);
                                if(!tree_view)
                                    return QObject::eventFilter(obj, event);
                                QModelIndexList list = tree_view->selectionModel()->selectedIndexes();
				foreach(QModelIndex index, list)
				{
					AbstractContactList::instance().sendEventActivated(index);
				}
			}
		}
	}
	return QObject::eventFilter(obj, event);
}
void ContactListEventEater::signalToChangeBackground()
{
	QTimer::singleShot(100,this,SLOT(doScreenShot()));
}
void ContactListEventEater::doScreenShot()
{
	AbstractContactList::instance().doScreenShot();
}
void ContactListEventEater::itemActivated(const QModelIndex & index)
{
	//AbstractContactList::instance().sendEventActivated(index);
}
void ContactListEventEater::collapsed(const QModelIndex & index)
{
	AbstractContactList::instance().setExpanded(index,false);
}
void ContactListEventEater::expanded(const QModelIndex & index)
{
	AbstractContactList::instance().setExpanded(index,true);	
}
