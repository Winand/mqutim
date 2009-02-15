/*
    jConference

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>
             2008-2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef JCONFERENCE_H_
#define JCONFERENCE_H_

#include <QString>
//#include <QObject>
#include "gloox/client.h"
#include "gloox/mucroom.h"
#include "gloox/mucroomhandler.h"
#include "gloox/message.h"
#include "gloox/discohandler.h"
#include "src/jPluginSystem.h"
#include "src/jAccount.h"
#include "src/jProtocol.h"
#include "src/jBuddy.h"
#include "src/jClientIdentification.h"
#include "src/jJoinChat.h"
#include "src/conference/topicConfigDialog.h"

class jProtocol;
class jAccount;
class jBuddy;
class jPluginSystem;
class jJoinChat;

class jConference : public QObject, MUCRoomHandler, DiscoHandler, PresenceHandler
{
	Q_OBJECT

public:
	struct MucContact;
	jConference(jProtocol *real_parent, Client *client, jAccount *plugin_system, QObject *parent=0);
	virtual ~jConference();
        void loadSettings();
	void handleMUCParticipantPresence (MUCRoom *room, const MUCRoomParticipant participant, Presence::PresenceType presence);
	void handleMUCMessage (MUCRoom *room, const Message &message, bool privateMessage);
	bool handleMUCRoomCreation (MUCRoom *room);
	void handleMUCSubject (MUCRoom *, const std::string &, const std::string &);
	void handleMUCInviteDecline (MUCRoom *, const JID &, const std::string &);
	void handleMUCError (MUCRoom *room, StanzaError error);
	void handleMUCInfo (MUCRoom *, int , const std::string &, const DataForm *);
	void handleMUCItems (gloox::MUCRoom*, const std::list<gloox::Disco::Item*, std::allocator<gloox::Disco::Item*> >&);
	void sendMessageToConference(const QString &conference_name, const QString &message);
	void leaveConference(const QString& conference_name);
	bool JIDIsRoom(const QString &jid){ return m_contacts_list.contains(jid); }
	void handleDiscoInfo(const JID &from, const Disco::Info &info, int context);
	void handleDiscoItems(const JID &from, const Disco::Items &items, int context);
	void handleDiscoError(const JID &from, const Error *error, int context);
	bool handleDiscoSet(const IQ &iq);
	void handlePresence (const Presence &presence);
	void setClient(const QString &conference, const QString &nick, const VersionExtension *version);
	void sendPresence(const QString &conference);
	void sendPresenceToAll();
	QString getToolTip(const QString &conference, const QString &nickname);
	QStringList getAdditionalInfoAboutContact(const QString &conference, const QString &nickname);
	void setAvatar(const QString &conference, const QString &nickname, const QString &path);
	void connectAll();
	void disconnectAll();
	void setConferences();
	void showTopicConfig(const QString &conference);
	void itemContextMenu(const QList<QAction*> &action_list,const QString &conference_name,
		const QString &nickname,const QPoint &menu_point);
	void chatWindowAboutToBeOpened(const QString &conference, const QString &nick);
	void chatWindowClosed(const QString &conference, const QString &nick);
	void joinGroupchat(const QString &room, const QString &password, bool showBookmarks);

public slots:
	//void joinGroupChat();
	void joinGroupchat();
	void destroyJoinGroupChat();
	void createConferenceRoom(const QString &host,
			const QString &room,
			const QString &nick,
			const QString &password);
	void s_createConferenceRoom(const QString &conference, const QString &nickname, const QString &password);
	void kickUser();
	void banUser();
	void setVisitorUser();
	void setParticipantUser();
	void setModeratorUser();
	void s_conferenceInvite(const JID &room, const JID &from, const QString &reason, const QString &password);

signals:
	void addConferenceItem(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname);
	void removeConferenceItem(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname);
	void renameConferenceItem(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname,const QString &new_nickname);
	void setConferenceItemStatus(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname,const QString &status, int mass);
	void setConferenceItemRole(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname,const QString &role, int mass);
	void addMessageToConference(const QString &conference_name, const QString &account_name,
			const QString &from, const QString &message, const QDateTime &date, bool history);
	void conferenceClientVersion(const QString &protocol_name,const QString &conference_name,
			const QString &account_name,const QString &nickname, const QString &name);
	void changeOwnConferenceNickName(const QString &protocol_name,
			const QString &conference_name, const QString &account_name, const QString &nickname);
	void createConference(const QString &protocol_name, const QString &conference_name, const QString &account_name);
	void setConferenceItemIcon(const QString &protocol_name, const QString &conference_name, const QString &account_name,
			const QString &nickname, const QString &icon_name, int position);
	void setConferenceTopic(const QString &protocol_name, const QString &conference_name,
			const QString &account_name, const QString &topic);
	void addSystemMessageToConference(const QString &protocol_name, const QString &conference_name,
			const QString &account_name, const QString &message, const QDateTime &date, bool history);

private:
	jAccount *m_jabber_account;
	Client *m_client_base;
	QHash<QString, MUCRoom *> m_room_list;
	QHash<QString, QString> m_topic_list;
	QHash<QString,QHash<QString,MucContact> *> m_contacts_list;
	QString m_account_name;
	jProtocol *m_real_parent;
	jPluginSystem &m_plugin_system;
	Presence *m_presence;
	jJoinChat *m_join_form;
	QMenu *m_context_menu;
	QLabel *m_menu_label;
	QWidgetAction *m_menu_title;
	QAction *m_kick_user;
	QAction *m_ban_user;
	QActionGroup *m_role_user;
	QAction *m_user_visitor;
    QAction *m_user_participant;
	QAction *m_user_moderator;
	QString m_current_context_nick;
	QString m_current_context_conference_name;
        bool m_auto_retrieve_vcards;
};

class BookmarkAction: public QObject
{
	Q_OBJECT

public:
	BookmarkAction(const ConferenceListItem &item, jConference *cmo, QObject *parent);
	BookmarkAction(const BookmarkListItem &item, QObject *parent);
public slots:
	void triggered();
private:
	bool m_is_conference;
	ConferenceListItem m_conference_item;
	BookmarkListItem m_url_item;
	jConference *m_cmo;

};

#endif /*JCONFERENCE_H_*/
