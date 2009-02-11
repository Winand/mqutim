/*
    TabbedConference

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

#ifndef TABBEDCONFERENCE_H
#define TABBEDCONFERENCE_H

#include <QtGui/QWidget>
#include "ui_conferenceform.h"
#include "tabbedchatwindow.h"
#include "chatemoticonmenu.h"
#include "separatechatwindow.h"
#include "conferencecontactlist.h"
#include "conferencetabcompletion.h"

struct UnreadConfMessage
{
	QString m_message;
	bool m_sender;
};

struct ConferenceUserSpace
{
	QString m_protocol_name;
	QString m_account_name;
	QString m_conference_name;
	QString m_chat_history;
	QString m_mine_nick;
	QString m_last_sender;
	bool m_last_message_sender;
	int m_last_message_icon_position;
	int m_last_message_position;
    QList<int> m_message_positions;
    bool m_lock_for_first_message;
    bool m_waiting_for_activation;
    bool m_unreaded_message;
    QString m_send_text;
    QList<UnreadConfMessage> m_unreaded_messages;
    bool m_last_history;
    ConferenceContactList *m_contact_list;
	TabCompletion *m_tab_compl;
	QString m_topic;
};

class TabbedConference: public QWidget
{
    Q_OBJECT
public:
	TabbedConference(const QString &emoticon_path,
                /*bool webkit_mode,*/
    		const QString &style_path,
    		const QString &style_variant,
			QWidget *parent = 0);
	~TabbedConference();
    void setOptions(bool remove_after, quint16 remove_count,
    		bool close_after, bool show_names, bool on_enter);
//	void restoreSplitters(const QByteArray &state, const QByteArray &state2);
	bool conferenceTabOpened(const QString &identification);
	void createTab(const QString &protocol_name,
    	const QString &account_name,
    	const QString &conference_name,
    	const QString &identification);
	void addMessage(const QString &identification, const QString &from,
    	const QString &message, const QString &date, bool history);
	void setOwnerNickName(const QString &identification, const QString &nickname);
	void addServiceMessage(const QString &identification, const QString &message, const QString &date);
	void addConferenceItem(const QString &identification, const QString &nickname);
	void removeConferenceItem(const QString &identification, const QString &nickname);
	void renameConferenceItem(const QString &identification, const QString &nickname, const QString &new_nickname);
	void setConferenceItemStatus(const QString &identification, const QString &nickname, const QIcon &icon, const QString &status, int mass);
	void setConferenceItemIcon(const QString &identification, const QString &nickname,	const QIcon &icon, int position);
	void setConferenceItemRole(const QString &identification, const QString &nickname, const QIcon &icon, const QString &role, int mass);
	void setConferenceTopic(const QString &identification, const QString &topic);
protected:
	void showEvent(QShowEvent * event);
private slots:
	void insertEmoticon(const QString &emoticon_text);
	void currentTabChanged(int index);
	void closeTabAt(int index);
	void closeCurrentTab();
	void on_sendButton_clicked();
	void windowFocused();
	void newsOnLinkClicked(const QUrl &url);
	void on_historyButton_clicked();
	void on_translitButton_clicked();
	void on_onEnterButton_clicked();
	void on_quoteButton_clicked();
	void on_clearButton_clicked();
	void on_topicButton_clicked();
	void on_configButton_clicked();
private:
	void focusTextEdit();
	QString invertMessage(QString &text);
	void moveCursorToEnd();
//	void appendMessage(QString message, bool same_from);
	void windowActivatedByUser();
	QList<QString> m_tab_chat_index_list;
	void setIconsToButtons();
	void setEmoticonsToMenu();
	void setEmoticonPath(const QString &path);
	bool eventFilter(QObject *obj, QEvent *ev);
	QString checkForEmoticons(const QString &message);
	void checkForActive(ConferenceUserSpace *conf_space, const QString &message);
	QList<ConferenceUserSpace *> m_conf_spaces_chat_list;
	ChatEventEater *m_event_eater;
	AbstractChatLayer &m_abstract_chat_layer;
	Ui::ConferenceForm ui;
	ChatWindowStyleOutput *m_style_output;
	GeneralTabBar *m_chat_tab_bar;
	QToolButton *m_close_tab_button;
	QHBoxLayout *m_tab_layout;
//	QWebView *m_web_view;
	QTextBrowser *m_text_browser;
//    bool m_webkit_mode;
	QString m_webkit_style_path;
	QString m_webkit_variant;
	QString m_emoticons_path;
	QMenu *m_emotic_menu;
    QWidgetAction *m_emoticon_action;
    ChatEmoticonMenu *m_emoticon_widget;
    QHash<QString, QString> m_emoticon_list;
    QHash<QString, QString> m_urls;
	ContactListItemDelegate *m_item_delegate;
	ConferenceContactListEventEater *m_cl_event_eater;

	bool m_remove_message_after;
    quint16 m_remove_count;
    QList<quint64> m_message_positions;
    bool m_close_after_send;
    bool m_show_names;
	int m_previous_tab;
bool m_scroll_at_max;
 quint64 m_current_scroll_position;
};

#endif // TABBEDCONFERENCE_H
