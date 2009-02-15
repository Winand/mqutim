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

#ifndef JSEARCHCONFERENCE_H
#define JSEARCHCONFERENCE_H

#include "ui_jSearchConference.h"
#include "jServiceDiscovery.h"
#include "jProtocol.h"
#include "jAccount.h"
#include "jAdhoc.h"
#include <QTreeWidgetItem>

class jServiceDiscovery;

class jSearchConference : public QWidget
{
	Q_OBJECT

	public:
		jSearchConference(const QString &target, jAccount *jabber_account, QWidget *parent=0);
		~jSearchConference();

	signals:
		void joinConferenceRoom(const QString &conference, const QString &nickname, const QString &password);
		void selectConference(const QString &conference);
		void finishSearch();

	protected:
		void closeEvent(QCloseEvent*);
		bool eventFilter(QObject *obj, QEvent *event);

	private slots:
		void on_searchButton_clicked();
		void on_closeButton_clicked();
		void on_selectButton_clicked();
		void setConferencesList(QTreeWidgetItem *parent_item, const QList<QStringList> &confList);
		void setConferencesList(QTreeWidgetItem *parent_item, const QStringList &entityList);
		void joinConference(QTreeWidgetItem*, int);
		void joinConference(QTreeWidgetItem*);
		void getDiscoItem(const QString &server, const QString &node, DiscoHandler *dh);
		void getDiscoInfo(const QString &server, const QString &node, DiscoHandler *dh);

	private:
		Ui::SearchConference ui;
		jAccount *m_jabber_account;
		//QIcon icon_server, icon_conference_server, icon_conference;
		QString m_target;
		//jServiceDiscovery *handler;
		//QTreeWidgetItem *m_root_item;

};

#endif //JSEARCHCONFERENCE_H
