/*
    jPluginSystem.h

    Copyright (c) 2008 by Nikita Belov <zodiac.nv@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef JPLUGINSYSTEM_H
#define JPLUGINSYSTEM_H

#include "jLayer.h"

class jLayer;

class jPluginSystem
{
public:
	jPluginSystem();
	static jPluginSystem &instance();
	void setJabberLayer(jLayer *jabber_layer);
        void init();

	void updateStatusIcons();
	bool addItemToContactList(TreeModelItem Item, QString name=QString());
	bool removeItemFromContactList(TreeModelItem Item); 
	bool moveItemInContactList(TreeModelItem OldItem, TreeModelItem NewItem);
	bool setContactItemName(TreeModelItem Item, QString name);
	bool setContactItemIcon(TreeModelItem Item, QIcon icon, int position); 
	bool setContactItemRow(TreeModelItem Item, QList<QVariant> row, int position);
	bool setContactItemStatus(TreeModelItem Item, QIcon icon, QString text, int mass);
	bool setStatusMessage(QString &status_message, bool &dshow);
	void addMessageFromContact(const TreeModelItem &item, const QString &message, const QDateTime &message_date);
	void addServiceMessage(const TreeModelItem &item, const QString &message);
	void addImage(const TreeModelItem &item, const QByteArray &image_raw);
	void contactTyping(const TreeModelItem &item, bool typing);
	void messageDelievered(const TreeModelItem &item, int position);
	bool checkForMessageValidation(const TreeModelItem &item, const QString &message, int message_type, bool special_status);
	QString getIconFileName(const QString &icon_name);
	QIcon getIcon(const QString &icon_name);
	QString getStatusIconFileName(const QString &icon_name, const QString &default_path);
	QIcon getStatusIcon(const QString &icon_name, const QString &default_path);
	void setAccountIsOnline(const TreeModelItem &Item, bool online);
	void createChat(const TreeModelItem &item);
	void notifyAboutBirthDay(const TreeModelItem &item);
	void systemNotifiacation(const TreeModelItem &item, const QString &message);
	void customNotifiacation(const TreeModelItem &item, const QString &message);
	void getQutimVersion(quint8 &major, quint8 &minor, quint8 &secminor, quint16 &svn);
	void createConference(const QString &protocol_name, const QString &conference_name, const QString &account_name);
	void addMessageToConference(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &from, const QString &message, const QDateTime &date, bool history);
	void changeOwnConferenceNickName(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname);
	void addConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname);
	void removeConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname) ;
	void renameConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QString &new_nickname);
	void setConferenceItemStatus(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, const QString &status, int mass);
	void setConferenceItemIcon(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, int position);
	void setConferenceItemRole(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, const QString &role, int mass);
	void setItemVisible(const TreeModelItem &item, bool visible);
	void setItemInvisible(const TreeModelItem &item, bool invisible);
	void setConferenceTopic(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &topic);
	void addSystemMessageToConference(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &message, const QDateTime &date, bool history);
        bool changeChatWindowID(const TreeModelItem &item, const QString &id);
	
	QIcon getStatusIcon(int presence);
	QString getStatusIconName(int presence);
	
	QHash<QString, QString> &getMoodTr() { return m_mood_tr; }
	QHash<QString, QString> &getGeneralActivityTr() { return m_activity_general_tr; }
	QHash<QString, QString> &getSpecificActivityTr(const QString &general) { return m_activity_tr[general]; }
private:
	jLayer *m_parent_layer;
	QHash<QString, QString> m_icons_name_hash;
	QHash<QString, QIcon> m_icons_hash;
	QHash<QString, QString> m_mood_tr;
	QHash<QString, QString> m_activity_general_tr;
	QHash<QString, QHash<QString, QString> > m_activity_tr;
};

#endif
