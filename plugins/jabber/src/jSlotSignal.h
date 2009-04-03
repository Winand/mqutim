/*
    jSlotSignal.h

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

#ifndef JSLOTSIGNAL_H
#define JSLOTSIGNAL_H

#include "jAccount.h"
#include "xmlconsole.h"
#include <QDateTime>

class jAccount;
//class jBookmark;

class jSlotSignal : public QObject
{
	Q_OBJECT
public:
	jSlotSignal(jAccount *parent = 0);
	~jSlotSignal();

public slots:
	void addItemToContactList(const TreeModelItem &item, const QString &item_name);
	void removeItemFromContactList(TreeModelItem Item);
	void setContactItemStatus(const TreeModelItem &item, const QString &status, int mass);
	void setClientCustomIcon(const TreeModelItem &, const QString &, int);
	void setClientCustomText(const TreeModelItem &, const QString &, int);
	void addMessageFrom(const TreeModelItem &item, const QDateTime &date, const QString &text);
	void addServiceMessage(const TreeModelItem &item, const QString &text);
	void setItemInvisible(const TreeModelItem &item, bool invisible);
	void messageDelievered(const TreeModelItem &item, int position);
	void sendTypingNotification(const TreeModelItem &item, bool composing);
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
	void clientVersion(const TreeModelItem &contact, const QString &name);
	void systemNotification(const QString &account_name, const QString &message);
	void s_onFetchVCard(const QString &jid, const VCard *vcard, const QString &avatarUrl);
	//void s_receiveSDisco(const JID &from, const Disco::Items &items, QString &type);
	//void s_receiveSDiscoInfo(const JID &from, const Disco::Info &info, QString &type);
	void changeOwnConferenceNickName(const QString &protocol_name,
			const QString &conference_name, const QString &account_name, const QString &nickname);
	void createConference(const QString &protocol_name, const QString &conference_name, const QString &account_name);
	void setConferenceItemIcon(const QString &protocol_name, const QString &conference_name, const QString &account_name,
			const QString &nickname, const QString &icon_name, int position);
	void setConferenceTopic(const QString &protocol_name, const QString &conference_name,
			const QString &account_name, const QString &topic);
	void addSystemMessageToConference(const QString &protocol_name, const QString &conference_name,
			const QString &account_name, const QString &message, const QDateTime &date, bool history);
	void createAcceptAuthDialog(const QString &text, const JID &jid, Client *jabber_client);
	void customNotification(const TreeModelItem &item, const QString &message);
	void moveItemInContactList(const TreeModelItem &OldItem, const TreeModelItem &NewItem);
	void setContactItemName(const TreeModelItem &Item, const QString &name);
	void bookmarksHandled(const BookmarkList &b_list, const ConferenceList &c_list);
	void tagHandled(const QString &xml, bool in);
	void createXmlConsole();
	void destroyXmlConsole();
	void createBookmark();
	void destroyBookmark();

private:
	jAccount *m_jabber_account;
	XmlConsole *m_xml_console;
//	jBookmark *m_bookmark_settings;
	BookmarkList m_b_list;
	ConferenceList m_c_list;
	bool isConnected;
};

#endif
