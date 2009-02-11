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

#ifdef _MSC_VER
#pragma warning(disable:4309)
#endif

#include <QTcpSocket>

#include "clientIdentification.h"
#include "oscarprotocol.h"

// const quint16 sequences[] = {
//     0x04E7, 0x0679, 0x0704, 0x078A, 0x078A, 0x07C9, 0x0B25, 0x0FC5,
//     0x163F, 0x1BEA, 0x2294, 0x2493, 0x25D6, 0x25FA, 0x26EE, 0x2886,
//     0x30CC, 0x3303, 0x3526, 0x3C26, 0x42A8, 0x43DC, 0x4E94, 0x5342,
//     0x5697, 0x5A09, 0x5A09, 0x5C2B, 0x5C44, 0x5C44, 0x5D6E, 0x7339,
//     0x7570, 0x75A8, 0x7A02, 0x7F33
// };
// const quint32 sequences_num = 28;


clientIdentification::clientIdentification()
{
	screenName.setType(0x0001);
	password.setType(0x0002);
	clientName.setType(0x0003);
	clientName.setData(QString("ICQ Client"));
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

clientIdentification::~clientIdentification()
{
	
}

void clientIdentification::setPassword(const QString &pass)
{
	const char roastArray[] = { 0xF3, 0x26, 0x81, 0xC4, 0x39,
			0x86, 0xDB, 0x92, 0x71, 0xA3, 0xB9, 0xE6, 0x53, 0x7A, 0x95, 0x7C};
	quint8 length = pass.length() > 16 ? 16: pass.length();
	QByteArray roastedPass;
	for ( int i = 0; i < length; i++ )
		roastedPass[i] = pass.at(i).unicode() ^ roastArray[i];
	password.setData(roastedPass);
}

QByteArray clientIdentification::flapLength()
{
	quint16 l = 4;
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

QByteArray clientIdentification::getSeqNumber() const
{
        QByteArray seq;
	//quint16 num = sequences[rand() % sequences_num];
	//quint16 num = oscarProtocol::secnumGenerator();
	quint16 num = 0;
	seq[0] = num / 0x100;
	seq[1] = num % 0x100;
	return seq;
}

QByteArray clientIdentification::getBytePacket() const
{
	QByteArray packet;
	packet.append(protocolVersion);
	packet.append(screenName.getData());
	packet.append(password.getData());
        packet.append(clientName.getData());
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

void clientIdentification::sendPacket(QTcpSocket *socket)
{
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x01;
	packet.append(getSeqNumber());
	packet.append(flapLength());
	packet.append(getBytePacket());
	socket->write(packet);
	
}
