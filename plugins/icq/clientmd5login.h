/*
    clientMd5Login

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


#ifndef CLIENTMD5LOGIN_H_
#define CLIENTMD5LOGIN_H_

#include "tlv.h" 

class clientMd5Login
{
public:
	clientMd5Login();
	~clientMd5Login();
	void setScreenName(const QString &account) { screenName.setData(account); }
	void setPassword(const QString &, const QString &);
	void sendPacket(QTcpSocket *, quint16, quint32 );
private:
	QByteArray getBytePacket() const;
	QByteArray flapLength() const;
	QByteArray convertToByteArray(const quint16 &) const;
	QByteArray convertToByteArray(const quint32 &) const;
	
	tlv screenName;
	tlv password;
	tlv clientName;
	tlv clientID;
	tlv clientMajor;
	tlv clientMinor;
	tlv clientLesser;
	tlv clientBuild;
	tlv distributionNumber;
	tlv clientLanguage;
	tlv clientCountry;
};

#endif /*CLIENTMD5LOGIN_H_*/
