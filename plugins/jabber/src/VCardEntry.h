/*
Copyright (c) 2008 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef VCARDENTRY_H
#define VCARDENTRY_H

#include <QWidget>
#include <QToolButton>

class VCardEntry : public QWidget
{
	Q_OBJECT

	public:
		VCardEntry(QWidget *parent);
		virtual QString type() = 0;
		virtual void addDeleteButton(QToolButton *button) = 0;

};

#endif // VCARDENTRY_H
