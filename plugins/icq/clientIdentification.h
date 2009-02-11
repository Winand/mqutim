/*
    clientIdentification

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


#ifndef CLIENTIDENTIFICATION_H_
#define CLIENTIDENTIFICATION_H_

#include "tlv.h"

// extern const quint16 sequences[];
// extern const quint32 sequences_num;

class clientIdentification
{
public:
	clientIdentification();
	~clientIdentification();
	void setProcolVersion(const QByteArray &protocol) { protocolVersion = protocol; }
	void setScreenName(const QString &account) { screenName.setData(account); }
	void setPassword(const QString &);
	void setClientName(const QString &client) { clientName.setData(client); }
	void sendPacket(QTcpSocket *);
private:
	QByteArray flapLength();
	QByteArray getSeqNumber() const;
	QByteArray getBytePacket() const;
	
	QByteArray protocolVersion;
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

#endif /*CLIENTIDENTIFICATION_H_*/
