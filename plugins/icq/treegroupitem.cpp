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

#include "tlv.h"
#include "buffer.h"

#include "treegroupitem.h"

treeGroupItem::treeGroupItem()
{
	commonInit();
}

treeGroupItem::~treeGroupItem()
{

}

void treeGroupItem::commonInit()
{
	userCount = 0;
	userOnline = 0;
}

void treeGroupItem::setGroupText(const QString &text)
{
	name = text;
}

void treeGroupItem::updateText()
{
	QString t(name + " (" + QString::number(userOnline) + "/" + QString::number(userCount) + ")");
//	setText(1,t);
}

void treeGroupItem::readData(icqBuffer *socket, quint16 length)
{
	for ( ;length > 0; )
	{
		tlv tmpTlv;
		tmpTlv.readData(socket);
		takeTlv(tmpTlv);
		length -= tmpTlv.getLength();
	}
}

void treeGroupItem::takeTlv(tlv &newTlv)
{
	switch(newTlv.getTlvType())
	{
	case 0x00c8:
		userCount = newTlv.getTlvLength() / 2;
		addBuddiesToList(newTlv.getTlvData());
		updateText();
		break;
	default:
		break;
	}
}

void treeGroupItem::groupClicked()
{


//	setExpanded(!isExpanded());


}

void treeGroupItem::buddyOnline()
{
	userOnline++;
	updateText();

}

void treeGroupItem::buddyOffline()
{
	userOnline--;
	updateText();
//	treeWidget()->setUpdatesEnabled(false);
//	offlineList->sortChildren(1,Qt::AscendingOrder);
//	treeWidget()->setUpdatesEnabled(true);
}

void treeGroupItem::setOnOffLists()
{

}

void treeGroupItem::updateOnline()
{
//	treeWidget()->setUpdatesEnabled(false);
//	onlineList->sortChildren(1,Qt::AscendingOrder);
//
//	treeWidget()->setUpdatesEnabled(true);
}

void treeGroupItem::setExpanded(bool aexpand)
{
//	if ( userCount)
//	if ( aexpand )
//	{
//
//		if ( !(onlineList->isHidden() && offlineList->isHidden()) )
//		{
//			setIcon(0, m_iconManager->getSystemIcon("expanded"));
//			QTreeWidgetItem::setExpanded(true);
//			offlineList->sortChildren(1,Qt::AscendingOrder);
//			onlineList->sortChildren(1,Qt::AscendingOrder);
//		}
//	} else {
//		if ( !(onlineList->isHidden() && offlineList->isHidden())  )
//		{
//			setIcon(0, m_iconManager->getSystemIcon("collapsed"));
//			QTreeWidgetItem::setExpanded(false);
//		}
//	}

}

void treeGroupItem::setCustomFont(const QString &f, int s, const QColor &c)
{
//	QFont font;
//	font.setFamily(f);
//	font.setPointSize(s);
//	setFont(1, font);
//	setForeground(1, c);
}

void treeGroupItem::hideSeparators(bool hide)
{
//	onlineList->hideSeparator(hide);
//	offlineList->hideSeparator(hide);
	separatorsHided = hide;
}

quint16 treeGroupItem::convertToInt16(const QByteArray &packet)
{
	bool ok;
	return packet.toHex().toUInt(&ok,16);
}

void treeGroupItem::addBuddiesToList(QByteArray array)
{
	int size = array.size() / 2;

	for ( int i = 0; i < size ; i++)
	{
		buddiesList.append(convertToInt16(array.left(2)));
		array = array.right(array.size() - 2);
	}
}

