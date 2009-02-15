/*
    jRoster.h

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

#ifndef JROSTER_H
#define JROSTER_H

#include "jLayer.h"
#include "jProtocol.h"
#include <QWidgetAction>
#include <QLabel>

class jBuddy;
class jAccount;
class jProtocol;
class jConference;

using namespace gloox;

class jRoster : public QObject
{
	Q_OBJECT
public:
	jRoster(const QString &account_name, const QString &profile_name, jAccount *jabber_account);
	~jRoster();
	void constr();
        void loadSettings();
        void loadRosterFromSettings();
	void setRosterManager(RosterManager *roster_manager) { m_roster_manager = roster_manager; }

	void startLoadRoster(){ m_buddies.clear(); }
	void stopLoadRoster();
	void createMyConnectionsGroup();
	void addGroup(const QString &group_name, bool use_qsettings=true);
	void addItem(const QString &item_name, const QString &contact_name, const QString &parent_name, QString history_name = "", Presence::PresenceType presence = Presence::Unavailable, bool invisible = false);
	void delItem(const QString &item_name, const QString &parent_name);
	void moveItem(const QString &item_name, const QString &parent_name, const QString &new_parent_name);
	void renameItem(const QString &item_name, const QString &contact_name, const QString &parent_name);
        jBuddy *addContact(const QString &item_name, const QString &contact_name, const QString &parent_name, bool use_qsettings=true);
	void renameContact(const QString &item_name, const QString &contact_name);
	void moveContact(const QString &item_name, const QString &parent_name);
	void removeContact(const QString &item_name);
	void addMyConnect(const QString &resource, int priority, Presence::PresenceType presence);
	void addResource(const QString &jid, const QString &resource, int priority, Presence::PresenceType presence);
	void delMyConnect(const QString &resource);
	void delResource(const QString &jid, const QString &resource);
	void changeItemStatus(const QString &item_name, Presence::PresenceType presence);
	void setInvisible(const QString &item_name, const QString &parent_name, quint8 item_type);
	void setStatusMessage(const QString &jid, const QString &resource, const QString &message);
	bool groupExist(const QString &group_name);
	bool contactExist(const QString &jid);
	bool myConnectExist(const QString &resource);
	jBuddy *getBuddy(const QString &jid){return jid==m_account_name?m_my_connections:m_roster.value(jid,0);}
	QString getMaxPriorityResource(const QString &jid);
	void setClient(const QString &jid, const QString &resource  = QString(), const QString &name = QString(), bool my_connection=false);
	void setOffline();
	QString getToolTip(const QString &jid);
	void setAvatar(const QString &jid, const QString &hash);
	void setStatusRow(const QString &jid);
	void customNotification(const QString &item_name, const QString &message);
	void itemContextMenu(const QList<QAction*> &action_list, const QString &contact_name, int item_type, const QPoint &menu_point);
	void setBirthday(const JID &jid, QDate &birthday);
	void chatWindowOpened(const QString &item_name);
	void chatWindowAboutToBeOpened(const QString &item_name);
	void chatWindowClosed(const QString &item_name);
	QStringList getGroups();
        void updateIcon(const QString &jid, const QString &name);

signals:
	void addItemToContactList(const TreeModelItem &item, QString item_name);
	void removeItemFromContactList(TreeModelItem Item);
	void setContactItemStatus(const TreeModelItem &item, QString status, int mass);
	void setItemInvisible(const TreeModelItem &item, bool invisible);
	void clientVersion(const TreeModelItem &contact, const QString &name);
	void setItemIcon(const TreeModelItem &, const QString &, int);
	void setItemCustomText(const TreeModelItem &, const QString &, int);
	void s_customNotification(const TreeModelItem &, const QString &);
	void moveItemInContactList(TreeModelItem OldItem, TreeModelItem NewItem);
	void setContactItemName(TreeModelItem Item, QString name);
	void addServiceMessage(const TreeModelItem &item, const QString &text);

public slots:
	void onRenameAction();
	void onDeleteAction();
	void onMoveAction();
	void onSendSubscriptionAction();
	void onAskSubscriptionAction();
	void onRemoveSubscriptionAction();
	void onSendMessage();
        void onAddContact();
	void onRegisterAction();
	void onUnregisterAction();
	void onLogInTransportAction();
	void onLogOutTransportAction();

private:
	void updateItemIcon(const TreeModelItem &, const QIcon &, const int &);

	QString m_account_name;
	QString m_profile_name;

	jPluginSystem &m_plugin_system;
	RosterManager *m_roster_manager;

	QStringList m_groups;
	QStringList m_buddies;
	QString m_path_to_avatars;
	QString m_list_string;
	jBuddy *m_my_connections;
	QHash<QString, jBuddy *> m_roster;
	jAccount *m_jabber_account;
	jProtocol *m_jabber_protocol;
	jConference *m_conferences;
	int m_menu_type;
	QString m_menu_name;
	QLabel *m_menu_label;
	QWidgetAction *m_menu_title;
        QAction *m_add_contact_action;
	QAction *m_rename_action;
	QAction *m_delete_action;
	QAction *m_move_action;
	QMenu *m_subscription_menu;
	QAction *m_send_subscription_action;
	QAction *m_ask_subscription_action;
	QAction *m_remove_subscription_action;
	QMenu *m_transports_menu;
	QAction *m_register_transport;
	QAction *m_unregister_transport;
	QAction *m_login_transport;
	QAction *m_logout_transport;
        QList<QAction *> m_menu_resources;
        bool m_show_mood;
        bool m_show_activity;
        bool m_show_tune;
        bool m_show_xstatus;
        bool m_show_both_activity;
};

#endif
