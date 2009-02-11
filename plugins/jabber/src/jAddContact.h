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

#ifndef JADDCONTACT_H
#define JADDCONTACT_H

#include "ui_jAddContact.h"

class jAddContact : public QWidget
{
	Q_OBJECT

	public:
                jAddContact(const QStringList &groups, const QString& jid="", const QString& name="", QWidget *parent=0);
		~jAddContact();
		void setIcons(QIcon search, QIcon info, QIcon apply);

	signals:
		void addContact(const QString&, const QString&, const QString&, bool);
		void showInformation(const QString&);
		void searchUser();

	private slots:
		void on_buttonAdd_clicked();
		void on_buttonCancel_clicked();
		void on_buttonInfo_clicked();
		void on_buttonFind_clicked();
                void on_groupText_changed(QString text);

	private:
		Ui::AddContact ui;

};

#endif //jAddContact_H
