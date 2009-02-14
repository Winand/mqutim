/*
    treeBuddyItem

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/


#ifndef TREEBUDDYITEM_H_
#define TREEBUDDYITEM_H_

#include <QTreeWidgetItem>

#include "statusiconsclass.h"
#include "quticqglobals.h"
#include "icqpluginsystem.h"

class messageFormat;
class contactListTree;	
class icqBuffer;
class tlv;

class treeBuddyItem
{
public:
	treeBuddyItem(const QString &, const QString &profile_name);
	~treeBuddyItem();

	void checkForXStatus();
	void readData(icqBuffer *, quint16);
	void oncoming(icqBuffer *, quint16);
	void buddyOffline();
	void setBuddyUin(const QString &);
	quint16 groupID;
	QString groupName;
	quint16 itemId;
	bool isOffline;
	inline contactStatus getStatus() const { return status; };
	inline QString getName() const { return buddyName; };
	inline QString getUin() const { return buddyUin; };
	void setName(const QString& n);
	QList<messageFormat *> messageList;
	statusIconClass::getIconFunction statusIconMethod;
	bool messageIcon;
	void readMessage();
	contactListTree *par;
	bool UTF8;
	bool statusChanged;
	void updateBuddyText();
	
	bool operator< ( const QTreeWidgetItem  &other ) const  ;
	bool underline;

	bool birth;
	QDate birthDay;
	
	quint8 picFlags;
	
	
	
	void waitingForAuth(bool);
	bool authorizeMe;
	QString authMessage;
	
	QList<QByteArray> capabilitiesList;
	
	QString clientId;
	quint32 externalIP;
	quint32 internalIP;
	quint32 onlineTime;
	quint32 signonTime;
	quint32 lastonlineTime;
	quint32 regTime;
	quint32 idleSinceTime;
	
/*	bool fileTransferSupport;*/
	
	bool xStatusPresent;
	
	bool icqLite;
	QByteArray xStatusMessage;
	QString xStatusIcon;

	


	quint8 protocolVersion;
	quint32 userPort;
	quint32 Cookie;
	quint32 lastInfoUpdate;
	quint32 lastExtInfoUpdate;
	quint32 lastExtStatusInfoUpdate;
	
	QList<quint16> shortCaps;
	
	inline QString getParentUin() const { return parentUin; };
	void setMessageIcon(bool) {};
	void setAvatarHash(const QByteArray &hash);
	QByteArray getAvatarHash() const {return avatarMd5Hash;} 
	void setNotAuthorizated(bool authflag);
	bool getNotAutho() {return notAutho;}
	void setClientIcon(const QIcon &client_icon);
	bool m_show_xstatus_icon;
	bool m_show_birthday_icon;
	bool m_show_auth_icon;
	bool m_show_vis_icon;
	bool m_show_invis_icon;
	bool m_show_ignore_icon;
	bool m_show_xstatus_text;
	bool m_visible_contact;
	bool m_invisible_contact;
	bool m_ignore_contact;
	void updateIcons();
	void setCustomIcon(const QIcon &icon, int position);
	bool m_xstatus_already_readed;
	void setXstatusText();
	QString createToolTip();
	bool m_xstatus_changed;
	QString xStatusCaption;
	QString xStatusMsg;
	void setXstatusCaptionAndMessage(const QString &caption,
			const QString &message);
	bool m_channel_two_support;
private:

	bool notAutho;
	QByteArray avatarMd5Hash;
	void setCapabilities(QByteArray);
	bool isUtf8Cap(const QByteArray &);
	void takeOncomingTlv(tlv &);
	void takeTlv(tlv &);
	quint32 convertToInt32(const QByteArray &) const;
	quint16 byteArrayToInt16(const QByteArray &) const;
	quint8 byteArrayToInt8(const QByteArray &) const;
	void changeStatus(const QByteArray &);
	QString buddyUin;
	QString buddyName;
	contactStatus status;
	void readAvailableMessTlv(QByteArray);

	QString iconPath;
	QString parentUin;

	void setExtIp(const QByteArray &);
	void setIntIp(const QByteArray &);
	void setOnlTime(const QByteArray &);
	void setSignOn(const QByteArray &);
	void setLastOnl();
	void setregTime(const QByteArray &);
	void readShortCap(quint16 ,const QByteArray &);
	void setIdleSinceTime(quint16, const QByteArray &);
	QString statToStr(contactStatus st);
	QString m_profile_name;
	IcqPluginSystem &m_icq_plugin_system;
	void setContactStatus(const QIcon &statuc_icon, const QString &status_name,
			int mass);
	void setContactXStatus(const QIcon &xstatus_icon);

	void setTextToRow(const QString &text, int position);
	void clearRow(int position);
	void setBirthdayIcon();
};

#endif /*TREEBUDDYITEM_H_*/
