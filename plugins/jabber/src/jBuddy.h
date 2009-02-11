/*
    jBuddy.h

    Copyright (c) 2008-2009 by Nikita Belov <zodiac.nv@gmail.com>
					Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef JBUDDY_H
#define JBUDDY_H

#include "jAccount.h"
#include "jPluginSystem.h"
#include <gloox/chatstate.h>

class jAccount;
class jPluginSystem;

class jBuddy {
public:
	struct SimpleInfo
	{
		QString m_html;
                QString m_icon;
		int m_position;
	};
	struct ResourceInfo
	{
		Presence::PresenceType m_presence;
		QString m_name;
		int m_priority;
		QString m_status_message_html;
		QString m_status_message;
		QString m_caps_node;		
		QString m_caps_ver;
		QString m_client_name;
		QString m_client_version;
		QString m_client_os;
		ChatStateType m_chat_state;
		QStringList m_features;
		SimpleInfo m_xstatus;
		bool m_in_cl;
	};
	jBuddy(const QString &jid, const QString &group, const QString &path_to_avatars);
	virtual ~jBuddy();

	static QString getBaseToolTip(const ResourceInfo &info);
	
	void addResource(const QString & resource, int priority, Presence::PresenceType presence);
	void delResource(const QString & resource);
	void setName(const QString &name) { m_name = name; }
	QString getName() { return m_name; }
	bool resourceExist(const QString & resource, bool hard = false);
	void newMaxPriorityResource();
	QStringList getResources() { return m_resources_info.keys(); }
	QString getToolTip(const QString &resource="");
	ResourceInfo *getResourceInfo(const QString &name);
	//void setResourceInfo(const QString &name, const ResourceInfo &info);
	QString getMaxPriorityResource();
	QString getGroup();
	void setGroup(const QString &group) { m_group = group; }
	int getCountResources();
	QString getAvatarHash() { return m_avatar_hash; }
	void setAvatarHash(const QString &hash) { m_avatar_hash = hash; }
	Presence::PresenceType getMaxPriorityStatus();
	void setSimpleInfo(const QString &name, const SimpleInfo &info);
        SimpleInfo &getSimpleInfo(const QString &name) { return m_simple_info[name]; }
        void setSubscription(SubscriptionType type) { m_subscription_type = type; }
        SubscriptionType getSubscription() { return m_subscription_type; }
	ChatStateType m_chat_state;
	QString m_mood;
	QString m_mood_name;
private:
	jPluginSystem &m_plugin_system;
	QString m_jid;
	QString m_group;
	QString max_priority_resource;
	QString m_name;
	QString m_avatar_hash;
	QString m_path_to_avatars;
	QHash <QString, ResourceInfo> m_resources_info;
	QHash <QString, SimpleInfo> m_simple_info;
	int count_resources;
	int max_priority_resource_value;
        SubscriptionType m_subscription_type;
};

#endif
