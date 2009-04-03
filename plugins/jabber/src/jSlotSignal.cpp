/*
    jSlotSignal.cpp

    Copyright (c) 2008-2009 by Nikita Belov <zodiac.nv@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "jSlotSignal.h"
#include "jClientIdentification.h"
#include "acceptautdialog.h"


jSlotSignal::jSlotSignal(jAccount *parent) : m_jabber_account(parent)
{
	m_xml_console=0;
//	m_bookmark_settings=0;
	isConnected = false;
}

jSlotSignal::~jSlotSignal()
{
}

void jSlotSignal::setContactItemStatus(const TreeModelItem &item, const QString &status, int mass)
{
	m_jabber_account->getPluginSystem().setContactItemStatus(item, m_jabber_account->getPluginSystem().getStatusIcon(status, "jabber"), status, mass);
}

void jSlotSignal::addItemToContactList(const TreeModelItem &item, const QString &item_name)
{
//	qDebug() << item.m_item_name << item.m_item_type << "added SlotSignal";
	m_jabber_account->getPluginSystem().addItemToContactList(item, item_name);
}

void jSlotSignal::removeItemFromContactList(TreeModelItem Item)
{
	m_jabber_account->getPluginSystem().removeItemFromContactList(Item);
}

void jSlotSignal::setClientCustomIcon(const TreeModelItem & item, const QString &icon_name, int position)
{
	QIcon icon = m_jabber_account->getPluginSystem().getIcon(icon_name);
	if(icon.actualSize(QSize(16,16)).width()<0)
		icon = QIcon(icon_name);
	m_jabber_account->getPluginSystem().setContactItemIcon(item, icon, position);
}

void jSlotSignal::setClientCustomText(const TreeModelItem &item, const QString &text, int position)
{
	QList<QVariant> list;
	if(!text.isEmpty())
	{
		QString tmp_text = text;
		tmp_text.replace("\n"," | ");
		list.append(tmp_text);
	}
	m_jabber_account->getPluginSystem().setContactItemRow(item, list, position);
}

void jSlotSignal::addMessageFrom(const TreeModelItem &item, const QDateTime &date, const QString &text)
{
	m_jabber_account->getPluginSystem().addMessageFromContact(item,text,date);
}

void jSlotSignal::addServiceMessage(const TreeModelItem &item, const QString &text)
{
	m_jabber_account->getPluginSystem().addServiceMessage(item,text);
}

void jSlotSignal::setItemInvisible(const TreeModelItem &item, bool invisible)
{
	m_jabber_account->getPluginSystem().setItemInvisible(item, invisible);
}

void jSlotSignal::messageDelievered(const TreeModelItem &item, int position)
{
	m_jabber_account->getPluginSystem().messageDelievered(item, position);
}

void jSlotSignal::sendTypingNotification(const TreeModelItem &item, bool composing)
{
	m_jabber_account->getPluginSystem().contactTyping(item, composing);
}

void jSlotSignal::addConferenceItem(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname)
{
        m_jabber_account->getPluginSystem().addConferenceItem(protocol_name,conference_name,account_name,nickname);
        m_jabber_account->getPluginSystem().setConferenceItemIcon(protocol_name, conference_name, account_name, nickname,QIcon(":/icons/clients/unknown"),12);
}

void jSlotSignal::removeConferenceItem(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname)
{
	m_jabber_account->getPluginSystem().removeConferenceItem(protocol_name,conference_name,account_name,nickname);
}

void jSlotSignal::renameConferenceItem(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname,const QString &new_nickname)
{
	m_jabber_account->getPluginSystem().renameConferenceItem(protocol_name,conference_name,account_name,nickname,new_nickname);
}

void jSlotSignal::setConferenceItemStatus(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname,const QString &status, int mass)
{
	QIcon icon;
	switch(mass)
	{
	case 1:
		icon=jPluginSystem::instance().getStatusIcon(Presence::Chat);
		break;
	case 2:
		icon=jPluginSystem::instance().getStatusIcon(Presence::Away);
		break;
	case 3:
		icon=jPluginSystem::instance().getStatusIcon(Presence::DND);;
		break;
	case 4:
		icon=jPluginSystem::instance().getStatusIcon(Presence::XA);
		break;
	default:
		icon=jPluginSystem::instance().getStatusIcon(Presence::Available);
		break;
	}
	m_jabber_account->getPluginSystem().setConferenceItemStatus(protocol_name,conference_name,account_name,nickname,icon,status,mass);
}

void jSlotSignal::setConferenceItemRole(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname,const QString &role, int mass)
{
	QIcon icon;
	switch(mass)
	{
	case 1:
		icon=QIcon(":/icons/dnd.png");
		break;
	case 2:
		icon=QIcon(":/icons/online.png");
		break;
	case 3:
		icon=QIcon(":/icons/chatty.png");
		break;
	}
	m_jabber_account->getPluginSystem().setConferenceItemRole(protocol_name,conference_name,account_name,nickname,icon,role,mass);
}

void jSlotSignal::addMessageToConference(const QString &conference_name, const QString &account_name,
		const QString &from, const QString &message, const QDateTime &date, bool history)
{
	m_jabber_account->getPluginSystem().addMessageToConference("Jabber", conference_name,
			account_name,
			from,
			message, date, history);
}

void jSlotSignal::conferenceClientVersion(const QString &protocol_name,const QString &conference_name,
		const QString &account_name,const QString &nickname, const QString &name)
{
	m_jabber_account->getPluginSystem().setConferenceItemIcon(protocol_name, conference_name, account_name, nickname,jClientIdentification::instance().clientIcon(name.isEmpty()?"unknown":name),12);
}
void jSlotSignal::clientVersion(const TreeModelItem &contact, const QString &name)
{
	m_jabber_account->getPluginSystem().setContactItemIcon(contact,jClientIdentification::instance().clientIcon(name.isEmpty()?"unknown":name),12);
}

void jSlotSignal::systemNotification(const QString &account_name, const QString &message)
{
	TreeModelItem contact;
	contact.m_protocol_name = "Jabber";
	contact.m_account_name = account_name;
	contact.m_item_name = account_name;
	contact.m_item_type = TreeModelItem::Account;
	m_jabber_account->getPluginSystem().systemNotifiacation(contact, message);
}

void jSlotSignal::s_onFetchVCard(const QString &jid, const VCard *vcard, const QString &avatarUrl)
{
	m_jabber_account->setVCardInfo(vcard, jid, avatarUrl);
}

void jSlotSignal::changeOwnConferenceNickName(const QString &protocol_name,
		const QString &conference_name, const QString &account_name, const QString &nickname)
{
	m_jabber_account->getPluginSystem().changeOwnConferenceNickName(protocol_name, conference_name, account_name, nickname);
}

void jSlotSignal::createConference(const QString &protocol_name, const QString &conference_name, const QString &account_name)
{
	m_jabber_account->getPluginSystem().createConference(protocol_name, conference_name, account_name);
}

void jSlotSignal::setConferenceItemIcon(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QString &icon_name, int position)
{

	QIcon icon = m_jabber_account->getPluginSystem().getIcon(icon_name);
	if(icon.actualSize(QSize(16,16)).width()<0)
		icon = QIcon(icon_name);
	m_jabber_account->getPluginSystem().setConferenceItemIcon(protocol_name, conference_name, account_name, nickname,icon,position);
}
void jSlotSignal::setConferenceTopic(const QString &protocol_name, const QString &conference_name,
		const QString &account_name, const QString &topic)
{
	m_jabber_account->getPluginSystem().setConferenceTopic(protocol_name, conference_name, account_name, topic);
}

void jSlotSignal::addSystemMessageToConference(const QString &protocol_name, const QString &conference_name,
		const QString &account_name, const QString &message, const QDateTime &date, bool history)
{
	m_jabber_account->getPluginSystem().addSystemMessageToConference(protocol_name, conference_name, account_name, message, date, history);
}

void jSlotSignal::createAcceptAuthDialog(const QString &text, const JID &jid, Client *jabber_client)
{
	AcceptAuthDialog *widget = new AcceptAuthDialog(text,jid,jabber_client);
	widget->show();
}

void jSlotSignal::customNotification(const TreeModelItem &item, const QString &message)
{
	m_jabber_account->getPluginSystem().customNotifiacation(item, message);
}

void jSlotSignal::moveItemInContactList(const TreeModelItem &OldItem, const TreeModelItem &NewItem)
{
	m_jabber_account->getPluginSystem().moveItemInContactList(OldItem, NewItem);
}

void jSlotSignal::setContactItemName(const TreeModelItem &Item, const QString &name)
{
	m_jabber_account->getPluginSystem().setContactItemName(Item, name);
}
void jSlotSignal::bookmarksHandled(const BookmarkList &b_list, const ConferenceList &c_list)
{
	if (isConnected)
	{
		m_jabber_account->setConferences();
	}
	else
	{
		isConnected = true;
		ConferenceList::const_iterator ci = c_list.begin();
		for(;ci!=c_list.end();ci++)
		{
			if((*ci).autojoin)
			{
				JID jid((*ci).jid);
				QString server = jProtocol::fromStd(jid.server());
				QString room = jProtocol::fromStd(jid.username());
				QString conference = tr("%1@%2").arg(room).arg(server);
				QString nick = jProtocol::fromStd((*ci).nick);
				QString password = jProtocol::fromStd((*ci).password);
				m_jabber_account->getConferenceManagementObject()->s_createConferenceRoom(conference, nick, password);
			}
		}
	}
	/*QMenu *bookmark_menu = m_jabber_account->getProtocol()->getBookmarkMenu();
	if(bookmark_menu)
		delete bookmark_menu;
	bookmark_menu = new QMenu();
	QAction *bookmark_action = m_jabber_account->getProtocol()->getBookmarkAction();
	bookmark_action->setMenu(bookmark_menu);
	ConferenceList::const_iterator ci = c_list.begin();
	for(;ci!=c_list.end();ci++)
	{
		QAction *conference_action = new QAction(jPluginSystem::instance().getIcon("chat"),jProtocol::fromStd((*ci).name),bookmark_menu);
		bookmark_menu->addAction(conference_action);
		BookmarkAction *action = new BookmarkAction((*ci), m_jabber_account->getConferenceManagementObject(), conference_action);
		connect(conference_action, SIGNAL(triggered()),action, SLOT(triggered()));
		if((*ci).autojoin)
			action->triggered();
	}
	if(c_list.size() && b_list.size())
		bookmark_menu->addSeparator();
	BookmarkList::const_iterator bi = b_list.begin();
	for(;bi!=b_list.end();bi++)
	{
		QAction *url_action = new QAction(jPluginSystem::instance().getIcon("link"),jProtocol::fromStd((*bi).name),bookmark_menu);
		bookmark_menu->addAction(url_action);
		BookmarkAction *action = new BookmarkAction((*bi), url_action);
	}
	if(c_list.size() || b_list.size())
		bookmark_menu->addSeparator();

	QAction *m_bookmark_action = new QAction(jPluginSystem::instance().getIcon("bookmarkstorage"),tr("Configure"),bookmark_menu);
	connect(m_bookmark_action, SIGNAL(triggered()),
			this, SLOT(createBookmark()));
	bookmark_menu->addAction(m_bookmark_action);

	m_jabber_account->getProtocol()->setBookmarkMenu(bookmark_menu);
	m_b_list = b_list;
	m_c_list = c_list;
	if(m_bookmark_settings)
		m_bookmark_settings->setBookmarks(b_list, c_list);
	*/

}

void jSlotSignal::tagHandled(const QString &xml, bool in)
{
	if(m_xml_console)
	{
		m_xml_console->appendTag(xml,in);
	}
}

void jSlotSignal::createXmlConsole()
{
	if(!m_xml_console)
	{
		m_xml_console = new XmlConsole("Xml Console on " + m_jabber_account->getAccountName());
		m_xml_console->show();
		connect(m_xml_console, SIGNAL(destroyed()), this, SLOT(destroyXmlConsole()));
	}
}

void jSlotSignal::destroyXmlConsole()
{
	m_xml_console = 0;
}

/*void jSlotSignal::createBookmark()
{
	if(!m_bookmark_settings)
	{
		m_bookmark_settings = new jBookmark(m_jabber_account->getProtocol()->getBookmarkStorage());
		m_bookmark_settings->setBookmarks(m_b_list, m_c_list);
		m_bookmark_settings->show();
		connect(m_bookmark_settings, SIGNAL(destroyed()), this, SLOT(destroyBookmark()));
	}
}*/

/*void jSlotSignal::destroyBookmark()
{
	m_bookmark_settings = 0;
}*/

/*void jSlotSignal::s_receiveSDisco(const JID &from, const Disco::Items &items, QString &type)
{
	qDebug() << "items";
	if (type == "request-conference-item")
	{
		foreach (Disco::Item *item, items.items())
		{
			conferenceList << jProtocol::fromStd(item->jid().full());
		}
		emit setConferencesList(conferenceList);
		conferenceList.clear();
	}
	else
	{
		foreach (Disco::Item *item, items.items())
		{
			m_jabber_account->getProtocol()->getDiscoInfo(jProtocol::fromStd(item->jid().full()), "", type);
		}
	}
}

void jSlotSignal::s_receiveSDiscoInfo(const JID &from, const Disco::Info &info, QString &type)
{
	qDebug() << "info";
	foreach (Disco::Identity *identity, info.identities())
	{
		if (type == "request-conference")
		{
			qDebug() << jProtocol::fromStd(identity->category());
			if (jProtocol::fromStd(identity->category()) == "conference" && jProtocol::fromStd(identity->type()) == "text")
			{
				m_jabber_account->getDiskoItem(jProtocol::fromStd(from.full()), "", "request-conference-item");
			}
			else if (jProtocol::fromStd(identity->category()) == "server")
			{
				m_jabber_account->getDiskoItem(jProtocol::fromStd(from.full()), "", "request-conference");
			}
		}
		else
		{
			m_jabber_account->getDiskoItem(jProtocol::fromStd(from.full()), "", type);
		}
	}
}
*/
