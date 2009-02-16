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

#ifndef CONTACTLISTEVENTEATER_H_
#define CONTACTLISTEVENTEATER_H_
#include <QObject>
#include <QEvent>
#include <QModelIndex>

class ContactListEventEater : public QObject
{
	Q_OBJECT
public:
	ContactListEventEater();
protected:
	bool eventFilter(QObject *obj, QEvent *event);
public slots:
	void itemActivated(const QModelIndex & index);
	void collapsed(const QModelIndex & index);
	void expanded(const QModelIndex & index);
};

#endif /*CONTACTLISTEVENTEATER_H_*/
