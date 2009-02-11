/*
    servicesSetup

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


#ifndef SERVICESSETUP_H_
#define SERVICESSETUP_H_

#include <QString>
#include "quticqglobals.h"
#include "icqpluginsystem.h"


class QTcpSocket;

class servicesSetup
{
public:
	servicesSetup(const QString &, const QString &profile_name);
	~servicesSetup();
	quint16 flap0202seq;
	quint32 snac0202seq;
	quint16 flap0204seq;
	quint32 snac0204seq;
	quint16 flap0302seq;
	quint32 snac0302seq;
	quint16 flap0404seq;
	quint32 snac0404seq;
	quint16 flap0402seq;
	quint32 snac0402seq;
	quint16 flap0402seq02;
	quint32 snac0402seq02;
	quint16 flap0902seq;
	quint32 snac0902seq;
	quint16 flap1302seq;
	quint32 snac1302seq;
	quint16 flap1305seq;
	quint32 snac1305seq;
	quint16 flap1307seq;
	quint32 snac1307seq;
	quint16 flap1309seq;
	quint32 snac1309seq;
	quint16 flap011eseq;
	quint32 snac011eseq;
	quint16 flap0102seq;
	quint32 snac0102seq;
	quint16 flap1502seq;
	quint32 snac1502seq;
	quint16 req1502seq;
	quint32 uin;
	void sendData(QTcpSocket *, const QString &);
	void answerToList(QTcpSocket *);
	void setStatus(accountStatus);
	void changeStatus(accountStatus, QTcpSocket *, const QString &);
	void setPrivacy(const QString &, quint16, quint16, QTcpSocket *);
	void sendCapabilities(QTcpSocket *);
	void sendXStatusAsAvailableMessage(QTcpSocket *);
	

private:
	QString icqMood;
	QByteArray convertToByteArray(const quint8 &);
	QByteArray convertToByteArray(const quint16 &);
	QByteArray convertToByteArray(const quint32 &);
	QByteArray get0202();
	QByteArray get0204();
	QByteArray get0302();
	QByteArray get0404();
	QByteArray get0402();
	QByteArray get0902();
	QByteArray get1302();
	QByteArray get1307();
	QByteArray get1305();
	QByteArray get011e(const QString &);
	QByteArray get0102();
	QByteArray get1502();
	QByteArray utf8Cap();
	QByteArray buddyIconCap();
	QByteArray rtfMessages();
	QByteArray serverRelaying();
	
	quint16 currentStatus;
	
	QByteArray getClientIdentification();
	QString icqUin;
	QByteArray icq6Capab();
	QByteArray icq51Capab();
	QByteArray icq5Capab();
	QByteArray icq4Capab();
	QByteArray icq2003bCapab();
	QByteArray icq2002Capab();
	QByteArray icqMacCapab();
	QByteArray icqQip2005Capab();
	QByteArray icqQipInfCapab();
	QByteArray qutimCapab();
	QByteArray getProtocolVersion(unsigned,unsigned);
	QByteArray getXstatusCap(int);
	QString m_profile_name;
};

#endif /*SERVICESSETUP_H_*/
