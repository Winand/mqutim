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

#include <QCryptographicHash>
#include <QTcpSocket>

#include "snac.h"

#include "clientmd5login.h"

clientMd5Login::clientMd5Login()
{
	screenName.setType(0x0001);
	password.setType(0x0025);
	clientName.setData(QString("ICQ Client"));
	clientName.setType(0x0003);
	clientID.setType(0x0016);
	clientID.setData((quint16)266);
	clientMajor.setType(0x0017);
	clientMajor.setData((quint16)7);
	clientMinor.setType(0x0018);
	clientMinor.setData((quint16)2);
	clientLesser.setType(0x0019);
	clientLesser.setData((quint16)1);
	clientBuild.setType(0x001A);
	clientBuild.setData((quint16)3916);
	distributionNumber.setType(0x0014);
	distributionNumber.setData((quint32)85);
	clientLanguage.setType(0x000F);
	clientLanguage.setData(QString("en"));
	clientCountry.setType(0x000E);
	clientCountry.setData(QString("us"));
}

clientMd5Login::~clientMd5Login()
{
	
}

void clientMd5Login::setPassword(const QString &pass, const QString &authKey)
{
	QByteArray hashPassword;
	hashPassword.append(pass);
	QByteArray authkey;
	authkey.append(authKey);
	QByteArray AIM_MD5_STRING("AOL Instant Messenger (SM)");
	QCryptographicHash passHash(QCryptographicHash::Md5);
	passHash.addData(authkey);
	passHash.addData(hashPassword);
	passHash.addData(AIM_MD5_STRING);
	password.setData(passHash.result());	
}

void clientMd5Login::sendPacket(QTcpSocket *socket, quint16 flapSeq, quint32 snacSeq)
{
		QByteArray packet;
		packet[0] = 0x2A;
		packet[1] = 0x02;
		packet.append(convertToByteArray(flapSeq));
		packet.append(flapLength());
		packet.append(convertToByteArray((quint16)0x0017));
		packet.append(convertToByteArray((quint16)0x0002));
		packet.append(convertToByteArray((quint16)0x0000));
		packet.append(convertToByteArray(snacSeq));
		packet.append(getBytePacket());
		socket->write(packet);
}

QByteArray clientMd5Login::flapLength() const
{
		quint16 l = 10;
		l += screenName.getLength();
		l += password.getLength();
		l += clientName.getLength();
		l += clientID.getLength();
		l += clientMajor.getLength();
		l += clientMinor.getLength();
		l += clientLesser.getLength();
		l += clientBuild.getLength();
		l += distributionNumber.getLength();
		l += clientLanguage.getLength();
		l += clientCountry.getLength();
		QByteArray packetLength;
		packetLength[0] = l / 0x100;
		packetLength[1] = l % 0x100;
		return packetLength;
}

QByteArray clientMd5Login::getBytePacket() const
{
		QByteArray packet;
		packet.append(screenName.getData());
		packet.append(clientName.getData());
                packet.append(password.getData());
		packet.append(clientMajor.getData());
		packet.append(clientMinor.getData());
		packet.append(clientLesser.getData());
		packet.append(clientBuild.getData());
                packet.append(clientID.getData());
		packet.append(distributionNumber.getData());
		packet.append(clientLanguage.getData());
		packet.append(clientCountry.getData());
		return packet;
}

QByteArray clientMd5Login::convertToByteArray(const quint16 &d) const
{
	QByteArray packet;
		packet[0] = (d / 0x100);
		packet[1] = (d % 0x100);
		return packet;
}

QByteArray clientMd5Login::convertToByteArray(const quint32 &d) const
{
	QByteArray packet;
		packet[0] = (d / 0x1000000);
		packet[1] = (d / 0x10000);
		packet[2] = (d / 0x100);
		packet[3] = (d % 0x100);
		return packet;
}
