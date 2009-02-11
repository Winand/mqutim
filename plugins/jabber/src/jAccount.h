/*
    jAccount.h

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

#ifndef JACCOUNT_H
#define JACCOUNT_H

//#include <QtGui/QWidget>
#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QHBoxLayout>
#include <QSettings>
#include <QVector>
#include <QToolButton>

#include "gloox/client.h"

#include "jProtocol.h"
#include "jPluginSystem.h"
#include "jRoster.h"
#include "jSlotSignal.h"
#include "changeResource.h"
#include "conference/jConference.h"

#include "jVCard.h"
#include "jAddContact.h"
#include "jServiceBrowser.h"

using namespace gloox;

class jPluginSystem;
class jProtocol;
class jSlotSignal;
class jRoster;
class jConference;
class jVCard;
class VCardRole;
class jJoinChat;
class jSearchConference;

class jAccount : public QObject
{
	Q_OBJECT
public:
	jAccount(const QString &account_name, const QString &profile_name, QObject *parent = 0);
	~jAccount();
	void createAccountButton(QHBoxLayout *account_button_layout);
	QMenu *getAccountMenu();
	void removeMenuAccount(QMenu *account_menu);
	void removeAccountButton();

	inline jProtocol *getProtocol() const { return m_jabber_protocol; }
	inline jRoster *getJabberRoster() const { return m_jabber_roster; }
	inline jPluginSystem &getPluginSystem() const { return m_jabber_plugin_system; }
	inline jConference *getConferenceManagementObject() const { return m_conference_management_object; }
	inline jSlotSignal *getSlotSignal() const {return m_jabber_slotsignal;}
	inline QString getAccountName() const {return m_account_name;}
	inline QString getProfileName() const {return m_profile_name;}
	Presence::PresenceType getPresence() { return m_status; }

	QString getStatusIconName(Presence::PresenceType presence);
	static QString getStatusName(Presence::PresenceType presence);
	static int getStatusMass(Presence::PresenceType presence);
	void showVCardWidget(const QString &jid);
	void setVCardInfo(const VCard *vcard, const QString &jid, const QString &avatarUrl);
	void setDiscoItems(const JID &from, const Disco::Items &items);
        const QString &getPathToAvatars() { return m_path_to_avatars; }
        const QIcon &getStatusIcon() { return m_status_icon; }
	void setConferences();
        void setStatusN(Presence::PresenceType presence);
        const QString &getAccountName() {return m_account_name;}
        const QString &getProfileName() {return m_profile_name;}

public slots:
	void setRealStatus(int status);
	void getDiskoItem(const QString &server, const QString &node, DiscoHandler *handler);
	void getDiskoInfo(const QString &server, const QString &node, DiscoHandler *handler);
        void showAddDialog(const QString &jid="", const QString &nick="");

private slots:
	void showTransportsBrowser();
	void showOtherServicesBrowser();
	void setStatus();
	void s_requestVCard(const QString &jid);
	void s_saveVCard(VCard *vcard);
	void s_closeVCard(const QString &jid);
	void showChangeResourceDialog();
        void s_requestDiscoItems(const QString &server, const QString &node);
	void addContact(const QString &jid, const QString &name, const QString &group, bool authorize);
	void showInformation(const QString &jid) {showVCardWidget(jid);}

private:
	void createStatusMenu();

	QToolButton *m_account_button;

	jSlotSignal *m_jabber_slotsignal;
	jProtocol *m_jabber_protocol;
	jRoster *m_jabber_roster;
	jConference *m_conference_management_object;

	QAction *onlineAction;
	QAction *offlineAction;
	QAction *ffcAction;
	QAction *awayAction;
	QAction *xaAction;
	QAction *dndAction;
	QHash<QString, QAction *> statusMenuActions;

	QMenu *m_conference_menu;
	QAction *m_join_groupchat;

	QMenu *m_additional_menu;
	QAction *m_join_group_chat;
	QAction *m_change_resource;
	QAction *m_open_xml_console;
	QAction *m_add_contact;
	QAction *m_transport_browser;
	QAction *m_other_browser;

	QMenu *m_status_menu;
	QString m_account_name;
	QString m_profile_name;
	QString m_path_to_avatars;

	jPluginSystem &m_jabber_plugin_system;

	Presence::PresenceType m_status;
	QIcon m_status_icon;

	QHash<QString, jVCard*> m_vcard_list;
};

#endif
