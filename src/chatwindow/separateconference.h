/*
    SeparateConference

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

#ifndef SEPARATECONFERENCE_H
#define SEPARATECONFERENCE_H

#include <QtGui/QWidget>
#include "ui_conferenceform.h"
/*#include "chatemoticonmenu.h"*/
#include "separatechatwindow.h"
#include "conferencecontactlist.h"
/*#include "conferencetabcompletion.h"*/
#include <QPushButton>
#include <QToolButton>
#include <QTextEdit>
#include <QTextBrowser>
#include <QSoftMenuBar>

class AbstractChatLayer;

class SeparateConference : public QWidget
{
    Q_OBJECT

public:
    SeparateConference(
    		const QString &protocol_name,
    		const QString &conference_name,
    		const QString &account_name,
    		const QString &emoticon_path,
    		QWidget *parent = 0);
    ~SeparateConference();
    void addMessage(const QString &from,
    		const QString &message, const QString &date, bool history);
    void addServiceMessage(const QString &message, const QString &date);
    void setOptions(bool remove_after, quint16 remove_count,
    		bool close_after, bool show_names, bool on_enter);
    void setOwnerNickName(const QString &nick)
    {
    	m_mine_nick = nick;
    }
	void addConferenceItem(const QString &nickname);
	void removeConferenceItem(const QString &nickname);
	void renameConferenceItem(const QString &nickname, const QString &new_nickname);
	void setConferenceItemStatus(const QString &nickname, const QIcon &icon, const QString &status, int mass);
	void setConferenceItemIcon(const QString &nickname,	const QIcon &icon, int position);
	void setConferenceItemRole(const QString &nickname, const QIcon &icon, const QString &role, int mass);
//	void restoreSplitters(const QByteArray &state, const QByteArray &state2);
	QStringList getItemsList() { return m_contact_list?m_contact_list->getUsers():QStringList(); };
    void Vibrate();

protected:
	void showEvent(QShowEvent * event);
	void keyPressEvent(QKeyEvent *ev);

private slots:
	void on_clearButton_clicked();
	void newsOnLinkClicked(const QUrl &url);
	void windowFocused();
	void stopVibrate();
private:
	void moveCursorToEnd();
	void focusTextEdit();
	void checkForActive(const QString &message);
	bool eventFilter(QObject *obj, QEvent *ev);
	QString checkForEmoticons(const QString &message);
/*	QString invertMessage(QString &text);*/
	AbstractChatLayer &m_abstract_chat_layer;
	QString m_conference_name;
	QString m_account_name;
	QString m_protocol_name;
	Ui::ConferenceForm ui;
	QMenu *m_emotic_menu;
	QWidgetAction *m_emoticon_action;
/*	ChatEmoticonMenu *m_emoticon_widget;*/
	QString m_emoticons_path;
	QHash<QString, QString> m_emoticon_list;
	QHash<QString, QString> m_urls;
	bool m_remove_message_after;
	quint16 m_remove_count;
	QList<quint64> m_message_positions;
/*	bool m_close_after_send;
        bool m_show_names;*/
	QTextBrowser *m_text_browser;
        QLineEdit *conferenceTextEdit;
	QString m_now_html;
	QString m_mine_nick;
	QString m_last_sender;
	int m_last_message_position;
	bool m_last_history;
	bool m_already_hred;
	ConferenceContactList *m_contact_list;
	ContactListItemDelegate *m_item_delegate;
	ConferenceContactListEventEater *m_cl_event_eater;
	bool m_scroll_at_max;
	quint64 m_current_scroll_position;
/*	TabCompletion *m_tab_compl;*/
};

#endif // SEPARATECONFERENCE_H
