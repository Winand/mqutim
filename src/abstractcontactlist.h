/*****************************************************************************
    Abstract Contact List

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

#ifndef ABSTRACTCONTACTLIST_H_
#define ABSTRACTCONTACTLIST_H_
#include <QTreeView>
#include <QAbstractItemModel>
#include <QDebug>
#include <QTemporaryFile>
#include <QToolTip>
#include "src/contactlist/contactlistproxymodel.h"
#include "src/contactlist/treecontactlistmodel.h"
#include "src/contactlist/contactlistitemdelegate.h"
#include "src/contactlist/contactlisteventeater.h"

using namespace qutim_sdk_0_2;

class AbstractContactList
{
public:
	static AbstractContactList &instance();
	AbstractContactList();
	virtual ~AbstractContactList();
	void setTreeView(QTreeView *TreeView);
	bool addItem(const TreeModelItem & Item, QString name=QString());
	bool removeItem(const TreeModelItem & Item);
	bool moveItem(const TreeModelItem & OldItem, const TreeModelItem & NewItem);
	bool setItemName(const TreeModelItem & Item, QString name);
	bool setItemIcon(const TreeModelItem & Item, QIcon icon, int position);
	bool setItemRow(const TreeModelItem & Item, QList<QVariant> row, int position);
	bool setItemStatus(const TreeModelItem & Item, QIcon icon, QString text, int mass);
	void setItemHasUnviewedContent(const TreeModelItem & Item, bool has_content);
	bool getItemHasUnviewedContent(const TreeModelItem & item);
	void setItemIsTyping(const TreeModelItem & Item, bool has_content);
	bool getItemIsTyping(const TreeModelItem & item);
	bool itemExists(const TreeModelItem & item);
	void itemActivated(const TreeModelItem & Item);
	void contactMenu(const TreeModelItem & Item, QPoint point);
	void itemMoved(const TreeModelItem & OldItem, const TreeModelItem & NewItem);
	void setAccountIsOnline(const TreeModelItem &Item, bool online);
	QString getItemStatus(const TreeModelItem &Item);
	void sendEventActivated(const QModelIndex & index);
	void sendEventActivated(const QPoint &pos);
	void sendEventClicked(const QModelIndex & index, const QPoint & point);
	void sendEventHelp(const QModelIndex & index, const QPoint & point);
	QIcon getItemStatusIcon(const TreeModelItem & item);
	QPixmap getItemClientIcon(const TreeModelItem & item);
	void setShowOffline(bool show);
	void setShowEmptyGroup(bool show);
	void setSortStatus(bool sort);
	void setShowSeparator(bool show);
	void setModelType(int type);
	bool getShowOffline();
	bool getShowEmptyGroup();
	bool getSortStatus();
	bool getShowSeparator();
	int getModelType();
	void selectedItemDeleted();
	void selectedItemRenamed();
	void loadProfile(const QString &profile_name);
	void loadSettings();
	void loadGuiSettings();
	void setExpanded(const QModelIndex & index, bool expanded);
	void saveSettings();
	void setItemVisible(const TreeModelItem &item, bool visible);
	void setItemInvisible(const TreeModelItem &item, bool invisible);
private:
	bool m_first_load;
	QTemporaryFile *m_background_file;
	QString m_profile_name;
	QTreeView *m_tree_view;
	TreeContactListModel *m_item_model;
	ContactListEventEater *m_event_eater;
	ContactListProxyModel *m_proxy_model;
	ContactListItemDelegate *m_item_delegate;
        bool m_has_tree_view;
};

#endif /*ABSTRACTCONTACTLIST_H_*/
