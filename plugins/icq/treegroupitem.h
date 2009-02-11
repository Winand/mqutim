/*
    treeGroupItem

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


#ifndef TREEGROUPITEM_H_
#define TREEGROUPITEM_H_
#include <QString>
#include <QList>
#include <QColor>

class contactListTree;
class tlv;
class contactSeparator;
class icqBuffer;

class treeGroupItem
{
public:
	treeGroupItem();
	~treeGroupItem();

	void commonInit();//!< It is called from constructors for common initialization
	void setGroupText(const QString &);
	void updateText();
	void readData(icqBuffer *, quint16);
	void takeTlv(tlv &);
	void groupClicked();
	void buddyOnline();
	void buddyOffline();
	void setOnOffLists();
	void updateOnline();
	QString name;
	quint32 userCount;
	quint32 userOnline;
	void setExpanded(bool);
	void setCustomFont(const QString &, int, const QColor &);
	void hideSeparators(bool);
	QList<quint16> buddiesList;

	contactListTree *par;
	bool fromItem;
private:
	void addBuddiesToList(QByteArray);
	bool separatorsHided;
	quint16 convertToInt16(const QByteArray &);
};

#endif /*TREEGROUPITEM_H_*/
