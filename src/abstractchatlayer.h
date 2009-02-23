/*
    AbstractChatLayer

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

#ifndef ABSTRACTCHATLAYER_H_
#define ABSTRACTCHATLAYER_H_

#include "pluginsystem.h"
#include <QHash>
#include <QString>
#include <QDebug>
#include <QDateTime>

class SeparateChatWindow;
class SeparateConference;
struct UnreadedMessage
{
	QString m_protocol_name;
	QString m_account_name;
	QString m_contact_name;
	QString m_message;
	QDateTime m_message_time;
};

class AbstractChatLayer
{
public:
	static AbstractChatLayer &instance();
	QString findUrls(const QString &sourceHTML);
	void loadProfile(const QString &profile_name);
	void createChat(const TreeModelItem &item, bool new_chat = false);
	void removeSeparateWindowFromList(const QString &identification);
	void removeSeparateConferenceFromList(const QString &identification);
	void addMessage(const TreeModelItem &item, const QString &message, bool in,
			QDateTime message_date, bool save_history=true,
			bool unreaded_message = false, bool history=false, const QString &not_modified = "");
	void addModifiedMessage(const TreeModelItem &item, const QString &message, bool in,
			QDateTime message_date, bool save_history=true,
			bool unreaded_message = false);
	void sendMessageTo(const QString &protocol_name, const QString &account_name,
			const QString &contact_name, const QString &message, int message_icon_position, bool from_plugin = false);
	void addServiceMessage(const TreeModelItem &item, const QString &message);
	void readAllUnreadedMessages();
	void showContactInformation(const QString &protocol_name, const QString &account_name
			,const QString &contact_name);
	void sendImageTo(const QString &protocol_name, const QString &account_name, const QString &item_name,
			const QByteArray &image_raw);
	void addImage(const TreeModelItem &item, const QByteArray &image_raw, bool in);
	void sendFileTo(const QString &protocol_name, const QString &account_name, const QString &contact_name);
	void loadSettings();
	void sendTypingNotification(const QString &protocol_name, const QString &account_name, const QString &contact_name,
			int notification_type);
	void contactTyping(const TreeModelItem& item, bool typing);
	void messageDelievered(const TreeModelItem &item, int message_position);
	void animateTrayIconMessage();
	void windowActivated(const QString &protocol_name, const QString &account_name,
			const QString &item_name);
/*	void saveWindowSizeAndPosition(const QPoint &position, const QSize &size
                        , const QByteArray &splitter_state);*/
/*	void saveConferenceSizeAndPosition(const QPoint &position, const QSize &size
                        , const QByteArray &splitter_state, const QByteArray &splitter_state2);*/
	void restoreData();
	void reloadContent();
	void addItemToActivationList(const QString &protocol_name, const QString &account_name,
			const QString &item_name, const QString &message);
	void contactChageStatusIcon(const TreeModelItem &item, const QIcon &icon);
	void contactChangeCLientIcon(const TreeModelItem &item);
	void askForContactMenu(const QString &protocol_name,
			const QString &account_name,
			const QString &item_name,
			const QPoint &menu_point);
        bool createConference(const QString &protocol_name,
			const QString &conference_name, const QString &account_name);
	void addMessageToConference(const QString &protocol_name,
			const QString &conference_name, const QString &account_name,
			const QString &from,
			const QString &message, const QDateTime &date, bool history);
	void sendMessageToConference(
			const QString &protocol_name,
			const QString &conference_name, const QString &account_name,
			const QString &message);
	void changeOwnConferenceNickName(const QString &protocol_name,
				const QString &conference_name,
				const QString &account_name,
				const QString &nickname);
	void setConferenceTopic(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &topic);
	void addSystemMessageToConference(const QString &protocol_name,
			const QString &conference_name, const QString &account_name,
			const QString &message, const QDateTime &date, bool history);
        bool addConferenceItem(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname);
        bool removeConferenceItem(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname);
        bool renameConferenceItem(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname,
			const QString &new_nickname);
        bool setConferenceItemStatus(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname,
			const QIcon &icon, const QString &status, int mass);
        bool setConferenceItemIcon(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname,
			const QIcon &icon, int position);
        bool setConferenceItemRole(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname,
			const QIcon &icon, const QString &role, int mass);
	void addSeparator(const TreeModelItem &item);
	void sendMessageFromPlugin(const TreeModelItem &item, const QString &message);
	void showConferenceTopicSettings(const QString &protocol_name, const QString &account_name, const QString &conference);
	QStringList getConferenceItemsList(const QString &protocol_name, const QString &conference_name,
			const QString &account_name);
        bool changeChatWindowID(const TreeModelItem &item, const QString &id);
private:
	void readUnreaded(const TreeModelItem &item);
//	void restoreWindowSizeAndPosition(QWidget *window);
//	void restoreConferenceSizeAndPosition(QWidget *window);
	void saveOpenedChatWindows();
	void restoreOpenedWindows();
	void saveUnreadedMessages();
	void restoreUnreadedMessages();
	QString m_profile_name;
	QString m_tmp_image_path;
	AbstractChatLayer();
	~AbstractChatLayer();
	QHash<QString, SeparateChatWindow*> m_separate_window_list;
	QHash<QString, SeparateConference*> m_separate_conference_list;
	PluginSystem &m_plugin_system;
//	bool m_webkit_mode;
	QList<UnreadedMessage> m_unreaded_messages_list;
	bool m_remove_messages_after;
	bool m_close_after_send;
	bool m_open_on_new;
	quint16 m_remove_count;
	bool m_dont_show_tray_event;
	bool m_show_names;
	quint8 m_timestamp_type;
	bool m_do_not_show_if_open;
	QString getTimeStamp(const QDateTime &message_date);
	bool m_send_on_enter;
	bool m_send_typing_notifications;
	bool m_remember_tabs_on_closing;
	QList<TreeModelItem> m_waiting_for_activation_list;
	QString m_emoticons_path;
	QString m_chat_form_path;
	QString m_webkit_style_path;
	QString m_webkit_variant;
	bool m_open_all_after_click_on_tray;
	QHash<QString, TreeModelItem> m_unreaded_contacts;
	bool m_dont_show_content_in_notifications;
};

#endif /*ABSTRACTCHATLAYER_H_*/
