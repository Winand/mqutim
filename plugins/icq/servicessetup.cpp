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

//#include <QtGui>
#include <QTcpSocket>
#include <QSettings>
#include <QTime>

#include "tlv.h"
#include "snac.h"

#include "servicessetup.h"

servicesSetup::servicesSetup(const QString &u, const QString &profile_name) 
: icqUin(u)
, m_profile_name(profile_name)
{
	flap0202seq = 0;
	snac0202seq = 0;
	flap0204seq = 0;
	snac0204seq = 0;
	flap0302seq = 0;
	snac0302seq = 0;
	flap0404seq = 0;
	snac0404seq = 0;
	flap0402seq = 0;
	snac0402seq = 0;
	flap0902seq = 0;
	snac0902seq = 0;
	flap1307seq = 0;
	snac1307seq = 0;
	flap1305seq = 0;
	snac1305seq = 0;
	flap011eseq = 0;
	snac011eseq = 0;
	flap0102seq = 0;
	snac0102seq = 0;

}

servicesSetup::~servicesSetup()
{
	
}

QByteArray servicesSetup::convertToByteArray(const quint8 &d)
{
	QByteArray packet;
	packet[0] = d;
	return packet;
}

QByteArray servicesSetup::convertToByteArray(const quint16 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x100);
	packet[1] = (d % 0x100);
	return packet;
}

QByteArray servicesSetup::convertToByteArray(const quint32 & d)
{
	QByteArray packet;
	packet[0] = (d / 0x1000000);
	packet[1] = (d / 0x10000);
	packet[2] = (d / 0x100);
	packet[3] = (d % 0x100);
	return packet;
}

void servicesSetup::sendData(QTcpSocket *socket, const QString &uin)
{
	QByteArray packet;
	packet.append(get011e(uin));
	packet.append(get0202());
	packet.append(get0204());
	packet.append(get0302());
	packet.append(get0404());
	packet.append(get0402());
	packet.append(get0902());
	packet.append(get1302());
	packet.append(get1305());	
	socket->write(packet);
}

void servicesSetup::answerToList(QTcpSocket *socket)
{
	QByteArray packet;
	packet.append(get1307());
	packet.append(get0102());
	packet.append(get1502());
	socket->write(packet);
}

QByteArray servicesSetup::get0204()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin, "accountsettings");
	snac snac0204;
	snac0204.setFamily(0x0002);
	snac0204.setSubType(0x0004);
	snac0204.setReqId(snac0204seq);
	tlv capabilities;
	capabilities.setType(0x0005);
	QByteArray capList;
	capList.append(utf8Cap());
	capList.append(serverRelaying());
	capList.append(buddyIconCap());

	capList.append(getClientIdentification());
	int xstatusIndex = settings.value("xstatus/index", 0).toInt();
	
	if ( xstatusIndex )
		capList.append(getXstatusCap(xstatusIndex - 1));
	
//	capList.append(rtfMessages());
	capabilities.setData(capList);
	quint16 length  = 10 + capabilities.getLength();
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap0204seq));
	packet.append(convertToByteArray(length));
	packet.append(snac0204.getData());
	packet.append(capabilities.getData());
	return packet;
	
}

QByteArray servicesSetup::utf8Cap()
{
	//ICQ UTF8 Support {0946134e-4c7f-11d1-8222-444553540000}
	QByteArray packet;
	packet.append(convertToByteArray((quint32)0x0946134e));
	packet.append(convertToByteArray((quint32)0x4c7f11d1));
	packet.append(convertToByteArray((quint32)0x82224445));
	packet.append(convertToByteArray((quint32)0x53540000));
	return packet;
}

QByteArray servicesSetup::buddyIconCap()
{
	//Buddy Icon {09461346-4c7f-11d1-8222-444553540000}
	QByteArray packet;
	packet.append(convertToByteArray((quint32)0x09461346));
	packet.append(convertToByteArray((quint32)0x4c7f11d1));
	packet.append(convertToByteArray((quint32)0x82224445));
	packet.append(convertToByteArray((quint32)0x53540000));
	return packet;
}

QByteArray servicesSetup::rtfMessages()
{
	//RTF messages {97B12751-243C-4334-AD22-D6ABF73F1492}
	QByteArray packet;
	packet.append(convertToByteArray((quint32)0x97B12751));
	packet.append(convertToByteArray((quint32)0x243C4334));
	packet.append(convertToByteArray((quint32)0xAD22D6AB));
	packet.append(convertToByteArray((quint32)0xF73F1492));
	return packet;	
}

QByteArray servicesSetup::serverRelaying()
{
	//ICQ Server relaying {09461349-4C7F-11D1-8222-444553540000}
	QByteArray packet;
	packet.append(convertToByteArray((quint32)0x09461349));
	packet.append(convertToByteArray((quint32)0x4C7F11D1));
	packet.append(convertToByteArray((quint32)0x82224445));
	packet.append(convertToByteArray((quint32)0x53540000));
	return packet;
}

QByteArray servicesSetup::get0402()
{
	snac snac0402;
	snac0402.setFamily(0x0004);
	snac0402.setSubType(0x0002);
	snac0402.setReqId(snac0402seq);
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap0402seq));
	packet.append(convertToByteArray((quint16)26));
	packet.append(snac0402.getData());
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint32)0x000000b));
	packet.append(convertToByteArray((quint16)0x1f40));
	packet.append(convertToByteArray((quint16)0x03e7));
	packet.append(convertToByteArray((quint16)0x03e7));
	packet.append(convertToByteArray((quint16)0x0000));
	packet.append(convertToByteArray((quint16)0x0000));
	
	snac0402.setReqId(snac0402seq02);
	QByteArray packet02;
	packet02[0] = 0x2A;
	packet02[1] = 0x02;
	packet02.append(convertToByteArray(flap0402seq02));
	packet02.append(convertToByteArray((quint16)26));
	packet02.append(snac0402.getData());
	packet02.append(convertToByteArray((quint16)0x0002));
	packet02.append(convertToByteArray((quint32)0x0000003));
	packet02.append(convertToByteArray((quint16)0x1f40));
	packet02.append(convertToByteArray((quint16)0x03e7));
	packet02.append(convertToByteArray((quint16)0x03e7));
	packet02.append(convertToByteArray((quint16)0x0000));
	packet02.append(convertToByteArray((quint16)0x0000));
	
	packet.append(packet02);
	
	return packet;
}

QByteArray servicesSetup::get1307()
{
		snac snac1307;
		snac1307.setFamily(0x0013);
		snac1307.setSubType(0x0007);
		snac1307.setReqId(snac1307seq);
		QByteArray packet;
		packet[0] = 0x2A;
		packet[1] = 0x02;
		packet.append(convertToByteArray(flap1307seq));
		packet.append(convertToByteArray((quint16)10));
		packet.append(snac1307.getData());
		return packet;	
}

QByteArray servicesSetup::get011e(const QString &account)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
        QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin, "accountsettings");
	quint32 webaware = 0x00000000;
	if ( settings.value("statuses/webaware", false).toBool() )
		webaware = 0x00010000;
	quint32 userFlags = 0x11000000 + webaware;
	if ( account_settings.value("xstatus/birth", false).toBool() )
	{
		userFlags ^= 0x00080000;
	}
	snac snac011e;
	snac011e.setFamily(0x0001);
	snac011e.setSubType(0x001e);
	snac011e.setReqId(snac011eseq);
	tlv userStatus;
	userStatus.setType(0x0006);
	userStatus.setData((quint32)( userFlags + currentStatus));
	
	unsigned clientIdentification = settings.value("clientid/index", 0).toUInt();
	unsigned port = settings.value("clientid/protocol", 1).toUInt();
	tlv unknownTlv;
	unknownTlv.setType(0x0008);
	unknownTlv.setData((quint16)0x0000);
	
	tlv dcInfo;
	dcInfo.setType(0x000c);
	
	dcInfo.setData(getProtocolVersion(clientIdentification, port));
	
	

	
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap011eseq));
//	if ( clientIdentification )
		packet.append(convertToByteArray((quint16)(18 + unknownTlv.getLength() + dcInfo.getLength())));
//	else
//		packet.append(convertToByteArray((quint16)18));
	packet.append(snac011e.getData());
	packet.append(userStatus.getData());
	
	
//	if ( clientIdentification )
//	{
		packet.append(unknownTlv.getData());
		packet.append(dcInfo.getData());

//	}
	return packet;	
}

QByteArray servicesSetup::get0102()
{
	snac snac0102;
	snac0102.setFamily(0x0001);
	snac0102.setSubType(0x0002);
	snac0102.setReqId(snac0102seq);
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap0102seq));
	packet.append(convertToByteArray((quint16)82));
	packet.append(snac0102.getData());
	
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint16)0x0003));
	packet.append(convertToByteArray((quint32)0x0110047b));
	
	packet.append(convertToByteArray((quint16)0x0002));
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint32)0x0101047b));
		
	packet.append(convertToByteArray((quint16)0x0003));
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint32)0x0110047b));
	
	packet.append(convertToByteArray((quint16)0x0004));
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint32)0x0110047b));
	
	packet.append(convertToByteArray((quint16)0x0006));
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint32)0x0110047b));
	
	packet.append(convertToByteArray((quint16)0x0008));
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint32)0x0110047b));
	
	packet.append(convertToByteArray((quint16)0x0009));
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint32)0x0110047b));
	
	packet.append(convertToByteArray((quint16)0x000A));
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint32)0x0110047b));
	
	packet.append(convertToByteArray((quint16)0x0013));
	packet.append(convertToByteArray((quint16)0x0002));
	packet.append(convertToByteArray((quint32)0x0110047b));
	
	return packet;
}

QByteArray servicesSetup::get1305()
{
	snac snac1305;
	snac1305.setFamily(0x0013);
	snac1305.setSubType(0x0005);
	snac1305.setReqId(snac1305seq);
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap1305seq));
	packet.append(convertToByteArray((quint16)16));
	packet.append(snac1305.getData());
	packet.append(convertToByteArray((quint32)0x00000000));
	packet.append(convertToByteArray((quint16)0x0000));
	return packet;
}

QByteArray servicesSetup::get1302()
{
	snac snac1302;
	snac1302.setFamily(0x0013);
	snac1302.setSubType(0x0002);
	snac1302.setReqId(snac1302seq);
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap1302seq));
	packet.append(convertToByteArray((quint16)10));
	packet.append(snac1302.getData());
	return packet;
}

QByteArray servicesSetup::get0202()
{
	snac snac0202;
	snac0202.setFamily(0x0002);
	snac0202.setSubType(0x0002);
	snac0202.setReqId(snac0202seq);
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap0202seq));
	packet.append(convertToByteArray((quint16)10));
	packet.append(snac0202.getData());
	return packet;
}

QByteArray servicesSetup::get0302()
{
	snac snac0302;
	snac0302.setFamily(0x0003);
	snac0302.setSubType(0x0002);
	snac0302.setReqId(snac0302seq);
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap0302seq));
	packet.append(convertToByteArray((quint16)10));
	packet.append(snac0302.getData());
	return packet;
}

QByteArray servicesSetup::get0404()
{
	snac snac0404;
	snac0404.setFamily(0x0004);
	snac0404.setSubType(0x0004);
	snac0404.setReqId(snac0404seq);
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap0404seq));
	packet.append(convertToByteArray((quint16)10));
	packet.append(snac0404.getData());
	return packet;
}

QByteArray servicesSetup::get0902()
{
	snac snac0902;
	snac0902.setFamily(0x0009);
	snac0902.setSubType(0x0002);
	snac0902.setReqId(snac0902seq);
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap0902seq));
	packet.append(convertToByteArray((quint16)10));
	packet.append(snac0902.getData());
	return packet;
}

QByteArray servicesSetup::get1502()
{
	snac snac1502;
	snac1502.setFamily(0x0015);
	snac1502.setSubType(0x0002);
	snac1502.setReqId(snac1502seq);
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap1502seq));
	packet.append(convertToByteArray((quint16)24));
	packet.append(snac1502.getData());
	tlv metaData;
	metaData.setType(0x0001);
	QByteArray mData;
	mData.append(convertToByteArray((quint16)0x0800));
	mData.append(uin % 0x100);
	mData.append(uin % 0x10000 / 0x100);
	mData.append(uin % 0x1000000 / 0x10000);
	mData.append(uin / 0x1000000);
	mData.append(convertToByteArray((quint16)0x3c00));
	mData.append(convertToByteArray((quint16)req1502seq));
	metaData.setData(mData);
	packet.append(metaData.getData());
	return packet;
}


void servicesSetup::setStatus(accountStatus status)
{
	switch ( status )
	{
	case online:
		currentStatus =  0x0000;
		break;
	case ffc:
		currentStatus =  0x0020;
		break;
	case away:
		currentStatus = 0x0001;
		break;
	case na:
		currentStatus = 0x0004;
		break;
	case occupied:
		currentStatus =  0x0010;
		break;
	case dnd:
		currentStatus = 0x0002;
		break;
	case invisible:
		currentStatus = 0x0100;
		break;
		
	case lunch:
		currentStatus = 0x2001;
		break;
	case evil:
		currentStatus = 0x3000;
		break;
	case depression:
		currentStatus = 0x4000;
		break;
	case athome:
		currentStatus = 0x5000;
		break;
	case atwork:
		currentStatus = 0x6000;
		break;
		
	default:
		currentStatus = 0x0000;
	}
}

void servicesSetup::changeStatus(accountStatus s, QTcpSocket *socket, const QString &uin)
{
	setStatus(s);
	socket->write(get011e(uin));
}

void servicesSetup::setPrivacy(const QString &uin, quint16 pdInfoId, quint16 pdInfoGroupId, QTcpSocket *tcpSocket)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin, "accountsettings");
	quint32 privacyType = settings.value("statuses/privacy", 4).toUInt();
	quint8 privacy;
	switch( privacyType )
	{
	case 1:
		privacy = 0x01;
		break;
	case 2:
		privacy = 0x03;
		break;
	case 3:
		privacy = 0x04;
		break;
	case 4:
		privacy = 0x05;
		break;
	case 5:
		privacy = 0x02;
		break;
	default:
		privacy = 0x05;
	}
	
	snac snac1309;
	snac1309.setFamily(0x0013);
	snac1309.setSubType(0x0009);
	snac1309.setReqId(snac1309seq);
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flap1309seq));
	quint16 length = 20;
	tlv tlv00ca;
	tlv00ca.setType(0x00ca);
	tlv00ca.setData((quint8)privacy);
	
	tlv tlv00cb;
	tlv00cb.setType(0x00cb);
	tlv00cb.setData((quint32)0xffffffff);
	
	length = length + tlv00ca.getLength() + tlv00cb.getLength();
	packet.append(convertToByteArray((quint16)length));
	packet.append(snac1309.getData());
	
	packet.append(convertToByteArray((quint16)0x0000));
	packet.append(convertToByteArray((quint16)pdInfoGroupId));
	packet.append(convertToByteArray((quint16)pdInfoId));
	packet.append(convertToByteArray((quint16)0x0004));
	packet.append(convertToByteArray((quint16)(tlv00ca.getLength() + tlv00cb.getLength())));
	
	packet.append(tlv00ca.getData());
	packet.append(tlv00cb.getData());
	tcpSocket->write(packet);
	
}

QByteArray servicesSetup::getClientIdentification()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	settings.beginGroup("clientid");
	
	unsigned newClientIndex = settings.value("index", 0).toUInt();

	QString newClientCap1 = settings.value("cap1").toString();
	QString	newClientCap2 = settings.value("cap2").toString();
	QString	newClientCap3 = settings.value("cap3").toString();
	
	settings.endGroup();
	
	QByteArray clientCapab;
	
	QByteArray cap1 = QByteArray::fromHex(newClientCap1.toLocal8Bit());
	QByteArray cap2 = QByteArray::fromHex(newClientCap2.toLocal8Bit());
	QByteArray cap3 = QByteArray::fromHex(newClientCap3.toLocal8Bit());
	
	if ( cap1.length() == 16 )
		clientCapab.append(cap1);
	
	if ( cap2.length() == 16 )
			clientCapab.append(cap2);
	
	if ( cap3.length() == 16 )
			clientCapab.append(cap3);
	
	
	switch(newClientIndex)
	{
	case 0:
		clientCapab.append(qutimCapab());
		break;
	case 1:
		clientCapab.append(icq6Capab());
		break;	
	case 2:
		clientCapab.append(icq51Capab());
		break;	
	case 3:
		clientCapab.append(icq5Capab());
		break;	
	case 4:
		clientCapab.append(icq4Capab());
		break;	
	case 5:
		clientCapab.append(icq2003bCapab());
		break;
	case 6:
		clientCapab.append(icq2002Capab());
		break;
	case 7:
		clientCapab.append(icqMacCapab());
		break;
	case 8:
		clientCapab.append(icqQip2005Capab());
		break;
	case 9:
		clientCapab.append(icqQipInfCapab());
		break;
	default:
		;
	}
	
	// we always send a short_caps capability
	clientCapab.append(QByteArray::fromHex("094600004c7f11d18222444553540000"));
	
	return clientCapab;
}

QByteArray servicesSetup::icq6Capab()
{
	QByteArray packet;
	
	packet.append(QByteArray::fromHex("0138ca7b769a491588f213fc00979ea8"));
	
	packet.append(QByteArray::fromHex("67361515612d4c078f3dbde6408ea041"));
	
	packet.append(QByteArray::fromHex("1a093c6cd7fd4ec59d51a6474e34f5a0"));
	
	packet.append(QByteArray::fromHex("b2ec8f167c6f451bbd79dc58497888b9"));
	
	packet.append(QByteArray::fromHex("178c2d9bdaa545bb8ddbf3bdbd53a10a"));
	
	packet.append(QByteArray::fromHex("0946134e4c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("094613494c7f11d18222444553540000"));
	
	
	packet.append(QByteArray::fromHex("563fc8090b6f41bd9f79422609dfa2f3"));
	
	packet.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));
	return packet;
}

void servicesSetup::sendCapabilities(QTcpSocket *socket)
{
	socket->write(get0204());
}

QByteArray servicesSetup::getProtocolVersion(unsigned index, unsigned port)
{
	QByteArray directInfo;
	directInfo.append(convertToByteArray((quint32)0x00000000));
	directInfo.append(convertToByteArray((quint32)0x00000000));
	directInfo.append(convertToByteArray((quint8)0x00));
	
	
	
	switch(index)
	{
	case 0:
		directInfo.append(convertToByteArray((quint16)0x0009));
		break;
	case 1:
		directInfo.append(convertToByteArray((quint16)0x0009));
		break;
	case 2:
		directInfo.append(convertToByteArray((quint16)0x0009));
		break;
	case 3:
		directInfo.append(convertToByteArray((quint16)0x0009));
		break;
	case 4:
		directInfo.append(convertToByteArray((quint16)0x0009));
		break;
	case 5:
		directInfo.append(convertToByteArray((quint16)0x000a));
		break;
	case 6:
		directInfo.append(convertToByteArray((quint16)0x0008));
		break;
	case 7:
		directInfo.append(convertToByteArray((quint16)0x0007));
		break;
	case 8:
		directInfo.append(convertToByteArray((quint16)0x000b));
		break;
	case 9:
		directInfo.append(convertToByteArray((quint16)0x000b));
		break;
	default:
		directInfo.append(convertToByteArray((quint16)port));
	}
	
	quint32 authcookie = ((quint32)qrand()) * QTime::currentTime().hour() * QTime::currentTime().minute()
		* QTime::currentTime().second() * QTime::currentTime().msec();
	
	directInfo.append(convertToByteArray((quint32)authcookie));
	
	quint16 webfrontport = ((quint32)qrand()) * QTime::currentTime().hour() * QTime::currentTime().minute()
		* QTime::currentTime().second() * QTime::currentTime().msec();
	
	
	directInfo.append(convertToByteArray((quint16)webfrontport));
	directInfo.append(convertToByteArray((quint32)0));
	
	if ( index == 8 )
	{
		directInfo.append(convertToByteArray((quint32)67584));
		directInfo.append(convertToByteArray((quint32)0x02000000));
		directInfo.append(convertToByteArray((quint32)0x000e0000));
		directInfo.append(convertToByteArray((quint16)0x000f));
	} else if ( index == 9 )
	{
		directInfo.append(convertToByteArray((quint32)65536));
		directInfo.append(convertToByteArray((quint32)0x23280000));
		directInfo.append(convertToByteArray((quint32)0x000b0000));
		directInfo.append(convertToByteArray((quint16)0x0000));
	} else {
		directInfo.append(convertToByteArray((quint32)65536));
		directInfo.append(convertToByteArray((quint32)0x00000000));
		directInfo.append(convertToByteArray((quint32)0x00000000));
		directInfo.append(convertToByteArray((quint16)0x0000));
	}
	directInfo.append(convertToByteArray((quint16)0x0000));
	return directInfo;
}

QByteArray servicesSetup::icq51Capab()
{

	QByteArray packet;
	
	packet.append(QByteArray::fromHex("0946134d4c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("094613494c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("b2ec8f167c6f451bbd79dc58497888b9"));
	
	packet.append(QByteArray::fromHex("563fc8090b6f41bd9f79422609dfa2f3"));
	
	packet.append(QByteArray::fromHex("e362c1e9121a4b94a6267a74de24270d"));
	
	packet.append(QByteArray::fromHex("178c2d9bdaa545bb8ddbf3bdbd53a10a"));
	
	packet.append(QByteArray::fromHex("97b12751243c4334ad22d6abf73f1492"));
	
	packet.append(QByteArray::fromHex("67361515612d4c078f3dbde6408ea041"));
	
	packet.append(QByteArray::fromHex("b99708b53a924202b069f1e757bb2e17"));
	
	packet.append(QByteArray::fromHex("1a093c6cd7fd4ec59d51a6474e34f5a0"));
	
	packet.append(QByteArray::fromHex("1a093c6cd7fd4ec59d51a6474e34f5a0"));
	
	packet.append(QByteArray::fromHex("0946134c4c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("094613444c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));
	
	return packet;
}

QByteArray servicesSetup::icq5Capab()
{

	QByteArray packet;
	
	packet.append(QByteArray::fromHex("0946134d4c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("563fc8090b6f41bd9f79422609dfa2f3"));
	
	packet.append(QByteArray::fromHex("e362c1e9121a4b94a6267a74de24270d"));
	
	packet.append(QByteArray::fromHex("094613444c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("178c2d9bdaa545bb8ddbf3bdbd53a10a"));
	
	packet.append(QByteArray::fromHex("97b12751243c4334ad22d6abf73f1492"));
	
	packet.append(QByteArray::fromHex("67361515612d4c078f3dbde6408ea041"));
	
	packet.append(QByteArray::fromHex("b99708b53a924202b069f1e757bb2e17"));
	
	packet.append(QByteArray::fromHex("1a093c6cd7fd4ec59d51a6474e34f5a0"));
	
	packet.append(QByteArray::fromHex("0946134c4c7f11d18222444553540000"));
	
	
	packet.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));
	
	
	return packet;
}

QByteArray servicesSetup::icq4Capab()
{

	QByteArray packet;
	
	packet.append(QByteArray::fromHex("0946134d4c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("563fc8090b6f41bd9f79422609dfa2f3"));
	
	packet.append(QByteArray::fromHex("094613444c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("178c2d9bdaa545bb8ddbf3bdbd53a10a"));
	
	packet.append(QByteArray::fromHex("97b12751243c4334ad22d6abf73f1492"));
	
	packet.append(QByteArray::fromHex("1a093c6cd7fd4ec59d51a6474e34f5a0"));
	
	packet.append(QByteArray::fromHex("0946134c4c7f11d18222444553540000"));
	
	return packet;
}

QByteArray servicesSetup::icq2003bCapab()
{

	QByteArray packet;
	
	packet.append(QByteArray::fromHex("0946134d4c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("563fc8090b6f41bd9f79422609dfa2f3"));
	
	packet.append(QByteArray::fromHex("094613444c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("97b12751243c4334ad22d6abf73f1492"));
	
	
	return packet;
}

QByteArray servicesSetup::icq2002Capab()
{

	QByteArray packet;
	
	packet.append(QByteArray::fromHex("0946134d4c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("094613444c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("97b12751243c4334ad22d6abf73f1492"));
	
	
	return packet;
}

QByteArray servicesSetup::icqMacCapab()
{

	QByteArray packet;
	
	packet.append(QByteArray::fromHex("0946134d4c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("094613444c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("dd16f20284e611d490db00104b9b4b7d"));
	
	return packet;
}

QByteArray servicesSetup::icqQip2005Capab()
{

	QByteArray packet;
	
	
	packet.append(QByteArray::fromHex("0946134d4c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("0946134c4c7f11d18222444553540000"));

	packet.append(QByteArray::fromHex("563fc8090b6f41bd9f79422609dfa2f3"));
	
	packet.append(QByteArray::fromHex("563fc8090b6f41514950203230303561"));
		
	return packet;
}

QByteArray servicesSetup::icqQipInfCapab()
{

	QByteArray packet;
	
	
	packet.append(QByteArray::fromHex("0946134d4c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("0946134c4c7f11d18222444553540000"));
	

	packet.append(QByteArray::fromHex("563fc8090b6f41bd9f79422609dfa2f3"));
	
	packet.append(QByteArray::fromHex("7c737502c3be4f3ea69f015313431e1a"));
	
	packet.append(QByteArray::fromHex("1a093c6cd7fd4ec59d51a6474e34f5a0"));
	
	packet.append(QByteArray::fromHex("7c533ffa68004f21bcfbc7d2439aad31"));
		
	return packet;
}

QByteArray servicesSetup::getXstatusCap(int index)
{
	switch(index)
	{
	case 0:
		icqMood = "icqmood23";
		return QByteArray::fromHex("01d8d7eeac3b492aa58dd3d877e66b92");
	case 1:
		icqMood = "icqmood1";
		return QByteArray::fromHex("5a581ea1e580430ca06f612298b7e4c7");
	case 2:
		icqMood = "icqmood2";
		return QByteArray::fromHex("83c9b78e77e74378b2c5fb6cfcc35bec");
	case 3:
		icqMood = "icqmood3";
		return QByteArray::fromHex("e601e41c33734bd1bc06811d6c323d81");
	case 4:
		icqMood = "icqmood4";
		return QByteArray::fromHex("8c50dbae81ed4786acca16cc3213c7b7");
	case 5:
		icqMood = "icqmood5";
		return QByteArray::fromHex("3fb0bd36af3b4a609eefcf190f6a5a7f");
	case 6:
		icqMood = "icqmood6";
		return QByteArray::fromHex("f8e8d7b282c4414290f810c6ce0a89a6");
	case 7:
		icqMood = "icqmood7";
		return QByteArray::fromHex("80537de2a4674a76b3546dfd075f5ec6");
	case 8:
		icqMood = "icqmood8";
		return QByteArray::fromHex("f18ab52edc57491d99dc6444502457af");
	case 9:
		icqMood = "icqmood9";
		return QByteArray::fromHex("1b78ae31fa0b4d3893d1997eeeafb218");
	case 10:
		icqMood = "icqmood10";
		return QByteArray::fromHex("61bee0dd8bdd475d8dee5f4baacf19a7");
	case 11:
		icqMood = "icqmood11";
		return QByteArray::fromHex("488e14898aca4a0882aa77ce7a165208");
	case 12:
		icqMood = "icqmood12";
		return QByteArray::fromHex("107a9a1812324da4b6cd0879db780f09");
	case 13:
		icqMood = "icqmood13";
		return QByteArray::fromHex("6f4930984f7c4affa27634a03bceaea7");
	case 14:
		icqMood = "icqmood14";
		return QByteArray::fromHex("1292e5501b644f66b206b29af378e48d");
	case 15:
		icqMood = "icqmood15";
		return QByteArray::fromHex("d4a611d08f014ec09223c5b6bec6ccf0");
	case 16:
		icqMood = "icqmood16";
		return QByteArray::fromHex("609d52f8a29a49a6b2a02524c5e9d260");
	case 17:
//		icqMood = "icqmood17";
		icqMood.clear();
		return QByteArray::fromHex("63627337a03f49ff80e5f709cde0a4ee");
	case 18:
		icqMood = "icqmood17";
		return QByteArray::fromHex("1f7a4071bf3b4e60bc324c5787b04cf1");
	case 19:
		icqMood = "icqmood18";
//		icqMood.clear();
		return QByteArray::fromHex("785e8c4840d34c65886f04cf3f3f43df");
	case 20:
		icqMood = "icqmood19";
		return QByteArray::fromHex("a6ed557e6bf744d4a5d4d2e7d95ce81f");
	case 21:
		icqMood = "icqmood20";
		return QByteArray::fromHex("12d07e3ef885489e8e97a72a6551e58d");
	case 22:
		icqMood = "icqmood21";
		return QByteArray::fromHex("ba74db3e9e24434b87b62f6b8dfee50f");
	case 23:
		icqMood = "icqmood22";
		return QByteArray::fromHex("634f6bd8add24aa1aab9115bc26d05a1");
	case 24:
		icqMood.clear();
		return QByteArray::fromHex("2ce0e4e57c6443709c3a7a1ce878a7dc");
	case 25:
		icqMood.clear();
		return QByteArray::fromHex("101117c9a3b040f981ac49e159fbd5d4");
	case 26:
		icqMood.clear();
		return QByteArray::fromHex("160c60bbdd4443f39140050f00e6c009");
	case 27:
		icqMood.clear();
		return QByteArray::fromHex("6443c6af22604517b58cd7df8e290352");
	case 28:
		icqMood.clear();
		return QByteArray::fromHex("16f5b76fa9d240358cc5c084703c98fa");
	case 29:
		icqMood.clear();
		return QByteArray::fromHex("631436ff3f8a40d0a5cb7b66e051b364");
	case 30:
		icqMood.clear();
		return QByteArray::fromHex("b70867f538254327a1ffcf4cc1939797");
	case 31:
		icqMood.clear();
		return QByteArray::fromHex("ddcf0ea971954048a9c6413206d6f280");
	case 32:
		icqMood.clear();
		return QByteArray::fromHex("d4e2b0ba334e4fa598d0117dbf4d3cc8");
	case 33:
		icqMood.clear();
		return QByteArray::fromHex("0072d9084ad143dd91996f026966026f");
	case 34:
		icqMood = "icqmood33";
		return QByteArray::fromHex("e601e41c33734bd1bc06811d6c323d82");
	case 35:
		icqMood = "icqmood32";
		return QByteArray::fromHex("d6687f4f3dc34bdb8a8c4c1a572763cd");
	case 36:
		icqMood = "icqmood60";
		return QByteArray();
	case 37:
		icqMood = "icqmood61";
		return QByteArray();
	case 38:
		icqMood = "icqmood62";
		return QByteArray();
	case 39:
		icqMood = "icqmood63";
		return QByteArray();
	case 40:
		icqMood = "icqmood64";
		return QByteArray();
	case 41:
		icqMood = "icqmood65";
		return QByteArray();
	}

	return QByteArray();
}

QByteArray servicesSetup::qutimCapab()
{
	QByteArray packet;
	
	packet.append(QByteArray::fromHex("69716d7561746769656d000000000000"));
	
	packet.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));
	
	packet.append(QByteArray::fromHex("563fc8090b6f41bd9f79422609dfa2f3"));
	
	QByteArray clientId;
	clientId.append("qutim");
	
#if defined(Q_OS_WIN32)
	clientId.append("w");
#elif defined(Q_OS_LINUX)
	clientId.append("l");
#elif defined(Q_OS_MAC)
	clientId.append("m");
#else
	clientId.append("u");
#endif
	quint8 major,minor, secminor;
	quint16 svn;
	//off version
	IcqPluginSystem::instance().getQutimVersion(major, minor, secminor, svn);
	clientId.append(convertToByteArray((quint8)major));
	clientId.append(convertToByteArray((quint8)minor));
	clientId.append(convertToByteArray((quint8)secminor));
	
	//svn version
	clientId.append(convertToByteArray((quint16)svn));
	
	clientId.append(QByteArray::fromHex("0000000000"));
	
	packet.append(clientId);
//	packet.append(QByteArray::fromHex("717574696d302e310000000000000000"));
	
	return packet;
}

void servicesSetup::sendXStatusAsAvailableMessage(QTcpSocket *socket)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin, "accountsettings");

	snac snac011e;
	snac011e.setFamily(0x0001);
	snac011e.setSubType(0x001e);
	snac011e.setReqId(snac011eseq);
	tlv availMess;
	availMess.setType(0x001d);
	
	QByteArray mess;
	if ( icqMood.isEmpty())
	{
		mess.append(QByteArray::fromHex("00020000000e0000"));
	} else {
		
		QByteArray cap = settings.value("xstatus/caption", "").toString().toUtf8().left(40);
		QByteArray msg = settings.value("xstatus/message", "").toString().toUtf8().left(200);
		quint16 length = cap.length() + msg.length() + 1;
		mess.append(QByteArray::fromHex("000204"));
		mess.append(convertToByteArray((quint8)(length +4)));
		mess.append(convertToByteArray((quint16)length));
		mess.append(cap);
		mess.append(convertToByteArray((quint8)0x20));
		mess.append(msg);
		mess.append(QByteArray::fromHex("0000000e"));
		mess.append(convertToByteArray((quint16)icqMood.length()));
		mess.append(icqMood);
	}
	
	availMess.setData(mess);
		
	
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;

	packet.append(convertToByteArray(flap011eseq));
	packet.append(convertToByteArray((quint16)(10 + availMess.getLength())));
	packet.append(snac011e.getData());
	packet.append(availMess.getData());

	socket->write(packet);	
}
