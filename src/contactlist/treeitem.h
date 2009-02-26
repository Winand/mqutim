/*****************************************************************************
    Tree Item

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

#ifndef TREEITEM_H_
#define TREEITEM_H_

#include <QList>
#include <QVariant>
#include <QVector>
#include <QHash>
#include <QIcon>
#include <QTimer>
#include <qutim/plugininterface.h>

using namespace qutim_sdk_0_2;

class TreeItem
{
public:  
	TreeItem(const QVariant &display, TreeItem *parent = 0);
	virtual ~TreeItem();
	TreeItem *child(int number);
	int childCount() const;
	QVariant data(int role) const;
	bool insertChildren(int position, int count);
	TreeItem *parent();
	bool removeChildren(int position, int count);
	bool removeChildren();
	int childNumber() const;
	bool setData(const QVariant &value, int role);
	void setStructure(const TreeModelItem &structure);
	const TreeModelItem &getStructure();
	QString getName();
	int indexOf(TreeItem *t);
	TreeItem *find(const QString &id);
	void setHash(const QString &id, TreeItem *item);
	void removeHash(const QString &id);
        bool hasHash(const QString &id) { return m_item_list.contains(id); }
	void setImage(const QIcon &icon, int column);
	QIcon getImage(int column);
	void setRow(const QVariant &item, int row);
	void setStatus(const QString &text, const QIcon &icon, int mass);
	bool isExpanded();
	void setExpanded(bool expanded);
	void setOnline(bool show);
	bool getOnline();
	int getChildPosition(const QString &child);
	void moveChild(const QString &child, int position);
	void setChildOrder(const QStringList &order);
	const QStringList &getChildOrder();
	void setAlwaysVisible(bool visible);
	bool getAlwaysVisible();
	void setAlwaysInvisible(bool invisible);
	bool getAlwaysInvisible();
	void setContent(int type, bool has){ m_content = has?m_content|type:m_content^m_content&type; } // it is a hell, but should work
	bool hasContent() { return m_content>0; }
	int getContent() { return m_content; }
	int m_visible;
	int m_invisible;
	bool m_is_visible;
private:
	bool m_is_online;
	QStringList m_child_order;
	QList<TreeItem*> m_child_items;
	QVariant m_item_display;
        QList<QVariant> m_item_icons;
        QList<QVariant> m_item_bottom_rows;
	TreeItem *m_parent_item;
	QHash<QString, TreeItem *> m_item_list;
	QVariant m_item_type;
	int m_item_mass;
	QString m_item_status;
	QVariant m_online_number;
	TreeModelItem m_structure;
	bool m_is_expanded;
	bool m_is_always_visible;
	bool m_is_always_invisible;
	QVariant m_current_status_icon;
	int m_content;
};

#endif /*TreeItem_H_*/
