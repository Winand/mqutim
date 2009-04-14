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

#ifndef JJOINCHAT_H
#define JJOINCHAT_H

#include "ui_jJoinChat.h"
#include "jProtocol.h"
#include "jAccount.h"
#include "jSearchConference.h"
#include "conference/jConference.h"
#include <QList>
#include <QDebug>
#include <QDesktopWidget>

class jJoinChat : public QWidget
{
	Q_OBJECT

	public:
		jJoinChat(jAccount *jabber_account, const QString &room, const QString &password, bool showBookmark, QWidget *parent=0);
		~jJoinChat();
		void setConferences();

	public slots:
		void showConference(QListWidgetItem *current, QListWidgetItem *previous);
		void on_addConferenceButton_clicked();
		void on_removeConferenceButton_clicked();
//		void on_searchButton_clicked();

	signals:
		void createConferenceRoom(const QString&, const QString&, const QString&);

	private slots:
		void on_joinButton_clicked();
		void on_saveButton_clicked();
		void joinBookmark(QListWidgetItem *item);
		void setConferenceRoom(const QString &conference);
		void finishSearch();
		void changeRecent(int i);

	private:
		void fillConferences();
		void getLocalBookmarks();
		void getRecentBookmarks();
		void saveLocalBookmarks();
		Ui::JoinChat ui;
		QPoint desktopCenter();
		jAccount *m_jabber_account;
		//jConference *m_real_parent;
		QList<BookmarkListItem> m_b_list;
		QList<ConferenceListItem> m_c_list;
		BookmarkStorage* m_bookmark_storage;
		QString m_settings, m_room, m_password;
		QSettings *m_account_settings;
		bool isLocal;
		QStringList recent_conf, recent_nick, recent_pass;

};

#endif //JJOINCHAT_H
