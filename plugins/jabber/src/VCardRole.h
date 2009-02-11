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

#ifndef VCARDROLE_H
#define VCARDROLE_H

#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include "jPluginSystem.h"
#include "VCardConst.h"

class VCardRole : public QLabel
{
	Q_OBJECT

	public:
		VCardRole(bool mode, const QString &type, QWidget *parent = 0);
		~VCardRole();
		void setStatus(const QString&);
		QString status();

	private slots:
		void changeStatus();

	protected:
		void mousePressEvent(QMouseEvent *event);

	private:
		QMenu *menuStatus;
		QAction *actionEmailPersonal;
		QAction *actionEmailWork;
		QAction *actionEmailEmpty;
		QAction *actionPhoneHome;
		QAction *actionPhoneWork;
		QAction *actionPhoneCell;
		QAction *actionPhoneEmpty;
		QAction *actionChecked;
		bool m_mode;
		QString m_type, m_status;

};

#endif // VCARDROLE_H
