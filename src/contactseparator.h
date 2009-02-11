/*
    contactSeparator

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


#ifndef CONTACTSEPARATOR_H_
#define CONTACTSEPARATOR_H_

#include <QtGui>

class contactSeparator : public QTreeWidgetItem
{
public:
	contactSeparator(QTreeWidgetItem *parent);
	contactSeparator(QTreeWidget *parent);
	~contactSeparator();
	void hideSeparator(bool);
	void setText(int, const QString &);
	
private:
	QString currentText;
};

#endif /*CONTACTSEPARATOR_H_*/
