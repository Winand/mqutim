/*
    modifyObject

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


#ifndef MODIFYITEM_H_
#define MODIFYITEM_H_

//#include <QtCore>
#include <QString>

struct modifyObject
{
	quint16 itemId;
	quint16 groupId;
	quint16 itemType;
	quint8 operation;
	QString buddyName;
	QString buddyUin;
	bool authReq;
};

#endif /*MODIFYITEM_H_*/
