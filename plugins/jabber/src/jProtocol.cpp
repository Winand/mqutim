/*
jProtocol.cpp

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

#include "jProtocol.h"
#include "jClientIdentification.h"
#include "customstatusdialog.h"
#include "activitydialog.h"
#include <gloox/capabilities.h>
#include <gloox/receipt.h>
#include <gloox/vcardupdate.h>
#include <gloox/sha.h>
#include <gloox/connectionsocks5proxy.h>
#include <gloox/connectionbosh.h>
#include <gloox/connectionhttpproxy.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/pubsubitem.h>
#include <gloox/nickname.h>
#include <QNetworkProxy>

jProtocol::jProtocol(const QString &account_name, const QString &profile_name, jRoster *jabber_roster, jAccount *parent) : m_jabber_roster(jabber_roster), m_jabber_account(parent)
{
	m_account_name = account_name;
	m_profile_name = profile_name;
	qDebug() << m_account_name;
	/*CustomStatusDialog *dialog = new CustomStatusDialog(m_account_name,m_profile_name);
	dialog->setStatuses("");
	dialog->show();
	dialog->exec();
	*/
	JID jid(toStd(m_account_name));
	jClient = new Client(jid,"");
        m_connection = new jConnection(jClient, m_profile_name, m_account_name);
	jClient->setConnectionImpl(m_connection);

	jClient->logInstance().registerLogHandler(LogLevelWarning,LogAreaClassDns,this);
	loadSettings();

	quint8 qutimver_major, qutimver_minor, qutimver_secminor;
	quint16 qutimver_svn;
	m_jabber_account->getPluginSystem().getQutimVersion(qutimver_major, qutimver_minor, qutimver_secminor, qutimver_svn);
	QString qutimver = QString("%1.%2.%3").arg(qutimver_major).arg(qutimver_minor).arg(qutimver_secminor)+(qutimver_svn>0?QString(" r%1").arg(qutimver_svn):"");

	//VCardManager *m_vcardManager;

	m_conference_management_object = new jConference(this, jClient, parent);
//	qDebug() << m_conference_management_object;

	((Capabilities*)jClient->presence().capabilities())->setNode("http://qutim.org");
	jClient->disco()->addFeature("http://jabber.org/protocol/bytestreams");
	jClient->disco()->addFeature("http://jabber.org/protocol/si");
//	jClient->disco()->addFeature("http://jabber.org/protocol/si/profile/file-transfer");
	jClient->disco()->addFeature("http://jabber.org/protocol/disco#info");
//	jClient->disco()->addFeature("http://jabber.org/protocol/commands");
	jClient->disco()->addFeature("http://jabber.org/protocol/rosterx");
	jClient->disco()->addFeature("http://jabber.org/protocol/muc");
	jClient->disco()->addFeature("jabber:x:data");
	jClient->disco()->addFeature("http://jabber.org/protocol/chatstates");
	jClient->disco()->addFeature("http://jabber.org/protocol/activity+notify");
	jClient->disco()->addFeature("http://jabber.org/protocol/mood+notify");
	jClient->disco()->addFeature("http://jabber.org/protocol/tune+notify");
	jClient->disco()->addFeature("http://jabber.org/protocol/physloc+notify");
	jClient->disco()->addFeature("http://jabber.org/protocol/geoloc+notify");
	jClient->disco()->addFeature("http://www.xmpp.org/extensions/xep-0084.html#ns-metadata+notify");
	jClient->disco()->addFeature("urn:xmpp:receipts");
//	jClient->disco()->addFeature("http://jabber.org/protocol/xhtml-im");
//	jClient->disco()->addFeature("urn:xmpp:tmp:sxe");
//	jClient->disco()->addFeature("http://www.w3.org/2000/svg");
	jClient->disco()->setVersion("qutIM", toStd(qutimver), toStd(SystemInfo::instance()->os()));
	jClient->disco()->setIdentity("client", "pc","qutIM");
	jClient->disco()->addIdentity("client", "pc","qutIM/"+toStd(qutimver)+"/"+toStd(SystemInfo::instance()->os()));
//	m_conference_management_object = new jConference(this, jClient,m_plugin_system);
//      jClient->setCompressionImpl(new jCompression(jClient));
//      CompressionBase* cmp = new CompressionZlib( this );
//      qDebug() << mp->init();
//      jClient->setCompressionImpl(cmp);
	jClient->registerConnectionListener(this);
	jClient->registerMessageHandler(this);
        jClient->logInstance().registerLogHandler(LogLevelDebug,LogAreaXmlIncoming|LogAreaXmlOutgoing,this);
	jClient->registerPresenceHandler(this);
	jClient->registerSubscriptionHandler( this );
	jClient->registerIqHandler(this,ExtVersion);
	jClient->registerIqHandler(this,ExtUser+2);
	m_invitationHandler = new jInvitationHandler(jClient);
	jClient->registerMUCInvitationHandler(m_invitationHandler);
	connect(m_invitationHandler, SIGNAL(conferenceInvite(const JID&, const JID&, const QString&, const QString&)), this, SLOT(receiveInvite(const JID&, const JID&, const QString&, const QString&)));
	jClient->setPresence(Presence::Available, m_priority);
	jClient->rosterManager()->registerRosterListener(this,false);
	jClient->disco()->registerDiscoHandler(this);
	m_vcardManager = new VCardManager(jClient);
	m_bookmark_storage = new BookmarkStorage(jClient);
	m_bookmark_storage->registerBookmarkHandler(this);
	m_bookmark_menu = 0;
	clearRecentBookmarks();
	m_pubsub_manager = new PubSub::Manager(jClient);
	m_pubsub_manager->registerEventHandler(this);

	jClient->registerStanzaExtension(new VCardUpdate());
	jClient->registerStanzaExtension(new MoodsExtenstion(0));
	jClient->registerStanzaExtension(new VersionExtension());
	jClient->registerStanzaExtension(new XStatusExtension());
	jClient->registerStanzaExtension(new Receipt(Receipt::Request));
	jClient->registerStanzaExtension(new ChatState(ChatStateActive));
	jClient->registerStanzaExtension(new DelayedDelivery(new Tag("")));
        jClient->registerStanzaExtension(new GMailExtension((Tag *)0));
        jClient->addPresenceExtension(new VCardUpdate(toStd(m_avatar_hash)));
	m_last_gmail_date = 0;
	m_pep_support = false;
	m_ping_timer.setInterval(60000);
	connect(&m_ping_timer, SIGNAL(timeout()),this,SLOT(onTimer()));
	m_server_ping=0;
	m_jabber_roster->setRosterManager(jClient->rosterManager());
}

void jProtocol::loadSettings()
{
	QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");

	account_settings.beginGroup("main");
	m_resource = account_settings.value("resource", "qutIM").toString();
	m_priority = account_settings.value("priority", 5).toInt();
	m_avatar_hash = account_settings.value("avatarhash","").toString();
	QString password = account_settings.value("password","").toString();
        m_my_nick = account_settings.value("nickname",m_account_name).toString();
        account_settings.endGroup();

        jClient->setResource(toStd(m_resource));
        jClient->setPassword(toStd(password));
        jClient->setPresence(jClient->presence().presence(), m_priority, jClient->presence().status());

        QSettings profile_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
        m_auto_retrieve_vcards = profile_settings.value("main/getavatars",true).toBool();
        m_gmail_notify_type = profile_settings.value("xeps/gmailtype",0).toInt();
}

jProtocol::~jProtocol()
{
	if (m_connection->state()!=StateDisconnected)
		jClient->disconnect();

	delete m_jabber_roster;
	delete m_conference_management_object;
	delete m_pubsub_manager;
	delete jClient;
}

QString jProtocol::fromStd(const std::string &str)
{
	return QString::fromUtf8(str.c_str(), str.size());
}

std::string jProtocol::toStd(const QString &str)
{
	return str.toUtf8().constData();
}

QDateTime jProtocol::fromStamp(const QString &stamp)
{
	QDateTime datetime;
	if(stamp.contains('-'))
	{
		QString time = stamp;
		int num = time.indexOf("Z");
		if(num<0)
			num = time.indexOf("-");
		time.truncate(num);
		if(num==18)
			datetime = QDateTime::fromString(time,"yyyy-MM-ddThh:mm:ss");
		else
			datetime = QDateTime::fromString(time,"yyyy-MM-ddThh:mm:ss.zzz");
		if(num>18)
		{
			time = stamp.right(5);
			QTime delta = QTime::fromString(time,"hh:mm");
			int multi=1;
			if(stamp.at(stamp.length()-6)=='+')
				multi = -1;
			datetime = datetime.addSecs(multi*(delta.minute()*60+delta.hour()*3600));
		}
	}
	else // Deprecated format of stamp
		datetime = QDateTime::fromString(stamp,"yyyyMMddThh:mm:ss");
	datetime.setTimeSpec(Qt::UTC);
	return datetime.toLocalTime();
}

QString jProtocol::getBare(const QString &full)
{
	return full.section('/',0,0);
}

QString jProtocol::getResource(const QString &full)
{
	return full.section('/',1);
}

void jProtocol::setStatus(Presence::PresenceType presence)
{
	QString message;
	QString sname = m_jabber_account->getStatusName(presence);
	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	if (!settings.value("autoreply/" + sname + "dshow", false).toBool())
	{
		bool dshow = false;
		message="";
		if (m_jabber_account->getPluginSystem().setStatusMessage(message, dshow))
		{
			settings.setValue("autoreply/" + sname + "dshow", dshow);
			if(dshow)
				settings.setValue("autoreply/" + sname + "msg", message);
		}
	}
	else
		message = settings.value("autoreply/" + sname + "msg", "").toString();
	if (message.isEmpty())
	{
		jClient->presence().resetStatus();
		jClient->setPresence(presence, m_priority);
	}
	else
		jClient->setPresence(presence, m_priority, toStd(message));
        if (presence != Presence::Unavailable)
        {
                if (m_connection->state()==StateDisconnected)
                {
                        start();
                        emit setRealStatus(-1);
                }
                else
                        emit setRealStatus(presence);
        }
	if (presence == Presence::Unavailable)
	{
		emit setRealStatus(presence);
		m_conference_management_object->disconnectAll();
		jClient->disconnect();
	}
}

void jProtocol::run()
{
	jClient->connect(false);
}

void jProtocol::onConnect()
{
	TreeModelItem item;
	item.m_protocol_name = "Jabber";
	item.m_account_name = m_account_name;
	item.m_item_type = TreeModelItem::Account;
	m_jabber_account->getPluginSystem().setAccountIsOnline(item, true);
	emit setRealStatus(m_jabber_account->getPresence());
	m_jids_auto_vcard.append(m_account_name);
	fetchVCard(m_account_name);
	m_conference_management_object->connectAll();
	m_bookmark_storage->requestBookmarks();
	JID server(jClient->jid().server());
	jClient->disco()->getDiscoInfo(server,"",this,100);
}

void jProtocol::requestBookmarks()
{
	m_bookmark_storage->requestBookmarks();
}

void jProtocol::onDisconnect(ConnectionError e)
{
	m_ping_timer.stop();
	TreeModelItem item;
	item.m_protocol_name = "Jabber";
	item.m_account_name = m_account_name;
	item.m_item_type = TreeModelItem::Account;
	if(e != ConnUserDisconnected && e != ConnNotConnected)
	{
		QString error_tr;
		switch(e)
		{
			case ConnStreamError:
				error_tr = tr("A stream error occured. The stream has been closed.");
				break;
			case ConnStreamVersionError:
				error_tr = tr("The incoming stream's version is not supported");
				break;
			case ConnStreamClosed:
				error_tr = tr("The stream has been closed (by the server).");
				break;
			case ConnProxyAuthRequired:
				error_tr = tr("The HTTP/SOCKS5 proxy requires authentication.");
				break;
			case ConnProxyAuthFailed:
				error_tr = tr("HTTP/SOCKS5 proxy authentication failed.");
				break;
			case ConnProxyNoSupportedAuth:
				error_tr = tr("The HTTP/SOCKS5 proxy requires an unsupported auth mechanism.");
				break;
			case ConnIoError:
				error_tr = tr("An I/O error occured.");
				break;
			case ConnParseError:
				error_tr = tr("An XML parse error occurred.");
				break;
			case ConnConnectionRefused:
				error_tr = tr("The connection was refused by the server (on the socket level).");
				break;
			case ConnDnsError:
				error_tr = tr("Resolving the server's hostname failed.");
				break;
			case ConnOutOfMemory:
				error_tr = tr("Out of memory. Uhoh.");
				break;
			case ConnNoSupportedAuth:
				error_tr = tr("The auth mechanisms the server offers are not supported or the server offered no auth mechanisms at all.");
				break;
			case ConnTlsFailed:
				error_tr = tr("The server's certificate could not be verified or the TLS handshake did not complete successfully.");
				break;
			case ConnTlsNotAvailable:
				error_tr = tr("The server didn't offer TLS while it was set to be required or TLS was not compiled in.");
				break;
			case ConnCompressionFailed:
				error_tr = tr("Negotiating/initializing compression failed.");
				break;
			case ConnAuthenticationFailed:
				error_tr = tr("Authentication failed. Username/password wrong or account does not exist. Use ClientBase::authError() to find the reason.");
				break;
			case ConnUserDisconnected:
				error_tr = tr("The user (or higher-level protocol) requested a disconnect.");
				break;
			case ConnNotConnected:
				error_tr = tr("There is no active connection.");
				break;
			default:
				error_tr = tr("Unknown error. It is amazing that you see it... O_o");
				break;
		}
		emit systemNotification(m_account_name,error_tr);
	}
	m_jabber_account->getPluginSystem().setAccountIsOnline(item, false);
	m_jabber_roster->setOffline();
	emit setRealStatus(Presence::Unavailable);
//	m_gloox_timer->stop();
	m_conference_management_object->disconnectAll();
        jClient->setCompressionImpl(0);
        jClient->setEncryptionImpl(0);
}

bool jProtocol::onTLSConnect(const CertInfo& info)
{
	return true;
}

void jProtocol::onResourceBindError(ResourceBindError error)
{
}

void jProtocol::onSessionCreateError(SessionCreateError error)
{
}

void jProtocol::handleItemSubscribed(const JID& jid)
{
}

void jProtocol::handleItemAdded(const JID& jid)
{
	RosterItem *item = jClient->rosterManager()->getRosterItem(jid);
	QString name = fromStd(item->name());
	QString group;
	StringList g = item->groups();
	StringList::const_iterator it_g = g.begin();
	for(; it_g != g.end(); ++it_g)
		group = fromStd(*it_g);
	if (group.isEmpty())
		group = "General";
	if ( !fromStd(jid.bare()).contains("@") )
		group = tr("Services");
	m_jabber_roster->addContact(fromStd(jid.bare()),name,group);
}

void jProtocol::handleItemUnsubscribed(const JID& jid)
{
}

void jProtocol::handleItemRemoved(const JID& jid)
{
	m_jabber_roster->removeContact(fromStd(jid.bare()));
}

void jProtocol::handleItemUpdated(const JID& jid)
{
	RosterItem *item = jClient->rosterManager()->getRosterItem(jid);
	QString bare = fromStd(jid.bare());
	jBuddy *buddy = m_jabber_roster->getBuddy(bare);
	if(!item || !buddy)
		return;
	QString name = fromStd(item->name());
	QString group;
	StringList g = item->groups();
	StringList::const_iterator it_g = g.begin();
	for(; it_g != g.end(); ++it_g)
		group = fromStd(*it_g);
	qDebug() << bare << buddy->getGroup() << group;
	if (group.isEmpty())
		group = "General";
	if ( !fromStd(jid.bare()).contains("@") )
		group = tr("Services");
	if(buddy->getName()!=name)
		m_jabber_roster->renameContact(bare,name);
	if(buddy->getGroup()!=group)
		m_jabber_roster->moveContact(bare,group);
}

void jProtocol::handleRoster(const Roster& roster)
{
	m_jabber_roster->startLoadRoster();
	Roster::const_iterator it = roster.begin();
	for(; it != roster.end(); ++it)
	{
		QString group;
		StringList g = (*it).second->groups();
		StringList::const_iterator it_g = g.begin();
		for(; it_g != g.end(); ++it_g)
			group = fromStd(*it_g);
		if (group.isEmpty())
			group = "General";
		if ( !fromStd((*it).second->jid()).contains("@") )
			group = tr("Services");
		if (!m_jabber_roster->groupExist(group))
                        m_jabber_roster->addGroup(group);
                jBuddy *buddy = m_jabber_roster->addContact(getBare(fromStd((*it).second->jid())), fromStd((*it).second->name()), group);
                buddy->setSubscription((*it).second->subscription());
	}
	m_jabber_roster->stopLoadRoster();
}

void jProtocol::handleRosterError(Stanza *stanza) {
    //
}

void jProtocol::handlePresence (const Presence &presence)
{
        QString jid = fromStd(presence.from().bare()).toLower();
	QString resource = fromStd(presence.from().resource());
        Presence::PresenceType presence_type = presence.presence();
        if(presence_type == Presence::Invalid)
            presence_type = Presence::Available;

	if(jid == m_account_name)
	{
                if (presence_type != Presence::Unavailable)
		{
			bool exist = m_jabber_roster->myConnectExist(resource);
                        m_jabber_roster->addMyConnect(resource, presence.priority(), presence_type);
                        m_jabber_roster->changeItemStatus(jid+"/"+resource, presence_type);
			if(!exist)
			{
				const Capabilities *caps = presence.capabilities();
				jBuddy::ResourceInfo *info = m_jabber_roster->getBuddy(jid)->getResourceInfo(resource);
				if(caps)
				{
					info->m_caps_ver = fromStd(caps->ver());
					info->m_caps_node = fromStd(caps->node());
				}
				else
				{
					info->m_caps_ver = "";
					info->m_caps_node = "";
				}
				if(resource!=m_resource)
				{
					jClientIdentification::instance().setClient(info,presence.from(),jClient,this);
				}
				else
				{
					info->m_client_name = fromStd(jClient->disco()->name());
					info->m_client_version = fromStd(jClient->disco()->version());
					info->m_client_os = fromStd(jClient->disco()->os());
				}
				m_jabber_roster->setClient(jid, resource, info->m_client_name,true);
			}
		}
		else
			m_jabber_roster->delMyConnect(resource);
	}
	else
	{
		if(!m_jabber_roster->contactExist(jid))
			return;

                if (presence_type == Presence::Unavailable)
			m_jabber_roster->delResource(jid, resource);
		else
		{
                        m_jabber_roster->addResource(jid, resource, presence.priority(), presence_type);
			const Capabilities *caps = presence.capabilities();
			jBuddy::ResourceInfo *info = m_jabber_roster->getBuddy(jid)->getResourceInfo(resource);
			if(caps)
			{
				info->m_caps_ver = fromStd(caps->ver());
				info->m_caps_node = fromStd(caps->node());
			}
			else
			{
				info->m_caps_ver = "";
				info->m_caps_node = "";
			}
			if(info->m_client_name=="")
			{
				jClientIdentification::instance().setClient(info,presence.from(),jClient,this);
				m_jabber_roster->setClient(jid, resource, info->m_client_name);
			}
			m_jabber_roster->setStatusMessage(jid, resource, fromStd(presence.status()));

			if (m_jabber_roster->getMaxPriorityResource(jid) == resource)
                                m_jabber_roster->changeItemStatus(jid, presence_type);

                        m_jabber_roster->changeItemStatus(jid+"/"+resource, presence_type);
		}
	}
        if(presence_type != Presence::Unavailable)
	{
		const VCardUpdate *vcard_update = presence.findExtension<VCardUpdate>(ExtVCardUpdate);
		if(vcard_update)
		{
			QString hash = fromStd(vcard_update->hash());
			jBuddy *buddy = m_jabber_roster->getBuddy(jid);
                        if(buddy && buddy->getAvatarHash()!=hash)
                        {
                                if(hash.isEmpty() || QFile(m_jabber_account->getPathToAvatars()+"/"+hash).exists())
				{
					m_jabber_roster->setAvatar(jid,hash);
				}
				else
				{
                                    if(m_auto_retrieve_vcards)
                                    {
					m_jids_auto_vcard.append(jid);
					fetchVCard(jid);
                                    }
				}
			}
		}
		const XStatusExtension *xstatus = presence.findExtension<XStatusExtension>(ExtUser+1);
		if(xstatus)
		{
			jBuddy *buddy = m_jabber_roster->getBuddy(jid);
			if(buddy)
			{
				QString icon = QString("icq_xstatus%1").arg(xstatus->status_id());
				jBuddy::ResourceInfo *info = buddy->getResourceInfo(resource);
				if(info)
				{
					if (xstatus->status_id()>=0)
						info->m_xstatus.m_icon = icon;
					info->m_xstatus.m_position = 5;
                                        if(buddy->getMaxPriorityResource()==resource)
                                            m_jabber_roster->updateIcon(jid, "xstatus");
				}
				if(buddy->getMaxPriorityResource()==resource)
				{
					TreeModelItem contact;
					contact.m_protocol_name = "Jabber";
					contact.m_account_name = m_account_name;
					contact.m_item_name = jid;
					contact.m_parent_name = buddy->getGroup();
					contact.m_item_type = TreeModelItem::Buddy;
					emit setClientCustomIcon(contact, icon, 5);
				}
			}
		}
	}
}

void jProtocol::handleSubscription (const Subscription &s10n)
{
        QString jid = fromStd(s10n.from().bare()).toLower();
        jBuddy *buddy = m_jabber_roster->getBuddy(jid);
        if(!buddy)
        {
                const Nickname *nickname = s10n.findExtension<Nickname>(ExtNickname);
                buddy = m_jabber_roster->addContact(jid,nickname?fromStd(nickname->nick()):"","");
        }
        RosterItem *item = jClient->rosterManager()->getRosterItem(s10n.from().bareJID());
        if(item)
            buddy->setSubscription(item->subscription());
        QString text;
	switch( s10n.subtype() )
	{
		case Subscription::Subscribe:
			emit createAcceptAuthDialog(fromStd(s10n.status()),s10n.from(),jClient);
                        text = tr("Authorization request");
			break;
		case Subscription::Subscribed:
                        text = tr("You were authorized");
			break;
                case Subscription::Unsubscribe:
                        text = tr("Contacts's authorization was removed");
                        break;
                case Subscription::Unsubscribed:
                        text = tr("Your authorization was removed");
                        break;
		default:
                        return;
        }
        if(!s10n.status().empty())
            text+="\n"+fromStd(s10n.status());
        TreeModelItem contact;
        contact.m_protocol_name = "Jabber";
        contact.m_account_name = m_account_name;
        contact.m_item_type = TreeModelItem::Buddy;
        contact.m_parent_name = buddy->getGroup();
        contact.m_item_name = jid;
        emit addMessageFrom(contact, QDateTime::currentDateTime(), text);
}

void jProtocol::handleRosterPresence(const RosterItem&/*item*/, const std::string&/*resource*/, Presence::PresenceType /*presence*/, const std::string &/*msg*/)
{
}

void jProtocol::handleSelfPresence(const RosterItem&/*item*/, const std::string& resource, Presence::PresenceType presence, const std::string &/*msg*/)
{
	if (fromStd(resource) == m_resource)
		emit setRealStatus(presence);
}

void jProtocol::handleNonrosterPresence(const gloox::Presence&)
{
}

void jProtocol::handleRosterError(const gloox::IQ&)
{
}

bool jProtocol::handleSubscriptionRequest(const JID& jid, const std::string &msg)
{
	return false;
}

bool jProtocol::handleUnsubscriptionRequest(const JID& jid, const std::string &msg)
{
	return false;
}

void jProtocol::handleNonrosterPresence(Stanza* stanza)
{
}

void jProtocol::handleMessage(const Message &stanza, MessageSession * /*session*/)
{
	addMessageFrom(stanza.from(), stanza);
}

void jProtocol::addMessageFrom(const JID &from, const Message &stanza)
{
        QString jid = fromStd(from.bare()).toLower();
        QString resource = fromStd(from.resource());
        QString full = jid + (resource.isEmpty() ? "" : ("/"+resource));
        jBuddy *o_contact = m_jabber_roster->getBuddy(jid);
	if(!o_contact)
	{
                const Nickname *nickname = stanza.findExtension<Nickname>(ExtNickname);
                m_jabber_roster->addContact(jid,nickname?fromStd(nickname->nick()):"","");
                o_contact = m_jabber_roster->getBuddy(jid);
        }
	/*const MoodsExtenstion *mood = stanza.findExtension<MoodsExtenstion>(ExtUser+107);
	if(mood)
	{
		TreeModelItem contact;
                //		if(!m_conference_management_object->JIDIsRoom(jid))
		//		{
			if(!o_contact)
				return;
			contact.m_protocol_name = "Jabber";
			contact.m_account_name = m_account_name;
                        contact.m_item_name = jid;//full());
			contact.m_parent_name = o_contact->getGroup();
			contact.m_item_type = 0;
			emit setClientCustomIcon(contact, ":/icons/moods/" + mood->mood_name(), 3);
			o_contact->m_mood = mood->mood_text();
			o_contact->m_mood_name = mood->mood_name();
			//		}
	}*/

	const Receipt *receipt = stanza.findExtension<Receipt>(ExtReceipt);
        if(receipt && o_contact->resourceExist(resource))
	{
		Receipt::ReceiptType receipt_type = receipt->rcpt();//(ReceiptType)(receipt->rcpt()-1);
		if(receipt_type==Receipt::Request)
		{
			Message msg(Message::Chat, from);
			Receipt *receipt = new Receipt(Receipt::Received);
                        msg.addExtension(receipt);
			msg.setID(stanza.id());
                        jClient->send(msg);
		}
		else if(receipt_type==Receipt::Received)
		{
			QPair<QString,int> pair = m_receipts_hash.value(fromStd(stanza.id()));
			m_receipts_hash.remove(fromStd(stanza.id()));
			TreeModelItem contact;
			contact.m_protocol_name = "Jabber";
			contact.m_account_name = m_account_name;
			contact.m_item_name = pair.first;
			contact.m_item_type = TreeModelItem::Buddy;
                        emit messageDelievered(contact, pair.second);
                }
	}

	const ChatState *chat_state = stanza.findExtension<ChatState>(ExtChatState);
	if(chat_state)
	{
		ChatStateType state = chat_state->state();
		TreeModelItem contact;
		contact.m_protocol_name = "Jabber";
		contact.m_account_name = m_account_name;
		contact.m_parent_name = "";
                contact.m_item_name = full;
		contact.m_item_type = TreeModelItem::Buddy;

		bool composing = state==ChatStateComposing;
		if(composing)
			m_jabber_roster->chatWindowAboutToBeOpened(fromStd(from.full()));
//		bool compose = true;
                if(jid==m_account_name)
			contact.m_parent_name = "My connections";
		else if(o_contact)
		{
//			QString resource = fromStd(from.resource());
//			if (o_contact->getMaxPriorityResource() == resource)
//			{
//				contact.m_item_name = jid;
//				if((o_contact->m_chat_state==ChatStateComposing)==composing)
//					compose = false;
//				o_contact->m_chat_state = state;
//			}
//			else
//			{
//				jBuddy::ResourceInfo *info = o_contact->getResourceInfo(resource);
//				if(info)
//				{
//					if((info->m_chat_state==ChatStateComposing)==composing)
//					compose = false;
//					info->m_chat_state = state;
//		}
//		}
			contact.m_parent_name = o_contact->getGroup();
		}
//		else
//		{
//			if(m_composing_hash.value(contact.m_item_name,false)==composing)
//			compose = false;
//			else
//				m_composing_hash.insert(contact.m_item_name,composing);
//	}
//        if(compose)
                emit sendTypingNotification(contact, composing);
	}

	if(!stanza.body().empty())
	{
		TreeModelItem contact;
		contact.m_protocol_name = "Jabber";
		contact.m_account_name = m_account_name;
		contact.m_item_type = TreeModelItem::Buddy;
                contact.m_item_name = jid;

                QString from_string = full;
                if(contact.m_item_name==m_account_name)
			contact.m_parent_name = "My connections";
		else if(o_contact)
		{
//			QString resource = fromStd(from.resource());
//			if (o_contact->getMaxPriorityResource() == resource)
//				from_string = jid;
			contact.m_parent_name = o_contact->getGroup();
		}
		QDateTime date;
		const DelayedDelivery * delayed_delievery = stanza.when();
		if(delayed_delievery)
		{
			date = fromStamp(fromStd(delayed_delievery->stamp()));
//			from_string = jid;
		}
		else
			date = QDateTime::currentDateTime();
                jBuddy::ResourceInfo *info = o_contact->getResourceInfo(resource);
                m_jabber_roster->chatWindowAboutToBeOpened(full);
                if(info && info->m_in_cl)
                    jPluginSystem::instance().changeChatWindowID(contact, from_string);
                contact.m_item_name = from_string;
		emit addMessageFrom(contact, date, fromStd(stanza.body()));
	}
}

void jProtocol::sendMessageTo(const QString &contact_name, const QString& message, int message_icon_position)
{
        Message msg(Message::Chat, JID(toStd(contact_name)), toStd(message));
        QString bare = getBare(contact_name);
        QString resource = getResource(contact_name);
        jBuddy *buddy = m_jabber_roster->getBuddy(bare);
        if(buddy && buddy->getSubscription()!=S10nBoth)
        {
            Nickname *nick = new Nickname(toStd(m_my_nick));
            msg.addExtension(nick);
        }
	std::string id = jClient->getID();
	msg.setID(id);
	QPair<QString,int> pair(contact_name,message_icon_position);
	m_receipts_hash.insert(fromStd(id),pair);
	Receipt *receipt = new Receipt(Receipt::Request);
	msg.addExtension(receipt);
	jClient->send(msg);
}

void jProtocol::sendTypingNotification(const QString &item_name, int notification_type)
{
        jBuddy *buddy = m_jabber_roster->getBuddy(getBare(item_name));
        if(!buddy)
            return;
        if(!buddy->resourceExist(getResource(item_name)))
            return;
	ChatStateType state;
	if(notification_type==2)
		state = ChatStateComposing;
	else if(notification_type==0)
		state = ChatStateActive;
	else
		return;
	Message msg(Message::Chat, toStd(item_name));
	ChatState *chat_state = new ChatState(state);
	msg.addExtension(chat_state);
	jClient->send(msg);
}

bool jProtocol::handleIq (const IQ &iq)
{
	const VersionExtension *version = iq.findExtension<VersionExtension>(ExtVersion);
	if(version)
	{
		if(version->name().isNull())
			return true;
		if(m_conference_management_object->JIDIsRoom(fromStd(iq.from().bare())))
		{
			QString conference = fromStd(iq.from().bare());
			//			QString full = fromStd(iq.from().full());
			QString nick = fromStd(iq.from().resource());
			//			qDebug() << fromStd(iq.from().full()) << version->name();
			//			emit conferenceClientVersion("Jabber",fromStd(iq.from().bare()),m_account_name,fromStd(iq.from().resource()), version->name());
			m_conference_management_object->setClient(conference,nick,version);
		}
		else
		{
			if(version->name().isEmpty())
				return true;
			QString bare = fromStd(iq.from().bare());
			//			QString full = fromStd(iq.from().full());
			QString resource = fromStd(iq.from().resource());
			if(!m_jabber_roster->contactExist(bare))
				return true;
			jBuddy *o_contact = m_jabber_roster->getBuddy(bare);
			jBuddy::ResourceInfo *info = o_contact->getResourceInfo(resource);
			jClientIdentification::instance().newInfo(version, info);

			m_jabber_roster->setClient(bare, resource, version->name(),bare==m_account_name);
		}
		return true;
	}
	const GMailExtension *gmail = iq.findExtension<GMailExtension>(ExtUser+2);
        if(gmail)
	{
            if(m_gmail_notify_type<0)
                return true;
		if(gmail->isNotification())
		{
			IQ iq(IQ::Get, JID(toStd(m_account_name)), jClient->getID());
			iq.addExtension(new GMailExtension(m_last_gmail_date));
			jClient->send(iq);
		}
		else
		{
			if(gmail->getMailThreads().size()==0)
				return true;
			m_last_gmail_date = gmail->getResultTime();
			QString message;
                        int num = 0;
			foreach(GMailExtension::MailThread mail, gmail->getMailThreads())
			{
                            num++;
				if(mail.senders.size()==1)
					message.append((tr("Sender: %2 <%1>")+"\n").arg(mail.senders.at(0).address).arg(mail.senders.at(0).name));
				else
				{
					message.append(tr("Senders: "));
					foreach(GMailExtension::Sender sender, mail.senders)
					{
						message.append((tr("%2 <%1>")+"\n").arg(sender.address).arg(sender.name));
					}
				}
				message.append(tr("Subject: %1").arg(mail.subject)+"\n");
				message.append(tr("URL: %1").arg(mail.url)+"\n\n");
			}
                        if(!num)
                            return true;
			if(!message.isEmpty())
				message.append("------\n");
			message.append(tr("Unreaded messages: %1").arg(gmail->getTotalMatched()));
			TreeModelItem item;
			item.m_protocol_name = "Jabber";
			item.m_account_name = item.m_item_name = m_account_name;

                        if(m_gmail_notify_type==0)
                        {
                            item.m_item_type = TreeModelItem::Buddy;
                            emit addMessageFrom(item, QDateTime::currentDateTime(), message);
                        }
                        else if(m_gmail_notify_type==1)
                        {
                            item.m_item_type = TreeModelItem::Buddy;
                            jPluginSystem::instance().customNotifiacation(item,message);
                        }
		}
		return true;
	}
	return false;
}

void jProtocol::handleIqID (const IQ &/*iq*/, int /*context*/)
{

}

void jProtocol::getDiscoItems(const QString &jid, const QString &node, DiscoHandler *handler)
{
	/*int context;
	if (tid == "request-conference")
		context = 1000;
	else if (tid == "request-conference-item")
		context = 1001;*/
	DiscoHandler *dh;
	if (handler)
		dh = handler;
	else
		dh = this;
	jClient->disco()->getDiscoItems(JID(toStd(jid)), toStd(node), dh, 0);
}

void jProtocol::getDiscoInfo(const QString &jid, const QString &node, DiscoHandler *handler)
{
	/*int context;
	if (tid == "request-conference")
		context = 1000;
	else if (tid == "request-conference-item")
		context = 1001;*/
	DiscoHandler *dh;
	if (handler)
		dh = handler;
	else
		dh = this;
	jClient->disco()->getDiscoInfo(JID(toStd(jid)), toStd(node), dh, 0);
}

void jProtocol::handleDiscoInfo(const JID &from, const Disco::Info &info, int context)
{
	if(context==100)
	{
		m_pep_support = false;
		const Disco::IdentityList &identities = info.identities();
		Disco::IdentityList::const_iterator ilt = identities.begin();
		for( ; ilt != identities.end(); ++ilt )
		{
			if((*ilt)->category()=="pubsub" && (*ilt)->type()=="pep")
				m_pep_support = true;
		}
		m_mood_action->setEnabled(m_pep_support);
		m_activity_action->setEnabled(m_pep_support);
		m_server_features.clear();
		const StringList &sl = info.features();
		StringList::const_iterator it = sl.begin();
		for( ; it != sl.end(); ++it )
			m_server_features << fromStd((*it));
		qSort(m_server_features);
		m_last_gmail_date = 0;
                if(hasFeature("google:mail:notify") && m_gmail_notify_type>-1)
		{
			IQ iq(IQ::Get, JID(toStd(m_account_name)), jClient->getID());
			iq.addExtension(new GMailExtension(m_last_gmail_date));
			jClient->send(iq);
                }
                m_ping_timer.start();
	}
	/*else if (context == 1000 || context == 1001)
	{
		QString type;
		if (context == 1000)
			type = "request-conference";
		else if (context == 1001)
			type = "request-conference-item";
		emit receiveSDiscoInfo(from, info, type);
	}*/
	else
	{
                QString bare = fromStd(from.bare()).toLower();
		QString resource = fromStd(from.resource());
		if(m_jabber_roster->contactExist(bare))
		{
			jBuddy *o_contact = m_jabber_roster->getBuddy(bare);
			jBuddy::ResourceInfo *resource_info = o_contact->getResourceInfo(resource);
			jClientIdentification::instance().newInfo(info,resource_info);
		}
	}
}

void jProtocol::handleDiscoItems(const JID &from, const Disco::Items &items, int context)
{
	/*QString type;
	if (context == 1000)
		type = "request-conference";
	else if (context == 1001)
		type = "request-conference-item";
	emit receiveSDisco(from, items, type);*/
}

void jProtocol::handleDiscoError(const JID &/*from*/, const Error * /*error*/, int /*context*/)
{
}

bool jProtocol::handleDiscoSet(const IQ &iq)
{
	return false;
}

QStringList jProtocol::getAdditionalInfoAboutContact(const QString &item_name)
{
	QString node;
	QString resource;
	if(item_name.indexOf("/")>-1)
	{
		resource = item_name;
		resource.remove(QRegExp(".*/"));
		node = item_name;
		node.remove(QRegExp("/.*"));
	}
	else
		node = item_name;
	if(m_conference_management_object->JIDIsRoom(node))
	{
		return m_conference_management_object->getAdditionalInfoAboutContact(node,resource);
	}
	else
	{
		jBuddy *buddy = m_jabber_roster->getBuddy(node);
		if(!buddy)
			return QStringList();
		if(resource.isEmpty() || resource.isNull())
		{
			resource = buddy->getMaxPriorityResource();
		}
		QStringList list_info;
		list_info << buddy->getName();
		QString hash = buddy->getAvatarHash();
		if(hash.isEmpty())
			list_info << QString();
		else
			list_info << m_jabber_account->getPathToAvatars()+"/"+buddy->getAvatarHash();
		jBuddy::ResourceInfo * info = buddy->getResourceInfo(resource);
		if(!info)
			return list_info;
		list_info << (info->m_client_name + " " + info->m_client_version + (info->m_client_os.isEmpty()?"":(" / " + info->m_client_os)));
		list_info << resource;
		return list_info;
	}
	return QStringList();
}

void jProtocol::fetchVCard(const QString &jid, bool is_auto)
{
	if(is_auto)
		m_jids_auto_vcard.append(jid);
	const JID &jid_tmp = JID(toStd(jid));
	m_vcardManager->fetchVCard(JID(toStd(jid)), this);
}

void jProtocol::handleVCard(const JID &jid, const VCard *fetchedVCard)
{
}

void jProtocol::updateAvatarPresence(const QString &hash)
{
/*
	if(m_avatar_hash!=hash)
	{
		if(m_avatar_hash.isEmpty() && hash.isEmpty())
			return;
		m_avatar_hash=hash;
		StanzaExtensionList *extensions = (StanzaExtensionList *)&(jClient->presence().extensions());
		StanzaExtensionList::iterator it = extensions->begin();
		StanzaExtensionList::iterator it2;
		while( it != extensions->end() )
		{
			it2 = it++;
			if((*it2)->extensionType()==ExtVCardUpdate)
			{
				delete (*it2);
				extensions->erase(it2);
			}
                }
                jClient->addPresenceExtension(new VCardUpdate(toStd(m_avatar_hash)));
		QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
		account_settings.setValue("main/avatarhash",hash);
		m_conference_management_object->sendPresenceToAll();
		jClient->setPresence();
	}
*/
}

void jProtocol::handleVCardResult(VCardContext context, const JID &jid, StanzaError se)
{
	if (context == StoreVCard && se == StanzaErrorUndefined)
		emit systemNotification(m_account_name, tr("vCard is succesfully saved"));
}

void jProtocol::storeVCard(VCard *vcard)
{
	m_vcardManager->storeVCard(vcard, this);
}

void jProtocol::setAvatar(const JID &jid, const QString &hash)
{
	QString node = fromStd(jid.bare());
	if(m_conference_management_object->JIDIsRoom(node))
	{
		QString resource = fromStd(jid.resource());
		if (hash.isEmpty())
			m_conference_management_object->setAvatar(node,resource,"");
		else
			m_conference_management_object->setAvatar(node,resource,m_jabber_account->getPathToAvatars()+"/"+hash);
	}
	else
	{
		m_jabber_roster->setAvatar(node,hash);
	}
}

void jProtocol::setResource(const QString &resource, int priority)
{
	m_resource = resource;
	m_priority = priority;
	QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
	account_settings.setValue("main/resource", m_resource);
	account_settings.setValue("main/priority", m_priority);

	jClient->setResource(toStd(m_resource));
	jClient->setPresence(jClient->presence().presence(), m_priority, jClient->presence().status());

	emit systemNotification(m_account_name, tr("The resource will be changed during the next login"));
}

void jProtocol::moveContact(const QString &item_name, const QString &parent_name)
{
	RosterItem *item = jClient->rosterManager()->getRosterItem(JID(toStd(item_name)));
	if(!item)
		return;
	if(item_name.indexOf('/')>-1)
		return;
	QString group = parent_name;
	if(group.isEmpty())
		return;
	if(group=="General")
		group="";
	StringList groups;
	groups.push_back(toStd(group));
	item->setGroups(groups);
	jClient->rosterManager()->synchronize();
}

void jProtocol::addContact(const QString &jid, QString name, const QString &group, bool authorize)
{
	if (jid.isEmpty())
		return;
	JID to = JID(toStd(jid));
	if (name.isEmpty())
		name = jid;
	StringList groups;
	groups.push_back(toStd(group));
	if (authorize)
		jClient->rosterManager()->subscribe(to, toStd(name), groups, toStd("Please authorize me and add to your contacr list"));
	else
		jClient->rosterManager()->add(to, toStd(name), groups);
	jClient->rosterManager()->synchronize();
}

void jProtocol::handleBookmarks(const BookmarkList &b_list, const ConferenceList &c_list)
{
	setRecentBookmarks(b_list, c_list);
	emit bookmarksHandled(b_list,c_list);
}


void jProtocol::handleLog (LogLevel, LogArea area, const std::string &message)
{
	bool in;
	if(area == LogAreaXmlIncoming)
		in = true;
	else if(area == LogAreaXmlOutgoing)
		in = false;
	else
	{
		qDebug() << fromStd(message);
		return;
	}
	QString xml = fromStd(message);
	emit tagHandled(xml, in);
}

void jProtocol::handleItemPublication (const JID &service, const std::string &node, const std::string &item, const Tag *entry)
{
	if(!entry)
		return;
	QString jid = fromStd(service.bare());
	jBuddy *buddy = m_jabber_roster->getBuddy(jid);
	if(!buddy)
		return;
	if( entry->name()=="mood" && entry->xmlns()=="http://jabber.org/protocol/mood" )
	{
		jPluginSystem& jps = jPluginSystem::instance();
		jBuddy::SimpleInfo info;
		info.m_position = 6;
		MoodsExtenstion mood(entry);
		if(!mood.moodName().isEmpty())
		{
			QIcon icon = jps.getIcon("moods/"+mood.moodName());
			if(icon.actualSize(QSize(16,16)).width()<0)
			{
				icon = QIcon(":/icons/moods/"+mood.moodName());
				info.m_icon = ":/icons/moods/"+mood.moodName();
			}
			else
				info.m_icon = "moods/"+mood.moodName();
			if(mood.moodText().isEmpty())
				info.m_html = QObject::tr("<font size='2'><i>%1</i></font>").arg(jps.getMoodTr().value(mood.moodName()));
			else
				info.m_html = QObject::tr("<font size='2'><i>%1:</i> %2</font>").arg(jps.getMoodTr().value(mood.moodName())).arg(mood.moodText());
		}
		buddy->setSimpleInfo("mood",info);
		m_jabber_roster->updateIcon(jid, "mood");
		if(jid==m_account_name)
		{
			m_current_mood = mood.moodName();
			QIcon icon;
			if(!m_current_mood.isEmpty())
			{
				icon = jPluginSystem::instance().getIcon("moods/"+m_current_mood);
				if(icon.actualSize(QSize(16,16)).width()<0)
					icon = QIcon(":/icons/moods/"+m_current_mood);
			}
			m_mood_action->setIcon(icon);
		}
	}
	else if( entry->name()=="activity" && entry->xmlns()=="http://jabber.org/protocol/activity" )
	{
                jPluginSystem& jps = jPluginSystem::instance();
                ActivityExtension activity(entry);
		jBuddy::SimpleInfo info;
                jBuddy::SimpleInfo info_spec;
                if(!activity.general().isEmpty())
		{
			QString activity_tr = jps.getGeneralActivityTr().value(activity.general());
			if(!activity.specific().isEmpty())
				activity_tr+=" ("+jps.getSpecificActivityTr(activity.general()).value(activity.specific())+")";
			if(activity.text().isEmpty())
				info.m_html = QObject::tr("<font size='2'><i>%1</i></font>").arg(activity_tr);
			else
				info.m_html = QObject::tr("<font size='2'><i>%1:</i> %2</font>").arg(activity_tr).arg(activity.text());
			QIcon icon = jps.getIcon("activity/"+activity.general()+"/category");
                        info.m_icon = "activity/"+activity.general()+"/category";
			if(icon.actualSize(QSize(16,16)).width()<0)
                        {
                            icon = QIcon(":/icons/activity/"+activity.general()+"/category");
                            info.m_icon = ":/icons/activity/"+activity.general()+"/category";
                        }
                        if(!activity.specific().isEmpty())
                        {
				icon = jps.getIcon("activity/"+activity.general()+"/"+activity.specific());
                                info_spec.m_icon = "activity/"+activity.general()+"/"+activity.specific();
				if(icon.actualSize(QSize(16,16)).width()<0)
                                {
                                    info_spec.m_icon = ":/icons/activity/"+activity.general()+"/"+activity.specific();
                                    icon = QIcon(":/icons/activity/"+activity.general()+"/"+activity.specific());
                                }
			}
		}
		if(jid==m_account_name)
		{
			m_current_activity.first = activity.general();
			m_current_activity.second = activity.specific();
			QString icon_path = activity.general()+"/"+(activity.specific().isEmpty()?"category":activity.specific());
			QIcon icon;
			if(icon_path!="/category")
			{
				icon = jPluginSystem::instance().getIcon("activity/"+icon_path);
				if(icon.actualSize(QSize(16,16)).width()<0)
					icon = QIcon(":/icons/activity/"+icon_path);
			}
			m_activity_action->setIcon(icon);
                }
                info.m_position = 5;
                info_spec.m_position = 4;
                buddy->setSimpleInfo("activity",info);
                buddy->setSimpleInfo("activity_spec",info_spec);
                m_jabber_roster->updateIcon(jid, "activity_spec");
                m_jabber_roster->updateIcon(jid, "activity");
	}
	else if( entry->name()=="tune" && entry->xmlns()=="http://jabber.org/protocol/tune" )
        {
		jBuddy::SimpleInfo info;
		TuneExtension tune(entry);
		if(!tune.getArtist().isEmpty())
			info.m_html.append(tune.getArtist());
		if(!tune.getTitle().isEmpty())
		{
			if(!info.m_html.isEmpty())
				info.m_html.append(" - ");
			info.m_html.append(tune.getTitle()+" ");
                }
		if(!info.m_html.isEmpty())
                        info.m_html = tr("<font size='2'><i>Listening:</i> %1</font>").arg(info.m_html);
                info.m_icon = info.m_html.isEmpty()?"":"icq_xstatus10";
                info.m_position = 3;
		buddy->setSimpleInfo("tune",info);
                m_jabber_roster->updateIcon(jid, "tune");
	}
	else
		qDebug() << "It is unknown pep! Please notify developers." << fromStd(service.full()) << fromStd(entry->xml());
}

void jProtocol::handleItemRemoval (const JID &service, const std::string &node, const std::string &item)
{
}

void jProtocol::setActions(QAction *bookmark, QAction *mood, QAction *activity)
{
	m_bookmark_action = bookmark;
	m_mood_action = mood;
	connect(m_mood_action,SIGNAL(triggered()),this,SLOT(onSetMood()));
	m_activity_action = activity;
	m_activity_action->setEnabled(true);
	connect(m_activity_action,SIGNAL(triggered()),this,SLOT(onSetActivity()));
}

void jProtocol::onSetMood()
{
	CustomStatusDialog dialog(m_account_name,m_profile_name);
	dialog.setStatuses(m_current_mood);
	dialog.show();
	if(dialog.exec()==QDialog::Accepted)
	{
		MoodsExtenstion *mood = new MoodsExtenstion(dialog.current_mood, dialog.status_message);
		PubSub::ItemList items;
		Tag *tag = new Tag("item");
		tag->addChild(mood->tag());
		items.push_back(new  PubSub::Item(tag));
		m_pubsub_manager->publishItem	(JID(), "http://jabber.org/protocol/mood", items, 0, this);
		delete mood;
	}
}

void jProtocol::onSetActivity()
{
	ActivityDialog dialog(m_account_name,m_profile_name);
	dialog.setActivity(m_current_activity.first, m_current_activity.second);
	dialog.show();
	if(dialog.exec()==QDialog::Accepted)
	{
		ActivityExtension *activity = new ActivityExtension(dialog.getGeneral(), dialog.getSpecific(), dialog.getText());
		PubSub::ItemList items;
		Tag *tag = new Tag("item");
		tag->addChild(activity->tag());
		items.push_back(new  PubSub::Item(tag));
		m_pubsub_manager->publishItem	(JID(), "http://jabber.org/protocol/activity", items, 0, this);
		delete activity;
	}
}

void jProtocol::getMessageFromPlugins(const QList<void *> &event)
{
	switch(*(EventToJabber*)event[0])
	{
		case qutim_sdk_jabber::SetStatus:
			break;
		case qutim_sdk_jabber::RestoreStatus:
			break;
		case SetMood:{
			QString mood_name;
			QString mood_text;
			if(event.size()<3)
				mood_name = "";
			else
				mood_name = *(QString*)event[2];
			if(event.size()<4)
				mood_text = "";
			else
				mood_text = *(QString*)event[3];
			MoodsExtenstion *mood = new MoodsExtenstion(mood_name, mood_text);
			PubSub::ItemList items;
			Tag *tag = new Tag("item");
			tag->addChild(mood->tag());
			items.push_back(new  PubSub::Item(tag));
			m_pubsub_manager->publishItem	(JID(), "http://jabber.org/protocol/mood", items, 0, this);
			delete mood;
			break;
		}
		case SetActivity:{
			QString activity_general;
			QString activity_specific;
			QString activity_text;
			if(event.size()<3)
				activity_general = "";
			else
				activity_general = *(QString*)event[2];
			if(event.size()<4)
				activity_specific = "";
			else
				activity_specific = *(QString*)event[3];
			if(event.size()<3)
				activity_text = "";
			else
				activity_text = *(QString*)event[4];
			ActivityExtension *activity = new ActivityExtension(activity_general, activity_specific, activity_text);
			PubSub::ItemList items;
			Tag *tag = new Tag("item");
			tag->addChild(activity->tag());
			items.push_back(new  PubSub::Item(tag));
			m_pubsub_manager->publishItem	(JID(), "http://jabber.org/protocol/activity", items, 0, this);
			delete activity;
			break;
		}
		case SetTune:{
			TuneExtension *tune = new TuneExtension();
			if(event.size()>2)
				tune->setArtist(*(QString*)event[2]);
			if(event.size()>3)
				tune->setLength(*(int*)event[3]);
			if(event.size()>4)
				tune->setRating(*(int*)event[4]);
			if(event.size()>5)
				tune->setSource(*(QString*)event[5]);
			if(event.size()>6)
				tune->setTitle(*(QString*)event[6]);
			if(event.size()>7)
				tune->setTrack(*(QString*)event[7]);
			if(event.size()>8)
				tune->setUri(*(QString*)event[8]);
			PubSub::ItemList items;
			Tag *tag = new Tag("item");
			tag->addChild(tune->tag());
			items.push_back(new  PubSub::Item(tag));
			m_pubsub_manager->publishItem	(JID(), "http://jabber.org/protocol/tune", items, 0, this);
			delete tune;
		}
			break;
	}
}

void jProtocol::onTimer()
{
    jClient->whitespacePing();
}

void jProtocol::handleEvent (const Event &/*event*/)
{
}


void jProtocol::showTransportReg(const QString &jid)
{
	jTransport *tran = new jTransport(jClient, jid);
	tran->addRegistration();
}

void jProtocol::removeTransportReg(const QString &jid)
{
	jTransport *tran = new jTransport(jClient, jid);
	tran->removeRegistration();
}

void jProtocol::receiveInvite(const JID &room, const JID &from, const QString &reason, const QString &password)
{
	emit conferenceInvite(room, from, reason, password);
}

void jProtocol::clearRecentBookmarks()
{
	QSettings *recent_settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "recent");
	recent_settings->remove("bookmarks");
	recent_settings->remove("urlmarks");
}

void jProtocol::setRecentBookmarks(const BookmarkList &b_list, const ConferenceList &c_list)
{
	QSettings *recent_settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "recent");
	recent_settings->beginWriteArray("bookmarks");
	QList<ConferenceListItem> m_c_list;
	for (ConferenceList::const_iterator it=c_list.begin(); it!=c_list.end(); it++) m_c_list.append(*it);
	for (int i = 0; i < m_c_list.size(); ++i)
	{
		recent_settings->setArrayIndex(i);
		recent_settings->setValue("name", jProtocol::fromStd(m_c_list[i].name));
		recent_settings->setValue("jid", jProtocol::fromStd(m_c_list[i].jid));
		recent_settings->setValue("nick", jProtocol::fromStd(m_c_list[i].nick));
		recent_settings->setValue("password", jProtocol::fromStd(m_c_list[i].password));
		recent_settings->setValue("autojoin", m_c_list[i].autojoin);
	}
	recent_settings->endArray();
	recent_settings->beginWriteArray("urlmarks");
	QList<BookmarkListItem> m_b_list;
	for (BookmarkList::const_iterator it=b_list.begin(); it!=b_list.end(); it++) m_b_list.append(*it);
	for (int i = 0; i < m_b_list.size(); ++i)
	{
		recent_settings->setArrayIndex(i);
		recent_settings->setValue("name", jProtocol::fromStd(m_b_list[i].name));
		recent_settings->setValue("url", jProtocol::fromStd(m_b_list[i].url));
	}
	recent_settings->endArray();
}
