/*****************************************************************************
    Contact List Item Delegate

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

#ifndef CONTACTLISTITEMDELEGATE_H_
#define CONTACTLISTITEMDELEGATE_H_
#include <QItemDelegate>
#include <QtopiaItemDelegate>
#include <QPainter>
#include <QDebug>
#include <QDomElement>
#include <QDomDocument>
#include <QTreeView>
#include <QFont>
#include <QVector>
#include <QHash>
#include <QVariant>

class ContactListItemDelegate : public QtopiaItemDelegate
{
	Q_OBJECT
public:
	explicit ContactListItemDelegate(QObject *parent = 0);
	virtual ~ContactListItemDelegate();
  
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
  
	void setTreeView(QAbstractItemView *tree);
	void setSettings(QVector<bool> show_icons);
private:
  QSize size(const QStyleOptionViewItem &option, const QModelIndex & index, const QVariant &item) const;
  void draw(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QVariant &value) const;
	QAbstractItemView *m_tree_view;
	
	QFont m_status_font;
	QHash<QString,QColor> m_status_color;
  
  QVector<bool> m_show_icons;
	
	QHash<QString,QColor> m_style_colors;
	QHash<QString,QFont> m_style_fonts;
	QHash<QString,bool> m_style_bools;
	QHash<QString,int> m_style_ints;
	QHash<QString,double> m_style_reals;
	QHash<QString,QString> m_style_strings;
	//ItemDelegateStyle m_style;
};

#endif /*CONTACTLISTITEMDELEGATE_H_*/
