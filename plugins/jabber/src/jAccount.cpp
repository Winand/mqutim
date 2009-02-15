/*
    jAccount.h

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

#include "jAccount.h"
#include <gloox/sha.h>

jAccount::jAccount(const QString &account_name, const QString &profile_name, QObject *parent) : QObject(parent), m_jabber_plugin_system(jPluginSystem::instance())
{
	m_status_icon = m_jabber_plugin_system.getStatusIcon("offline", "jabber");
	m_account_name = account_name;
	m_profile_name = profile_name;

	QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name/*+"/jabber."+m_account_name*/, "jabbericons");
	m_path_to_avatars = account_settings.fileName();
	m_path_to_avatars.truncate(m_path_to_avatars.length()-4);

	m_jabber_slotsignal = new jSlotSignal(this);
	m_jabber_roster = new jRoster(account_name, profile_name, this);
	m_jabber_protocol = new jProtocol(m_account_name, m_profile_name, m_jabber_roster, this);
	m_conference_management_object = m_jabber_protocol->getConferenceManagmentObject();

	createStatusMenu();

	m_status = Presence::Unavailable;
	connect(m_jabber_roster, SIGNAL(addItemToContactList(const TreeModelItem &, QString)), m_jabber_slotsignal, SLOT(addItemToContactList(const TreeModelItem &, QString)));
	connect(m_jabber_roster, SIGNAL(removeItemFromContactList(TreeModelItem)), m_jabber_slotsignal, SLOT(removeItemFromContactList(TreeModelItem)));
	connect(m_jabber_roster, SIGNAL(setContactItemStatus(const TreeModelItem &, QString, int)), m_jabber_slotsignal, SLOT(setContactItemStatus(const TreeModelItem &, QString, int)));
	connect(m_jabber_roster, SIGNAL(setItemInvisible(const TreeModelItem &, bool)), m_jabber_slotsignal, SLOT(setItemInvisible(const TreeModelItem &, bool)));
	connect(m_jabber_protocol, SIGNAL(setRealStatus(int)), this, SLOT(setRealStatus(int)));
	connect(m_jabber_protocol, SIGNAL(setClientCustomIcon(const TreeModelItem &, const QString &, int)), m_jabber_slotsignal, SLOT(setClientCustomIcon(const TreeModelItem &, const QString &, int)));
	connect(m_jabber_protocol, SIGNAL(setClientCustomText(const TreeModelItem &, const QString &, int)), m_jabber_slotsignal, SLOT(setClientCustomText(const TreeModelItem &, const QString &, int)));
	connect(m_jabber_protocol, SIGNAL(addMessageFrom(const TreeModelItem &, const QDateTime &, const QString &)), m_jabber_slotsignal, SLOT(addMessageFrom(const TreeModelItem &, const QDateTime &, const QString &)));
	connect(m_jabber_roster, SIGNAL(addServiceMessage(const TreeModelItem &, const QString &)), m_jabber_slotsignal, SLOT(addServiceMessage(const TreeModelItem &, const QString &)));
	connect(m_jabber_protocol, SIGNAL(messageDelievered(const TreeModelItem &, int)), m_jabber_slotsignal, SLOT(messageDelievered(const TreeModelItem &, int)));
	connect(m_jabber_protocol, SIGNAL(sendTypingNotification(const TreeModelItem &, bool)), m_jabber_slotsignal, SLOT(sendTypingNotification(const TreeModelItem &, bool)));
	connect(m_conference_management_object, SIGNAL(addConferenceItem(const QString &,const QString &,const QString &,const QString &)), m_jabber_slotsignal, SLOT(addConferenceItem(const QString &,const QString &,const QString &,const QString &)));
	connect(m_conference_management_object, SIGNAL(removeConferenceItem(const QString &,const QString &,const QString &,const QString &)), m_jabber_slotsignal, SLOT(removeConferenceItem(const QString &,const QString &,const QString &,const QString &)));
	connect(m_conference_management_object, SIGNAL(renameConferenceItem(const QString &,const QString &,const QString &,const QString &,const QString &)), m_jabber_slotsignal, SLOT(renameConferenceItem(const QString &,const QString &,const QString &,const QString &,const QString &)));
	connect(m_conference_management_object, SIGNAL(setConferenceItemStatus(const QString &,const QString &,const QString &,const QString &,const QString &, int)), m_jabber_slotsignal, SLOT(setConferenceItemStatus(const QString &,const QString &,const QString &,const QString &,const QString &, int)));
	connect(m_conference_management_object, SIGNAL(setConferenceItemRole(const QString &,const QString &,const QString &,const QString &,const QString &, int)), m_jabber_slotsignal, SLOT(setConferenceItemRole(const QString &,const QString &,const QString &,const QString &,const QString &, int)));
	connect(m_conference_management_object, SIGNAL(setConferenceItemIcon(const QString &,const QString &,const QString &,const QString &,const QString &, int)), m_jabber_slotsignal, SLOT(setConferenceItemIcon(const QString &,const QString &,const QString &,const QString &,const QString &, int)));
	connect(m_conference_management_object, SIGNAL(addMessageToConference(const QString &, const QString &,const QString &, const QString &, const QDateTime &, bool)), m_jabber_slotsignal, SLOT(addMessageToConference(const QString &, const QString &,const QString &, const QString &, const QDateTime &, bool)));
	connect(m_conference_management_object, SIGNAL(changeOwnConferenceNickName(const QString &,const QString &, const QString &, const QString &)), m_jabber_slotsignal, SLOT(changeOwnConferenceNickName(const QString &,const QString &, const QString &, const QString &)));
	connect(m_conference_management_object, SIGNAL(createConference(const QString &, const QString &, const QString &)), m_jabber_slotsignal, SLOT(createConference(const QString &, const QString &, const QString &)));
	connect(m_jabber_roster, SIGNAL(clientVersion(const TreeModelItem &, const QString &)), m_jabber_slotsignal, SLOT(clientVersion(const TreeModelItem &, const QString &)));
	connect(m_jabber_protocol, SIGNAL(conferenceClientVersion(const QString &,const QString &,const QString &,const QString &, const QString &)), m_jabber_slotsignal, SLOT(conferenceClientVersion(const QString &,const QString &,const QString &,const QString &, const QString &)));
	connect(m_conference_management_object, SIGNAL(conferenceClientVersion(const QString &,const QString &,const QString &,const QString &, const QString &)), m_jabber_slotsignal, SLOT(conferenceClientVersion(const QString &,const QString &,const QString &,const QString &, const QString &)));
	connect(m_jabber_protocol, SIGNAL(systemNotification(const QString &, const QString &)), m_jabber_slotsignal, SLOT(systemNotification(const QString &, const QString &)));
	connect(m_jabber_protocol, SIGNAL(onFetchVCard(const QString&, const VCard*, const QString&)), m_jabber_slotsignal, SLOT(s_onFetchVCard(const QString&, const VCard*, const QString&)));
	//connect(m_jabber_protocol, SIGNAL(receiveSDisco(const JID&, const Disco::Items&, QString&)), m_jabber_slotsignal, SLOT(s_receiveSDisco(const JID&, const Disco::Items&, QString&)));
	//connect(m_jabber_protocol, SIGNAL(receiveSDiscoInfo(const JID&, const Disco::Info&, QString&)), m_jabber_slotsignal, SLOT(s_receiveSDiscoInfo(const JID&, const Disco::Info&, QString&)));
	connect(m_jabber_roster, SIGNAL(setItemIcon(const TreeModelItem &, const QString &, int)), m_jabber_slotsignal, SLOT(setClientCustomIcon(const TreeModelItem &, const QString &, int)));
	connect(m_jabber_roster, SIGNAL(setItemCustomText(const TreeModelItem &, const QString &, int)), m_jabber_slotsignal, SLOT(setClientCustomText(const TreeModelItem &, const QString &, int)));
	connect(m_conference_management_object, SIGNAL(setConferenceTopic(const QString &, const QString &, const QString &, const QString &)), m_jabber_slotsignal, SLOT(setConferenceTopic(const QString &, const QString &, const QString &, const QString &)));
	connect(m_conference_management_object, SIGNAL(addSystemMessageToConference(const QString &, const QString &, const QString &, const QString &, const QDateTime &, bool)), m_jabber_slotsignal, SLOT(addSystemMessageToConference(const QString &, const QString &, const QString &, const QString &, const QDateTime &, bool)));
	connect(m_jabber_protocol, SIGNAL(createAcceptAuthDialog(const QString &, const JID &, Client *)), m_jabber_slotsignal, SLOT(createAcceptAuthDialog(const QString &, const JID &, Client *)));
	connect(m_jabber_roster, SIGNAL(s_customNotification(const TreeModelItem &, const QString &)), m_jabber_slotsignal, SLOT(customNotification(const TreeModelItem &, const QString &)));
	connect(m_jabber_roster, SIGNAL(moveItemInContactList(TreeModelItem, TreeModelItem)), m_jabber_slotsignal, SLOT(moveItemInContactList(TreeModelItem, TreeModelItem)));
	connect(m_jabber_roster, SIGNAL(setContactItemName(TreeModelItem, QString)), m_jabber_slotsignal, SLOT(setContactItemName(TreeModelItem, QString)));
	connect(m_jabber_protocol, SIGNAL(bookmarksHandled(const BookmarkList &, const ConferenceList &)), m_jabber_slotsignal, SLOT(bookmarksHandled(const BookmarkList &, const ConferenceList &)));
	connect(m_jabber_protocol, SIGNAL(tagHandled(const QString &, bool)), m_jabber_slotsignal, SLOT(tagHandled(const QString &, bool)));
	connect(m_jabber_protocol, SIGNAL(conferenceInvite(const JID&, const JID&, const QString&, const QString&)), m_conference_management_object, SLOT(s_conferenceInvite(const JID&, const JID&, const QString&, const QString&)));
//	connect(m_jabber_roster, SIGNAL(), m_jabber_slotsignal, SLOT());

        m_jabber_roster->constr();
        m_jabber_roster->loadSettings();
        m_jabber_roster->loadRosterFromSettings();
}

jAccount::~jAccount()
{
	delete m_jabber_protocol;
	delete m_jabber_slotsignal;
	delete m_account_button;
	delete m_additional_menu;
	delete m_status_menu;
}

void jAccount::createStatusMenu()
{
	m_conference_menu = new QMenu();
	m_conference_menu->setTitle(tr("Conferences"));
	m_join_groupchat = new QAction(tr("Join groupchat"), this);
	connect(m_join_groupchat, SIGNAL(triggered()), m_conference_management_object, SLOT(joinGroupchat()));
	m_conference_menu->addAction(m_join_groupchat);

	m_additional_menu = new QMenu();
	m_additional_menu->setTitle(tr("Additional"));
	m_join_group_chat = new QAction(tr("Join groupchat"), this);
	m_join_group_chat->setVisible(false);
	m_change_resource = new QAction(tr("Change resource"), this);
	m_open_xml_console = new QAction(tr("Open XML console"), this);
	m_add_contact = new QAction(tr("Add new contact"), this);
	m_transport_browser = new QAction(tr("Transports"), this);
	m_other_browser = new QAction(tr("Other Services"), this);

	QAction *bookmark_action = new QAction(jPluginSystem::instance().getIcon("bookmarkstorage"),tr("Bookmarks"),this);
	bookmark_action->setVisible(false);
//	m_jabber_protocol->setBookmarkAction(m_bookmark_action);
//	m_bookmark_action->setEnabled(false);

	QAction *mood_action = new QAction(jPluginSystem::instance().getIcon("mood"),tr("Set mood"),this);
	mood_action->setEnabled(false);
	QAction *activity_action = new QAction(jPluginSystem::instance().getIcon("activity"),tr("Set activity"),this);
	activity_action->setEnabled(false);
	m_jabber_protocol->setActions(bookmark_action, mood_action, activity_action);

	m_join_group_chat->setIcon(QIcon(":/icons/chat"));
	m_additional_menu->addAction(m_join_group_chat);

//	m_change_resource->setIcon(QIcon(":/icons/chat"));
	connect(m_transport_browser, SIGNAL(triggered()),
			this, SLOT(showTransportsBrowser()));
	m_additional_menu->addAction(m_transport_browser);

	connect(m_other_browser, SIGNAL(triggered()),
			this, SLOT(showOtherServicesBrowser()));
	m_additional_menu->addAction(m_other_browser);

	m_additional_menu->addSeparator();

	connect(m_change_resource, SIGNAL(triggered()),
			this, SLOT(showChangeResourceDialog()));
	m_additional_menu->addAction(m_change_resource);

	connect(m_open_xml_console, SIGNAL(triggered()),
			m_jabber_slotsignal, SLOT(createXmlConsole()));
	m_additional_menu->addAction(m_open_xml_console);


	//m_add_contact->setIcon(QIcon(":/icons/servdisco"));
	connect(m_add_contact, SIGNAL(triggered()), this, SLOT(showAddDialog()));
	m_additional_menu->addAction(m_add_contact);

	onlineAction = new QAction(m_jabber_plugin_system.getStatusIcon("online", "jabber"), tr("Online"), this);
	connect(onlineAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("online", onlineAction);

	offlineAction = new QAction(m_jabber_plugin_system.getStatusIcon("offline", "jabber"), tr("Offline"), this);
	connect(offlineAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("offline", offlineAction);

	ffcAction = new QAction(m_jabber_plugin_system.getStatusIcon("ffc", "jabber"), tr("Free for chat"), this);
	connect(ffcAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("ffc", ffcAction);

	awayAction = new QAction(m_jabber_plugin_system.getStatusIcon("away", "jabber"), tr("Away"), this);
	connect(awayAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("away", awayAction);

	xaAction = new QAction(m_jabber_plugin_system.getStatusIcon("na", "jabber"), tr("NA"), this);
	connect(xaAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("na", xaAction);

	dndAction = new QAction(m_jabber_plugin_system.getStatusIcon("dnd", "jabber"), tr("DND"), this);
	connect(dndAction, SIGNAL(triggered()), this, SLOT(setStatus()));
	statusMenuActions.insert("dnd", dndAction);

	QHashIterator<QString, QAction *> iterator(statusMenuActions);
	while (iterator.hasNext())
	{
		iterator.next();
		iterator.value()->setCheckable(true);
	}

	m_status_menu = new QMenu();
	m_status_menu->setTitle(m_account_name);
	m_status_menu->setIcon(m_jabber_plugin_system.getStatusIcon("offline", "jabber"));

	m_status_menu->addAction(onlineAction);
	m_status_menu->addAction(ffcAction);
	m_status_menu->addAction(awayAction);
	m_status_menu->addAction(xaAction);
	m_status_menu->addAction(dndAction);
	m_status_menu->addSeparator();
	m_status_menu->addAction(mood_action);
	m_status_menu->addAction(activity_action);
	m_status_menu->addAction(bookmark_action);
	m_status_menu->addMenu(m_additional_menu);
	m_status_menu->addMenu(m_conference_menu);
	m_status_menu->addSeparator();
	m_status_menu->addAction(offlineAction);

	offlineAction->setChecked(true);
}

void jAccount::showChangeResourceDialog()
{
	changeResource changeResourceDialog;
	changeResourceDialog.setWindowTitle(tr("Change resource/priority on") + " " + getAccountName());
	changeResourceDialog.setResource(m_jabber_protocol->getResource());
	changeResourceDialog.setPriority(m_jabber_protocol->getPriority());
	if (changeResourceDialog.exec())
		m_jabber_protocol->setResource(changeResourceDialog.getResource(), changeResourceDialog.getPriority());
}

void jAccount::createAccountButton(QHBoxLayout *account_button_layout)
{
	m_account_button = new QToolButton();
	m_account_button->setMinimumSize(QSize(22, 22));
	m_account_button->setMaximumSize(QSize(22, 22));
	m_account_button->setAutoRaise(true);
#if defined(Q_OS_MAC)
	account_button_layout->addWidget(m_account_button, 0, Qt::AlignLeft);
#else
	account_button_layout->addWidget(m_account_button, 0, Qt::AlignRight);
#endif
//	m_account_button->setToolTip(m_account_name);
	m_account_button->setIcon(m_jabber_plugin_system.getStatusIcon("offline", "jabber"));
	m_account_button->setPopupMode(QToolButton::InstantPopup);
	m_account_button->setMenu(m_status_menu);
}

QMenu *jAccount::getAccountMenu()
{
	return m_status_menu;
}

void jAccount::setStatus()
{
	QAction *act = qobject_cast<QAction *>(sender());

	if (act == onlineAction)
	{
		m_status = Presence::Available;
	}
	else if (act == offlineAction)
	{
		m_status = Presence::Unavailable;
	}
	else if (act == ffcAction)
	{
		m_status = Presence::Chat;
	}
	else if (act == awayAction)
	{
		m_status = Presence::Away;
	}
	else if (act == xaAction)
	{
		m_status = Presence::XA;
	}
	else if (act == dndAction)
	{
		m_status = Presence::DND;
	}

	setStatusN(m_status);

}
void jAccount::setStatusN(Presence::PresenceType presence)
{
	m_status = presence;
	m_jabber_protocol->setStatus(m_status);

}

void jAccount::setRealStatus(int status)
{
	QHashIterator<QString, QAction *> iterator(statusMenuActions);
	while (iterator.hasNext())
	{
		iterator.next();
		iterator.value()->setChecked(false);
	}

	QString name = "connecting";
	if (status != -1)
	{
		Presence::PresenceType presence = static_cast<gloox::Presence::PresenceType>(status);
		name = this->getStatusName(presence);
		statusMenuActions.value(name)->setChecked(true);
	}
	m_status_icon = m_jabber_plugin_system.getStatusIcon(name, "jabber");
	m_status_menu->setIcon(m_status_icon);
	m_account_button->setIcon(m_status_icon);
	m_jabber_plugin_system.updateStatusIcons();
	m_conference_management_object->sendPresenceToAll();
}

QString jAccount::getStatusIconName(Presence::PresenceType presence)
{
	return m_jabber_plugin_system.getStatusIconFileName(jAccount::getStatusName(presence), "jabber");
}

QString jAccount::getStatusName(Presence::PresenceType presence)
{
	QString name;
	switch(presence)
	{
		case Presence::Available:
			name = "online";
			break;
		case Presence::Chat:
			name = "ffc";
			break;
		case Presence::Away:
			name = "away";
			break;
		case Presence::DND:
			name = "dnd";
			break;
		case Presence::XA:
			name = "na";
			break;
		case Presence::Probe:
		case Presence::Error:
		case Presence::Invalid:
		case Presence::Unavailable:
			name = "offline";
			break;
	}
	return name;
}

int jAccount::getStatusMass(Presence::PresenceType presence)
{
	int mass;
	switch(presence)
	{
		case Presence::Available:
			mass = 10;
			break;
		case Presence::Chat:
			mass = 20;
			break;
		case Presence::Away:
			mass = 30;
			break;
		case Presence::DND:
			mass = 50;
			break;
		case Presence::XA:
			mass = 40;
			break;
		case Presence::Probe:
		case Presence::Error:
		case Presence::Invalid:
		case Presence::Unavailable:
			mass = 1000;
			break;
		default:
			mass = 0;
	}
	return mass;
}

void jAccount::showVCardWidget(const QString &jid_base) {

/*	QString jid = jid_base;
	jid.remove(QRegExp("/.*"));
	if (m_conference_management_object->JIDIsRoom(jid))
		jid = jid_base;
	if (m_vcard_list.contains(jid))
		return;
	bool mode;
	if (m_account_name == jid) mode = true;
	else mode = false;
	jVCard *vcard = new jVCard(jid, mode);
	m_vcard_list.insert(jid, vcard);
	m_vcard_list.value(jid)->setWindowIcon(m_jabber_plugin_system.getIcon("vcard"));
	m_vcard_list.value(jid)->setWindowTitle(jid);
	connect(m_vcard_list.value(jid), SIGNAL(requestVCard(const QString&)), this, SLOT(s_requestVCard(const QString&)));
	connect(m_vcard_list.value(jid), SIGNAL(saveVCard(VCard*)), this, SLOT(s_saveVCard(VCard*)));
	connect(m_vcard_list.value(jid), SIGNAL(closeVCard(const QString&)), this, SLOT(s_closeVCard(const QString&)));
	m_vcard_list.value(jid)->fillData();
	m_vcard_list.value(jid)->show();
*/
}

void jAccount::s_requestVCard(const QString &jid)
{
	m_jabber_protocol->fetchVCard(jid);
}

void jAccount::setVCardInfo(const VCard *vcard, const QString &jid_base, const QString &avatarUrl)
{
/*
	QString jid = jid_base;
	jid.remove(QRegExp("/.*"));
	if (m_conference_management_object->JIDIsRoom(jid))
		jid = jid_base;
	if(m_vcard_list.contains(jid))
		m_vcard_list.value(jid)->setVCard(vcard, avatarUrl);
	else
		qDebug() << "unknown vCard" << jid;
*/
}

void jAccount::s_saveVCard(VCard *vcard)
{
	QString hex = "";
	const VCard::Photo &photo = vcard->photo();
	if(!photo.binval.empty())
	{
		QByteArray data(photo.binval.c_str(),photo.binval.length());
		SHA sha;
		sha.feed(photo.binval);
		sha.finalize();
		hex = jProtocol::fromStd(sha.hex());
	}
	m_jabber_protocol->updateAvatarPresence(hex);
	m_jabber_protocol->storeVCard(vcard);
}

void jAccount::s_closeVCard(const QString &jid)
{
	m_vcard_list.remove(jid);
}

void jAccount::getDiskoItem(const QString &server, const QString &node, DiscoHandler *handler)
{
	m_jabber_protocol->getDiscoItems(server, node, handler);
}

void jAccount::getDiskoInfo(const QString &server, const QString &node, DiscoHandler *handler)
{
	m_jabber_protocol->getDiscoInfo(server, node, handler);
}

void jAccount::setConferences()
{
	m_conference_management_object->setConferences();
}

void jAccount::showAddDialog(const QString &jid, const QString &nick)
{
	QStringList groups;
	groups = m_jabber_roster->getGroups();
        groups.removeAll(tr("Services"));
        jAddContact *jadduser = new jAddContact(groups, jid, nick);
	jPluginSystem plugin = jPluginSystem::instance();
	jadduser->setIcons(plugin.getIcon("search"), plugin.getIcon("contactinfo"), plugin.getIcon("apply"));
	jadduser->setWindowIcon(plugin.getIcon("add_user"));
	jadduser->setWindowTitle(tr("Add new contact on") + " " + getAccountName());
	connect(jadduser, SIGNAL(showInformation(const QString&)), this, SLOT(showInformation(const QString&)));
	connect(jadduser, SIGNAL(addContact(const QString&, const QString&, const QString&, bool)), this, SLOT(addContact(const QString&, const QString&, const QString&, bool)));
	jadduser->show();
}

void jAccount::addContact(const QString &jid, const QString &name, const QString &group, bool authorize)
{
	m_jabber_protocol->addContact(jid, name, group, authorize);
}

void  jAccount::s_requestDiscoItems(const QString &server, const QString &node)
{
}

void jAccount::showTransportsBrowser()
{
	jSearchConference *service_browser = new jSearchConference("gateway", this);
	service_browser->show();
}

void jAccount::showOtherServicesBrowser()
{
	jServiceBrowser *service_browser = new jServiceBrowser(this, OTHER_SERVICES_BROWSER);
	service_browser->show();
}

