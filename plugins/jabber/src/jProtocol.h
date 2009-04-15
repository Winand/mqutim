/*
jProtocol.cpp

Copyright (c) 2008 by Nikita Belov <zodiac.nv@gmail.com>
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

#ifndef JPROTOCOL_H
#define JPROTOCOL_H

#include <QHash>
//#include <QHashIterator>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QDebug>

#include "jInvitationHandler.h"

#include <gloox/client.h>
#include <gloox/capabilities.h>
#include <gloox/connectionlistener.h>
#include <gloox/discohandler.h>
#include <gloox/disco.h>
#include <gloox/presencehandler.h>
#include <gloox/rostermanager.h>
#include <gloox/messagehandler.h>
#include <gloox/message.h>
#include <gloox/chatstatehandler.h>
#include <gloox/chatstate.h>
#include <gloox/iqhandler.h>
#include <gloox/pubsubmanager.h>
#include <gloox/vcard.h>
#include <gloox/vcardmanager.h>
#include <gloox/vcardhandler.h>
#include <gloox/bookmarkstorage.h>
#include <gloox/pubsubeventhandler.h>
#include <gloox/pubsubresulthandler.h>
#include <gloox/error.h>
#include <gloox/eventhandler.h>

#include <qutim/plugininterface.h>
#include "../include/qutim/jabber.h"

#include <string>

using namespace gloox;
using namespace qutim_sdk_0_2;
using namespace qutim_sdk_jabber;

#include "jTransport.h"
#include "jConnection.h"
#include "jAccount.h"
#include "jBuddy.h"
#include "systeminfo.h"
#include "xeps/versionextension.h"
#include "xeps/moodsextenstion.h"
#include "xeps/xstatusextension.h"
#include "xeps/gmailextension.h"
#include "xeps/activityextension.h"
#include "xeps/tuneextension.h"
#include "conference/jConference.h"
#include <QDateTime>
#include <string>
#include <list>

class jAccount;
class jRoster;
class jBuddy;
class jConference;

class jProtocol : public QThread, RosterListener, ConnectionListener, MessageHandler,
		PresenceHandler, IqHandler, DiscoHandler, VCardHandler, SubscriptionHandler,
		BookmarkHandler, LogHandler, PubSub::EventHandler, PubSub::ResultHandler, gloox::EventHandler
{
	Q_OBJECT

public:
	jProtocol(const QString &account_name, const QString &profile_name, jRoster *jabber_roster, jAccount *parent);

	void loadSettings();

	~jProtocol();
	static QString fromStd(const std::string &str);
	static std::string toStd(const QString &str);
	static QDateTime fromStamp(const QString &stamp);
	static QString getBare(const QString &full);
	static QString getResource(const QString &full);
	jConference *getConferenceManagmentObject(){return m_conference_management_object;}
	void setStatus(Presence::PresenceType presence);
	BookmarkStorage *getBookmarkStorage() { return m_bookmark_storage; }
	Client *getClient() { return jClient; }

	void run();

	void onConnect();
	void onDisconnect(ConnectionError e);
	bool onTLSConnect(const CertInfo& info);
	void onResourceBindError(ResourceBindError error);
	void onSessionCreateError(SessionCreateError error);
	void handleItemSubscribed(const JID& jid);
	void handleItemAdded(const JID& jid);
	void handleItemUnsubscribed(const JID& jid);
	void handleItemRemoved(const JID& jid);
	void handleItemUpdated(const JID& jid);
	void handleRoster(const Roster& roster);
	void handleRosterError(Stanza *stanza);
	void handlePresence (const Presence &presence);
	void handleSubscription (const Subscription &s10n);
	void handleRosterPresence(const RosterItem& item, const std::string& resource, Presence::PresenceType presence, const std::string &msg);
	void handleSelfPresence(const RosterItem& item, const std::string& resource, Presence::PresenceType presence, const std::string &msg);
	void handleNonrosterPresence(const gloox::Presence&);
	void handleRosterError(const gloox::IQ&);
	bool handleSubscriptionRequest(const JID& jid, const std::string &msg);
	bool handleUnsubscriptionRequest(const JID& jid, const std::string &msg);
	void handleNonrosterPresence(Stanza* stanza);
	void handleMessage(const Message &stanza, MessageSession *session);
	void sendMessageTo(const QString &contact_name, const QString& message, int message_icon_position);
	void sendTypingNotification(const QString &item_name, int notification_type);
	bool handleIq(const IQ &iq);
	void handleIqID(const IQ &iq, int context);
	void handleDiscoInfo(const JID &from, const Disco::Info &info, int context);
	void handleDiscoItems(const JID &from, const Disco::Items &items, int context);
	void handleDiscoError(const JID &from, const Error *error, int context);
	bool handleDiscoSet(const IQ &iq);
	QStringList getAdditionalInfoAboutContact(const QString &item_name);
	void fetchVCard(const QString &jid, bool is_auto=false);
	void storeVCard(VCard *vcard);
	void setAvatar(const JID &jid, const QString &hash);
	void setResource(const QString &resource, int priority);
	inline QString getResource() { return m_resource; }
	inline int getPriority() { return m_priority; }
	void updateAvatarPresence(const QString &hash);
	QString getAccountName() { return m_my_nick; }
	void moveContact(const QString &item_name, const QString &parent_name);
	void addContact(const QString &jid, QString name, const QString &group, bool authorize);
	void setActions(QAction *bookmark, QAction *mood, QAction *activity);
	QAction *getBookmarkAction() { return m_bookmark_action; }
	void handleBookmarks(const BookmarkList &b_list, const ConferenceList &c_list);
	QMenu *getBookmarkMenu() { return m_bookmark_menu; }
	void setBookmarkMenu(QMenu *menu) { m_bookmark_menu = menu; }
	void handleLog (LogLevel level, LogArea area, const std::string &message);
	void getDiscoItems(const QString &jid, const QString &node, DiscoHandler *handler = 0);
	void requestBookmarks();
	void getDiscoInfo(const QString &jid, const QString &node, DiscoHandler *handler = 0);
	bool hasFeature(const QString &feature) { return qBinaryFind(m_server_features,feature)!=m_server_features.end(); }
	void handleNodeCreation (const JID &service, const std::string &node, const DataForm *config) {}
	void handleNodeRemoval (const JID &service, const std::string &node) {}
	void handleItemPublication (const JID &service, const std::string &node, const std::string &item, const Tag *entry);
	void handleItemRemoval (const JID &service, const std::string &node, const std::string &item);
	void handleSubscriptionChange (const JID &service, const JID &jid, const std::string &node, const std::string &reason, PubSub::SubscriptionType type) {}
	void handleConfigurationChange (const JID &service, const std::string &node, const DataForm *config) {}
	void handleNodePurge (const JID &service, const std::string &node) {}
	void handleItem (const JID &service, const std::string &node, const Tag *entry) {}
	void handleItems (const JID &service, const std::string &node, const  PubSub::ItemList &itemList, const Error *error=0) {}
	void handleItemPublication (const JID &service, const std::string &node, const std::string &item, const Error *error=0) {}
	void handleItemDeletion (const JID &service, const std::string &node, const  PubSub::ItemList &itemList, const Error *error=0) {}
	void handleSubscriptionResult (const JID &service, const std::string &node, const std::string &sid, const JID &jid, const  PubSub::SubscriptionType subType, const Error *error=0) {}
	void handleUnsubscriptionResult (const JID &service, const std::string &node, const std::string &sid, const JID &jid, const Error *error=0) {}
	void handleSubscriptionOptions (const JID &service, const JID &jid, const std::string &node, const DataForm *options, const Error *error=0) {}
	void handleSubscriptionOptionsResult (const JID &service, const JID &jid, const std::string &node, const Error *error=0) {}
	void handleSubscribers (const JID &service, const std::string &node, const  PubSub::SubscriberList *list, const Error *error=0) {}
	void handleSubscribersResult (const JID &service, const std::string &node, const  PubSub::SubscriberList *list, const Error *error=0) {}
	void handleAffiliates (const JID &service, const std::string &node, const  PubSub::AffiliateList *list, const Error *error=0) {}
	void handleAffiliatesResult (const JID &service, const std::string &node, const  PubSub::AffiliateList *list, const Error *error=0) {}
	void handleNodeConfig (const JID &service, const std::string &node, const DataForm *config, const Error *error=0) {}
	void handleNodeConfigResult (const JID &service, const std::string &node, const Error *error=0) {}
	void handleNodeCreation (const JID &service, const std::string &node, const Error *error=0) {}
	void handleNodeDeletion (const JID &service, const std::string &node, const Error *error=0) {}
	void handleNodePurge (const JID &service, const std::string &node, const Error *error=0) {}
	void handleSubscriptions (const JID &service, const  PubSub::SubscriptionMap &subMap, const Error *error=0) {}
	void handleAffiliations (const JID &service, const  PubSub::AffiliationMap &affMap, const Error *error=0) {}
	void handleDefaultNodeConfig (const JID &service, const DataForm *config, const Error *error=0) {}
	void getMessageFromPlugins(const QList<void *> &event);
	void handleEvent (const Event &event);
	void showTransportReg(const QString &jid);
	void removeTransportReg(const QString &jid);
	void LogOutTransport(const QString &jid);
	void LogInTransport(const QString &jid);
	void setRecentBookmarks(const BookmarkList &b_list, const ConferenceList &c_list);

signals:
	void setRealStatus(int presence);
	void setClientCustomIcon(const TreeModelItem &, const QString &, int);
	void setClientCustomText(const TreeModelItem &, const QString &, int);
	void addMessageFrom(const TreeModelItem &item, const QDateTime &, const QString &);
	void messageDelievered(const TreeModelItem &item, int position);
	void sendTypingNotification(const TreeModelItem &item, bool composing);
	void conferenceClientVersion(const QString &protocol_name,const QString &conference_name,const QString &account_name,const QString &nickname, const QString &name);
	void systemNotification(const QString &account_name, const QString &message);
	void onFetchVCard(const QString &m_account_name, const VCard *vcard, const QString &avatarUrl);
	void createAcceptAuthDialog(const QString &text, const JID &jid, Client *jabber_client);
	void bookmarksHandled(const BookmarkList &b_list, const ConferenceList &c_list);
	void tagHandled(const QString &xml, bool in);
	void receiveSDisco(const JID &from, const Disco::Items &items, const QString &type);
	void receiveSDiscoInfo(const JID &from, const Disco::Info &info, const QString &type);
	void conferenceInvite(const JID &room, const JID &from, const QString &reason, const QString &password);

public slots:
	void addMessageFrom(const JID &from, const Message &stanza);
	void onSetMood();
	void onSetActivity();
	void onTimer();

private slots:
	void receiveInvite(const JID &room, const JID &from, const QString &reason, const QString &password);

private:
	void handleVCard(const JID &jid, const VCard *vcard);
	void handleVCardResult(VCardContext context, const JID &jid, StanzaError se);
	void clearRecentBookmarks();
	jRoster *m_jabber_roster;
	jAccount *m_jabber_account;
	jConference *m_conference_management_object;
	Client *jClient;
	VCardManager *m_vcardManager;
	jInvitationHandler *m_invitationHandler;
	BookmarkStorage *m_bookmark_storage;
	QMenu *m_bookmark_menu;
	QAction *m_bookmark_action;
	QAction *m_mood_action;
	QAction *m_activity_action;
	QTimer m_ping_timer;
	int m_server_ping;
//	QTimer *m_gloox_timer;
//	bool m_do_recv;
//	QMutex m_timer_mutex;
	jConnection *m_connection;

	QString m_account_name;
	QString m_profile_name;
	QString m_resource;
	QString m_avatar_hash;
	QString m_my_nick;
	int m_priority;
	QHash <QString, bool> m_composing_hash;
	QHash <QString,QPair<QString,int> > m_receipts_hash;
	QList<QString> m_jids_auto_vcard;
	QStringList m_server_features;

	int m_xmpp_port;
	QString m_xmpp_server;
	int m_proxy_type;
	int m_proxy_port;
	QString m_proxy_host;
	quint64 m_last_gmail_date;
	PubSub::Manager *m_pubsub_manager;
	QString m_current_mood;
	QPair<QString,QString> m_current_activity;

	bool m_uExit;
	bool m_pep_support;
        bool m_auto_retrieve_vcards;
        int m_gmail_notify_type;
};

#endif
