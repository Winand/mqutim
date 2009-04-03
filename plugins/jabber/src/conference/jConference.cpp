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

#include "jConference.h"
#include "joinconferenceform.h"
#include <gloox/vcardupdate.h>
#include "reasondialog.h"
#include <QUrl>

BookmarkAction::BookmarkAction(const ConferenceListItem &item, jConference *cmo, QObject *parent) : QObject(parent)
{
	m_is_conference = true;
	m_conference_item = item;
	m_cmo = cmo;
}

BookmarkAction::BookmarkAction(const BookmarkListItem &item, QObject *parent) : QObject(parent)
{
	m_is_conference = false;
	m_url_item = item;
}

void BookmarkAction::triggered()
{
	if(m_is_conference)
	{
		JID jid(m_conference_item.jid);
		QString server = jProtocol::fromStd(jid.server());
		QString room = jProtocol::fromStd(jid.username());
		QString nick = jProtocol::fromStd(m_conference_item.nick);
		QString password = jProtocol::fromStd(m_conference_item.password);
		m_cmo->createConferenceRoom(server, room, nick, password);
	}
	else
	{
		QUrl url(jProtocol::fromStd(m_url_item.url));
		QDesktopServices::openUrl(url);
	}
}

struct jConference::MucContact
{
	jBuddy::ResourceInfo m_info;
	QString m_avatar_hash;
	QString m_role_string;
	QString m_affiliation_string;
	QString m_real_jid;
	MUCRoomRole m_role;
	MUCRoomAffiliation m_affiliation;
};

jConference::jConference(jProtocol *real_parent, Client *client, jAccount *plugin_system, QObject *parent) :
	QObject(parent),
	m_jabber_account(plugin_system),
	m_client_base(client),
	m_real_parent(real_parent),
	m_plugin_system(jPluginSystem::instance())
{
	m_account_name = jProtocol::fromStd(client->jid().bare());
	m_presence = &client->presence();


	m_context_menu = new QMenu();
	m_menu_label = new QLabel;
	m_menu_label->setAlignment(Qt::AlignCenter);
	m_menu_title = new QWidgetAction(this);
	m_menu_title->setDefaultWidget(m_menu_label);
	m_kick_user = new QAction(tr("Kick"), this);
	m_ban_user = new QAction(tr("Ban"), this);
	m_role_user = new QActionGroup(this);
	m_user_visitor = new QAction(tr("Visitor"), m_role_user);
	m_user_participant = new QAction(tr("Participant"), m_role_user);
	m_user_moderator = new QAction(tr("Moderator"), m_role_user);
	m_user_visitor->setCheckable(true);
	m_user_participant->setCheckable(true);
	m_user_moderator->setCheckable(true);
	connect(m_kick_user, SIGNAL(triggered()), this, SLOT(kickUser()));
	connect(m_ban_user, SIGNAL(triggered()), this, SLOT(banUser()));
	connect(m_user_visitor, SIGNAL(triggered()), this, SLOT(setVisitorUser()));
	connect(m_user_participant, SIGNAL(triggered()), this, SLOT(setParticipantUser()));
	connect(m_user_moderator, SIGNAL(triggered()), this, SLOT(setModeratorUser()));


	m_join_form = 0;

//	m_client_base->registerPresenceHandler(this);
        qDebug("jConference::jConference");
}

jConference::~jConference()
{
}

void jConference::loadSettings()
{
    QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_jabber_account->getProfileName()+"/jabber."+m_account_name, "accountsettings");
    m_auto_retrieve_vcards = account_settings.value("main/getavatars",true).toBool();

}

/*void jConference::joinGroupChat()
{
	JoinConferenceForm *join_form = new JoinConferenceForm(this);
	join_form->setWindowTitle(QObject::tr("Join groupchat"));
	join_form->setWindowIcon(QIcon(":/icons/chat"));
	join_form->setIcons(m_jabber_account->getPluginSystem().getIcon("apply"),
			m_jabber_account->getPluginSystem().getIcon("cancel"));
	join_form->show();
}*/

void jConference::joinGroupchat()
{
	joinGroupchat("", "", true);
}

void jConference::joinGroupchat(const QString &room, const QString &password, bool showBookmarks)
{
	m_join_form = new jJoinChat(m_jabber_account, room, password, showBookmarks);
	m_join_form->setWindowTitle(QObject::tr("Join groupchat on") + " " + m_jabber_account->getAccountName());
	m_join_form->setWindowIcon(QIcon(":/icons/chat"));
	m_join_form->show();
	connect(m_join_form, SIGNAL(createConferenceRoom(QString, QString, QString)), this, SLOT(s_createConferenceRoom(QString, QString, QString)));
	connect(m_join_form, SIGNAL(destroyed()), this, SLOT(destroyJoinGroupChat()));
}

void jConference::destroyJoinGroupChat()
{
	m_join_form = 0;
}

void jConference::s_conferenceInvite(const JID &room, const JID &from, const QString &reason_sent, const QString &password)
{
	QString reason = reason_sent;
	if (reason.isEmpty())
		reason = "no reason";
	if (QMessageBox(QMessageBox::Question, tr("Invite to groupchat"), tr("User %1 invite you\nto conference %2\nwith reason \"%3\"\nAccept invitation?").arg(jProtocol::fromStd(from.bare())).arg(jProtocol::fromStd(room.full())).arg(reason), QMessageBox::Yes | QMessageBox::No).exec() == QMessageBox::Yes)
		joinGroupchat(jProtocol::fromStd(room.full()), password, true);
}

void jConference::createConferenceRoom(const QString &host, const QString &room,
		const QString &nick, const QString &password)
{
	QString conference = QString("%1@%2").arg(room).arg(host);
	if(m_room_list.contains(conference))
	{
		MUCRoom *room = m_room_list[conference];
		if ( !password.isEmpty() )
			room->setPassword(jProtocol::toStd(password));
                room->join(m_presence->subtype(), m_presence->status());
                QHash<QString, MucContact> *contacts = m_contacts_list.value(conference,0);
                if(!contacts)
                    return;
                QStringList nicknames = contacts->keys();
                foreach(const QString &nick, nicknames)
                {
                    MucContact contact = contacts->take(nick);
                    emit removeConferenceItem("Jabber", conference, m_account_name, nick);
                }
		return;
	}
	//m_jabber_account->getPluginSystem().
	emit createConference("Jabber", conference, m_account_name );
	JID jid(jProtocol::toStd(room) + "@" + jProtocol::toStd(host) + "/" + jProtocol::toStd(nick));
	MUCRoom *new_room = new MUCRoom(m_client_base,jid,this);
	m_client_base->registerPresenceHandler(jid, this);
	if ( !password.isEmpty() )
	{
		new_room->setPassword(jProtocol::toStd(password));
	}

	m_room_list.insert(conference, new_room);
	m_contacts_list.insert(conference, new QHash<QString,MucContact>());
	//m_jabber_account->getPluginSystem().
	emit changeOwnConferenceNickName("Jabber",
	conference, m_account_name, jProtocol::fromStd(new_room->nick()));
        new_room->join(m_presence->subtype(), m_presence->status());
//	sendPresence(conference);
}

void jConference::handleMUCParticipantPresence (MUCRoom * /*room*/, const MUCRoomParticipant /*participant*/, Presence::PresenceType /*presence*/)
{
}

void jConference::handleMUCMessage (MUCRoom *room, const Message &msg, bool privateMessage)
{
	if ( privateMessage )
	{
		JID jid(room->name() + "@" + room->service() + "/" + msg.from().resource());
		m_real_parent->addMessageFrom(jid, msg);
	}
	else
	{
		const DelayedDelivery* dd = msg.when();
		if(msg.from().resource().empty())
			emit addSystemMessageToConference("Jabber",jProtocol::fromStd(room->name() +
					"@" + room->service()),m_account_name,jProtocol::fromStd(msg.body()),
					dd==0?QDateTime::currentDateTime():jProtocol::fromStamp(jProtocol::fromStd(dd->stamp())),dd==0);
		else
			emit addMessageToConference(jProtocol::fromStd(room->name() +
					"@" + room->service()), m_account_name,jProtocol::fromStd(msg.from().resource()),
					jProtocol::fromStd(msg.body()), dd==0?QDateTime::currentDateTime():jProtocol::fromStamp(jProtocol::fromStd(dd->stamp())), dd!=0);
     }
}

bool jConference::handleMUCRoomCreation (MUCRoom *room)
{
	qDebug() << "handleMUCRoomCreation" << room;
	if(room)
	{
		qDebug()<<"connected";
		return true;
	}
	qDebug()<<"not connected";
	return false;

}

void jConference::handleMUCSubject (MUCRoom *room, const std::string &nick, const std::string &subject)
{
	QString conference = jProtocol::fromStd(room->name())+"@"+jProtocol::fromStd(room->service());
	QString nickname = jProtocol::fromStd(nick);
	QString topic = jProtocol::fromStd(subject);
	if(!nickname.isEmpty())
		emit addSystemMessageToConference("Jabber",conference,m_account_name,tr("%1 has set the subject to:\n%2").arg(nickname).arg(topic),QDateTime(),false);
	else
		emit addSystemMessageToConference("Jabber",conference,m_account_name,tr("The subject is:\n%2").arg(topic),QDateTime(),false);
	m_topic_list.insert(conference, topic);
	topic.replace("\n"," | ");
	emit setConferenceTopic("Jabber",conference,m_account_name,topic);
}

void jConference::handleMUCInviteDecline (MUCRoom * /*room*/, const JID &/*invitee*/, const std::string &/*reason*/)
{
}

void jConference::handleMUCError (MUCRoom *room, StanzaError error)
{
/*	PresenceJidHandlerList::const_iterator itj = m_client_base->m_presenceJidHandlers.begin();
	for( ; itj != m_client_base->m_presenceJidHandlers.end(); ++itj )
	{
		qDebug() << jProtocol::fromStd((*itj).jid->bare());
		if( (*itj).jid->bare() == pres.from().bare() && (*itj).ph )
		{
			(*itj).ph->handlePresence( pres );
			match = true;
		}
	}*/
	QString conference_name = jProtocol::fromStd(room->name()+"@"+room->service());
//	JID jid(room->name()+"@"+room->service());
//	m_client_base->removePresenceHandler( jid, this);//(PresenceHandler *)room );
	//m_client_base->removePresenceHandler( (PresenceHandler *)room );
	/*if ( m_room_list.contains(conference_name))
	{
		MUCRoom *tmp_room = m_room_list.value(conference_name);
		qDebug() << "tmp_room" << tmp_room;
		m_room_list.remove(m_room_list.key(tmp_room));
		tmp_room->leave();
		delete tmp_room;
		delete static_cast<QHash<QString,jBuddy::ResourceInfo> *>(m_contacts_list[conference_name]);
		m_contacts_list.remove(conference_name);
	}*/
	QString error_tr;
	switch(error)
	{
	case StanzaErrorNotAuthorized:
    		error_tr=tr("Not authorized: Password required.");
    		break;
	case StanzaErrorForbidden:
    		error_tr=tr("Forbidden: Access denied, user is banned.");
    		break;
	case StanzaErrorItemNotFound:
    		error_tr=tr("Item not found: The room does not exist.");
    		break;
	case StanzaErrorNotAllowed:
    		error_tr=tr("Not allowed: Room creation is restricted.");
    		break;
	case StanzaErrorNotAcceptable :
    		error_tr=tr("Not acceptable: Room nicks are locked down.");
    		break;
	case StanzaErrorRegistrationRequired:
    		error_tr=tr("Registration required: User is not on the member list.");
    		break;
	case StanzaErrorConflict:
    		error_tr=tr("Conflict: Desired room nickname is in use or registered by another user.");
    		break;
	case StanzaErrorServiceUnavailable:
    		error_tr=tr("Service unavailable: Maximum number of users has been reached.");
    		break;
    	default:
    		error_tr=tr("Unknown error: No description.");
    		break;
	}
	emit addSystemMessageToConference("Jabber",conference_name,m_account_name,error_tr,QDateTime(),false);
}

void jConference::handleMUCInfo (MUCRoom * /*room*/, int /*features*/, const std::string &/*name*/, const DataForm * /*infoForm*/)
{
}

void jConference::handleMUCItems (gloox::MUCRoom*, const std::list<gloox::Disco::Item*, std::allocator<gloox::Disco::Item*> >&)
{
}

void jConference::sendMessageToConference(const QString &conference_name, const QString &message)
{
	if ( m_room_list.contains(conference_name))
	{
		bool handled=false;
		if(message.startsWith("/nick "))
		{
			QString nick = message.section(' ',1);
			if(!nick.isEmpty())
			{
				m_room_list.value(conference_name)->setNick(jProtocol::toStd(nick));
				handled = true;
			}
		}
		if(message.startsWith("/topic "))
		{
			QString topic = message.section(' ',1);
			if(!topic.isEmpty())
			{
				m_room_list.value(conference_name)->setSubject(jProtocol::toStd(topic));
				handled = true;
			}
		}
		if(!handled)
			m_room_list.value(conference_name)->send(jProtocol::toStd(message));
	}
}

void jConference::leaveConference(const QString& conference_name)
{
	if ( m_room_list.contains(conference_name))
	{
		MUCRoom *tmp_room = m_room_list.value(conference_name);
		tmp_room->leave(""); // quit reason
		m_room_list.remove(m_room_list.key(tmp_room));
		delete tmp_room;
		delete m_contacts_list[conference_name];
		m_contacts_list.remove(conference_name);
		m_topic_list.remove(conference_name);
	}
}

void jConference::handleDiscoInfo(const JID &from, const Disco::Info &info, int /*context*/)
{
	QString bare = jProtocol::fromStd(from.bare());
	QString resource = jProtocol::fromStd(from.resource());
	if(m_contacts_list.contains(bare))
	{
		QHash<QString,MucContact> *contacts = m_contacts_list[bare];
		if(!contacts->contains(resource))
			return;
		jBuddy::ResourceInfo *resource_info = &(*contacts)[resource].m_info;
		jClientIdentification::instance().newInfo(info,resource_info);
	}
}

void jConference::handleDiscoItems(const JID &/*from*/, const Disco::Items &/*items*/, int /*context*/)
{
}

void jConference::handleDiscoError(const JID &/*from*/, const Error * /*error*/, int /*context*/)
{
}

bool jConference::handleDiscoSet(const IQ &/*iq*/)
{
	return false;
}

void jConference::handlePresence (const Presence &presence)
{
	if(presence.subtype()==Presence::Error)
		return;
	const MUCRoom::MUCUser* mu = presence.findExtension<MUCRoom::MUCUser>( ExtMUCUser );
	if(!mu)
		return;
	QString conference = jProtocol::fromStd(presence.from().bare());
	QString nick = jProtocol::fromStd(presence.from().resource());
	if(!m_contacts_list.contains(conference))
		return;
	QHash<QString,MucContact> *contacts = m_contacts_list[conference];

	int flags = mu->flags();
	bool send_role=false;
	if( flags & UserNickChanged && mu->newNick() )
	{
		// changed nick to new_nick
		QString new_nick = jProtocol::fromStd(*mu->newNick());
		if(!contacts->contains(nick))
			return;
		MucContact contact = (*contacts)[nick];
		jBuddy::ResourceInfo *info = &contact.m_info;
		info->m_name = new_nick;
		contacts->remove(nick);
		contacts->insert(new_nick, contact);
		emit addSystemMessageToConference("Jabber",conference,m_account_name,tr("%1 is now known as %2").arg(nick).arg(new_nick),QDateTime(),false);
		emit renameConferenceItem("Jabber",
			conference, m_account_name, nick, new_nick);
		return;
	}

	bool if_join=false;
	if(presence.presence()==Presence::Unavailable && contacts->contains(nick))
	{
		//offline
		contacts->remove(nick);
		emit removeConferenceItem("Jabber",
			conference, m_account_name, nick);
		QString text;
		if(flags & UserKicked)
			text = tr("%1 has been kicked").arg(nick);
		else if(flags & UserBanned)
			text = tr("%1 has been banned").arg(nick);
		else
			text = tr("%1 has left the room").arg(nick);
		if(mu->reason())
			text += jProtocol::fromStd(" ("+(*mu->reason())+")");
//		QString text = participant.reason.empty()?tr("%1 has left the room").arg(nick):tr("%1 has left the room (%2)").arg(nick).arg(jProtocol::fromStd(participant.reason));
		emit addSystemMessageToConference("Jabber",conference,m_account_name,text,QDateTime(),false);
		return;
        }
        else if(presence.presence()==Presence::Unavailable)
            return;
	else
	{
		QString status;
		int mass;
		switch(presence.presence())
		{
		case Presence::Chat:
			status="ffc";
			mass=1;
			break;
		case Presence::Away:
			status="away";
			mass=2;
			break;
		case Presence::DND:
			status="dnd";
			mass=3;
			break;
		case Presence::XA:
			status="na";
			mass=4;
			break;
		case Presence::Error:
			return;
		case Presence::Available:
		default:
			status="online";
			mass=0;
			break;
		}
                if(!contacts->contains(nick) || (*contacts)[nick].m_info.m_name.isNull() || (*contacts)[nick].m_info.m_name.isEmpty())
		{
			//online
                        if(!contacts->contains(nick))
			{
                                MucContact contact;
				jBuddy::ResourceInfo info;
                                info.m_name = nick;
				info.m_chat_state = ChatStateActive;
                                contact.m_info = info;
				contact.m_affiliation = AffiliationNone;
                                contact.m_role = RoleInvalid ;
				contacts->insert(nick,contact);
qDebug("from jabber: addConferenceItem");				emit addConferenceItem("Jabber",
                                        conference, m_account_name, nick);
			}
qDebug("Zzegfault!!!");			MucContact *contact = &(*contacts)[nick];
                        jBuddy::ResourceInfo *info = &contact->m_info;
qDebug("Where is my segfaults??");
                        info->m_name = nick;
                        info->m_presence = presence.presence();
			if(presence.presence() == Presence::Invalid)
				info->m_presence = Presence::Available;
                        info->m_priority = 0;
			if_join=true;
                        emit setConferenceItemRole("Jabber",
				conference, m_account_name, nick, status, mass);
			send_role=true;
//			JID jid(room->name()+"@"+room->service()+"/"+participant.nick->resource());
//			jClientIdentification::instance().setClient(&((*contacts)[nick]),jid,m_client_base,this);
		}
		else
			(*contacts)[nick].m_info.m_presence = presence.presence();
		if((*contacts)[nick].m_real_jid.isEmpty() && mu->jid())
			(*contacts)[nick].m_real_jid = jProtocol::fromStd(*mu->jid());
		//changed status
		emit setConferenceItemStatus("Jabber",
			conference, m_account_name, nick, status, mass);
	}
	if(contacts->contains(nick))
	{
		MucContact *contact = &(*contacts)[nick];
		QString message;
		if(!if_join && mu->role()==contact->m_role && mu->affiliation()==contact->m_affiliation)
			return;
		if(if_join && contact->m_real_jid.isEmpty())
		{
			if(mu->role()!=contact->m_role&&mu->affiliation()!=contact->m_affiliation)
				message = tr("%3 has joined the room as %1 and %2");
			else if(mu->role()!=contact->m_role||mu->affiliation()!=contact->m_affiliation)
				message = tr("%2 has joined the room as %1");
			else
				message = tr("%2 has joined the room");
		}
		else if(if_join)
		{
			if(mu->role()!=contact->m_role&&mu->affiliation()!=contact->m_affiliation)
				message = tr("%4 (%3) has joined the room as %1 and %2");
			else if(mu->role()!=contact->m_role||mu->affiliation()!=contact->m_affiliation)
				message = tr("%3 (%2) has joined the room as %1");
			else
				message = tr("%2 (%1) has joined the room");
		}
		else if(contact->m_real_jid.isEmpty())
		{
			if(mu->role()!=contact->m_role&&mu->affiliation()!=contact->m_affiliation)
				message = tr("%3 now is %1 and %2");
			else if(mu->role()!=contact->m_role||mu->affiliation()!=contact->m_affiliation)
				message = tr("%2 now is %1");
		}
		else
		{
			if(mu->role()!=contact->m_role&&mu->affiliation()!=contact->m_affiliation)
				message = tr("%4 (%3) now is %1 and %2");
			else if(mu->role()!=contact->m_role||mu->affiliation()!=contact->m_affiliation)
				message = tr("%3 (%2) now is %1");
		}
		if(mu->role()!=contact->m_role)
		{
			int mass;
			QString role;
			switch(mu->role())
			{
			case RoleModerator:
				role=tr("moderator");
				mass=1;
				break;
			case RoleParticipant:
				role=tr("participant");
				mass=2;
				break;
			default:
			case RoleVisitor:
				role=tr("visitor");
				mass=3;
				break;
			}
			contact->m_role = mu->role();
			contact->m_role_string = role;
			message = message.arg(role);
			contact->m_info.m_priority = mass;
			emit setConferenceItemRole("Jabber", conference, m_account_name, nick, role, mass);
		}
		if(mu->affiliation()!=contact->m_affiliation)
		{
			QString affiliation;
			switch(mu->affiliation())
			{
				case AffiliationOutcast:
					affiliation=tr("banned");
					break;
				case AffiliationMember:
					affiliation=tr("member");
					break;
				case AffiliationOwner:
					affiliation=tr("owner");
					break;
				case AffiliationAdmin:
					affiliation=tr("administrator");
					break;
				default:
					affiliation=tr("guest");
					break;
			}
			contact->m_affiliation = mu->affiliation();
			contact->m_affiliation_string = mu->affiliation()==AffiliationNone?"":affiliation;
			message = message.arg(affiliation);
		}
		if(!contact->m_real_jid.isEmpty())
			message = message.arg(contact->m_real_jid);
		message = message.arg(nick);
		emit addSystemMessageToConference("Jabber",conference,m_account_name,message,QDateTime(),false);
	}
	jBuddy::ResourceInfo *info = &(*contacts)[nick].m_info;
	info->m_status_message = jProtocol::fromStd(presence.status());
	info->m_status_message.replace("\n","<br/>");
	if(info->m_client_name.isEmpty()||info->m_client_name.isNull())
	{
		const Capabilities *caps = presence.capabilities();
		if(caps)
		{
			info->m_caps_ver = jProtocol::fromStd(caps->ver());
			info->m_caps_node = jProtocol::fromStd(caps->node());
		}
		else
		{
			info->m_caps_ver = "";
			info->m_caps_node = "";
		}
		jClientIdentification::instance().setClient(info,presence.from(),m_client_base,this);
		emit conferenceClientVersion("Jabber",conference,m_account_name,nick, info->m_client_name);
	}
	const VCardUpdate *vcard_update = presence.findExtension<VCardUpdate>(ExtVCardUpdate);
	if(vcard_update)
	{
		QString hash = jProtocol::fromStd(vcard_update->hash());
                if(!hash.isEmpty())
		{
			if(QFile(m_jabber_account->getPathToAvatars()+"/"+hash).exists())
			{
				setAvatar(conference,nick,m_jabber_account->getPathToAvatars()+"/"+hash);
			}
			else
			{
				m_real_parent->fetchVCard(conference+"/"+nick,true);
			}
		}
	}
	const XStatusExtension *xstatus = presence.findExtension<XStatusExtension>(ExtUser+1);
	if(xstatus)
	{
		if(xstatus->status_id()<0)
		{
			info->m_xstatus.m_html="";
			info->m_xstatus.m_icon="";
			info->m_xstatus.m_position=-1;
		}
		else
		{
			QString icon = QString("icq_xstatus%1").arg(xstatus->status_id());
			info->m_xstatus.m_html = "<b>"+Qt::escape(xstatus->status_title())+"</b> "+Qt::escape(xstatus->status_text());
			info->m_xstatus.m_html = info->m_xstatus.m_html.replace("\n","<br/>");
			info->m_xstatus.m_icon = icon;
			info->m_xstatus.m_position = 5;
			emit setConferenceItemIcon("Jabber",conference,m_account_name,nick, icon, 5);
		}
	}
}

void jConference::setClient(const QString &conference, const QString &nick, const VersionExtension *version)
{
	if(!m_contacts_list.contains(conference))
			return;
	QHash<QString,MucContact> *contacts = m_contacts_list[conference];
	if(!contacts->contains(nick))
		return;
	jBuddy::ResourceInfo *info = &(*contacts)[nick].m_info;
	jClientIdentification::instance().newInfo(version, info);
	emit conferenceClientVersion("Jabber",conference,m_account_name,nick, info->m_client_name);
	if(info->m_client_name.isEmpty())
		qDebug() << (conference+"/"+nick) << "no client";
	else
		qDebug() << (conference+"/"+nick) << info->m_client_name;
}

void jConference::sendPresence(const QString &conference)
{
//	JID jid(jProtocol::toStd(conference)+"/"+m_room_list[conference]->nick());
//	Presence presence(m_presence->subtype(),jid,m_presence->status(),m_presence->priority());
//	const StanzaExtensionList &extensions = m_presence->extensions();
//	StanzaExtensionList::const_iterator it = extensions.begin();
//	for( ; it != extensions.end(); ++it )
//		presence.addExtension((*it)->newInstance((*it)->tag()));
//	m_client_base->send(presence);
        m_room_list[conference]->setPresence(m_presence->subtype(), m_presence->status());
}

void jConference::sendPresenceToAll()
{
	QStringList rooms = m_room_list.keys();
	foreach(const QString &room, rooms)
		sendPresence(room);
}

QString jConference::getToolTip(const QString &conference, const QString &nickname)
{
	if(!m_contacts_list.contains(conference))
			return nickname;
	QHash<QString,MucContact> *contacts = m_contacts_list[conference];
	if(!contacts->contains(nickname))
		return nickname;
	MucContact *contact = &(*contacts)[nickname];
	jBuddy::ResourceInfo *info = &contact->m_info;
	QString html="<table><tr><td>";
	html.append("<img src='" + m_plugin_system.getStatusIconName(info->m_presence) + "'>");
	html.append("<b>"+info->m_name+"</b><br/>");
	if(!contact->m_affiliation_string.isEmpty())
		html.append(tr("<font size='2'><b>Affilation:</b> %1</font>").arg(contact->m_affiliation_string)+"<br/>");
	if(!contact->m_role_string.isEmpty())
		html.append(tr("<font size='2'><b>Role:</b> %1</font>").arg(contact->m_role_string)+"<br/>");
	if(!contact->m_real_jid.isEmpty())
		html.append(tr("<font size='2'><b>JID:</b> %1</font>").arg(contact->m_real_jid)+"<br/>");
	html.append(jBuddy::getBaseToolTip(*info));
	html.append("</td></tr>");
	html.append("</table>");
	if(!contact->m_avatar_hash.isEmpty())
	{
		html = QString("<table><tr><td>%2</td><td><img src=\"%1\"/></td></tr></table>").arg(contact->m_avatar_hash).arg(html);
	}
	return html;
}

void jConference::itemContextMenu(const QList<QAction*> &action_list,const QString &conference_name,
		const QString &nickname,const QPoint &menu_point)
{

	if(!m_contacts_list.contains(conference_name) || !m_room_list.contains(conference_name))
			return;
	QHash<QString,MucContact> *contacts = m_contacts_list[conference_name];
	if(!contacts->contains(nickname))
		return ;
	MucContact *contact = &(*contacts)[nickname];
    MucContact *mine_contact = &(*contacts)[jProtocol::fromStd(m_room_list.value(conference_name)->nick())];
	m_context_menu->clear();
	m_context_menu->addAction(m_menu_title);
	m_menu_label->setText("<b>"+ nickname +"</b>");
	m_current_context_nick = nickname;
	m_current_context_conference_name = conference_name;
	// Default actions at menu
	for(int i=0;i<2 && i<action_list.size();i++)
		m_context_menu->addAction(action_list[i]);


	if ( mine_contact->m_role == RoleModerator )
	{
		m_context_menu->addSeparator();
		m_context_menu->addAction(m_kick_user);
		m_context_menu->addAction(m_ban_user);
		m_context_menu->addSeparator();
		m_context_menu->addAction(m_user_moderator);
		m_context_menu->addAction(m_user_participant);
		m_context_menu->addAction(m_user_visitor);
		switch (contact->m_role)
		{
		case RoleModerator:
			m_user_moderator->setChecked(true);
			break;
		case RoleParticipant:
			m_user_participant->setChecked(true);
			break;
		default:
			m_user_visitor->setChecked(true);
		}
	}

	// Actions at menu from plugins
	for(int i=3;i<action_list.size();i++)
		m_context_menu->addAction(action_list[i]);

	m_context_menu->exec(menu_point);
}

void jConference::kickUser()
{
	if(!m_room_list.contains(m_current_context_conference_name))
			return;

	ReasonDialog dialog;
	dialog.setWindowTitle(tr("Kick message"));
	if ( dialog.exec() )
	{
		m_room_list.value(m_current_context_conference_name)->kick(
			jProtocol::toStd(m_current_context_nick), jProtocol::toStd(dialog.m_reason_text));
	}

}



void jConference::setVisitorUser()
{
	if(!m_room_list.contains(m_current_context_conference_name))
		return;
	m_room_list.value(m_current_context_conference_name)->setRole(jProtocol::toStd(m_current_context_nick), RoleVisitor );
}

void jConference::setParticipantUser()
{
	if(!m_room_list.contains(m_current_context_conference_name))
		return;
	m_room_list.value(m_current_context_conference_name)->setRole(jProtocol::toStd(m_current_context_nick), RoleParticipant );
}

void jConference::setModeratorUser()
{
	if(!m_room_list.contains(m_current_context_conference_name))
		return;
	m_room_list.value(m_current_context_conference_name)->setRole(jProtocol::toStd(m_current_context_nick), RoleModerator );
}


void jConference::banUser()
{
	if(!m_room_list.contains(m_current_context_conference_name))
			return;

	ReasonDialog dialog;
	dialog.setWindowTitle(tr("Ban message"));
	if ( dialog.exec() )
	{
		m_room_list.value(m_current_context_conference_name)->ban(
			jProtocol::toStd(m_current_context_nick), jProtocol::toStd(dialog.m_reason_text));
	}
}


QStringList jConference::getAdditionalInfoAboutContact(const QString &conference, const QString &nickname)
{
	QStringList info_list;
	info_list << nickname;
	if(!m_contacts_list.contains(conference))
		return info_list;
	QHash<QString,MucContact> *contacts = m_contacts_list[conference];
	if(!contacts->contains(nickname))
		return info_list;
	jBuddy::ResourceInfo *info = &((*contacts)[nickname].m_info);
	info_list << QString();
	info_list << (info->m_client_name + " " + info->m_client_version + (info->m_client_os.isEmpty()?"":(" / " + info->m_client_os)));
	info_list << conference+"/"+nickname;
	return info_list;
}

void jConference::setAvatar(const QString &conference, const QString &nickname, const QString &path)
{
	QHash<QString,MucContact> *contacts = m_contacts_list.value(conference,0);
	if(!contacts)
		return;
	if(!contacts->contains(nickname))
		return;
	MucContact *contact = &(*contacts)[nickname];
	contact->m_avatar_hash = path;
	emit setConferenceItemIcon("Jabber",conference,m_account_name,nickname,path,1);
}

void jConference::connectAll()
{
	QList<MUCRoom *> rooms = m_room_list.values();
        foreach(MUCRoom *room, rooms)
                room->join(m_presence->subtype(), m_presence->status());
}

void jConference::disconnectAll()
{
	QList<MUCRoom *> rooms = m_room_list.values();
	foreach(MUCRoom *room, rooms)
	{
		room->leave("qutIM : Jabber plugin");
	}
}

void jConference::setConferences()
{
	if (m_join_form)
		m_join_form->setConferences();
}

void jConference::s_createConferenceRoom(const QString &conference, const QString &nickname, const QString &password)
{
	if(m_room_list.contains(conference))
	{
		MUCRoom *room = m_room_list[conference];
		if ( !password.isEmpty() )
			room->setPassword(jProtocol::toStd(password));
                room->join(m_presence->subtype(), m_presence->status());
		return;
	}
	//m_jabber_account->getPluginSystem().
	emit createConference("Jabber", conference, m_account_name );
	JID jid(jProtocol::toStd(conference) + "/" + jProtocol::toStd(nickname));
	MUCRoom *new_room = new MUCRoom(m_client_base,jid,this);
	m_client_base->registerPresenceHandler(jid, this);
	if ( !password.isEmpty() )
	{
		new_room->setPassword(jProtocol::toStd(password));
	}

	m_room_list.insert(conference, new_room);
	m_contacts_list.insert(conference, new QHash<QString,MucContact>());
	//m_jabber_account->getPluginSystem().
	emit changeOwnConferenceNickName("Jabber", conference, m_account_name, jProtocol::fromStd(new_room->nick()));
        new_room->join(m_presence->subtype(), m_presence->status());
}

void jConference::showTopicConfig(const QString &conference)
{
	if (!m_room_list.contains(conference))
		return;

	topicConfigDialog configDialog;
	configDialog.setTopic(m_topic_list.value(conference));
	if (configDialog.exec())
		m_room_list.value(conference)->setSubject(jProtocol::toStd(configDialog.getTopic()));
}

void jConference::chatWindowAboutToBeOpened(const QString &conference, const QString &nick)
{
}

void jConference::chatWindowClosed(const QString &conference, const QString &nick)
{
}
