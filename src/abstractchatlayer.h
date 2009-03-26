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

#include <QHash>
#include <QString>
#include <QDebug>
#include <QDateTime>

#include "messagedispatcher.h"
#include "pluginsystem.h"

class AbstractChatLayer
{
public:
  /** NEW API **/
  void messageReceived(const Message &msg);
  
  /** Deprecated API **/
  
  void contactTyping(const TreeModelItem& item, bool typing);
  void messageDelievered(const TreeModelItem &item, int message_position);
  void sendTypingNotification(const QString &protocol_name, const QString &account_name, const QString &contact_name,
      int notification_type);
  void createChat(const TreeModelItem &item, bool new_chat = false);
  
  void addMessage(const TreeModelItem &item, const QString &message, bool in,
      QDateTime message_date, bool save_history=true,
      bool unreaded_message = false, bool history=false, const QString &not_modified = "");
//   void addModifiedMessage(const TreeModelItem &item, const QString &message, bool in,
//       QDateTime message_date, bool save_history=true,
//       bool unreaded_message = false);
  void addServiceMessage(const TreeModelItem &item, const QString &message);
	static AbstractChatLayer &instance();
  
	void sendMessageTo(const QString &protocol_name, const QString &account_name,
			const QString &contact_name, const QString &message, int message_icon_position, bool from_plugin = false);

	void windowActivated(const QString &protocol_name, const QString &account_name,
			const QString &item_name);

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
	AbstractChatLayer();
	~AbstractChatLayer();

	PluginSystem &m_plugin_system;

	MessageDispatcher dispatcher;
};

#endif /*ABSTRACTCHATLAYER_H_*/
