/*
    snacChannel

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
#include <QHostAddress>
#include "tlv.h"
#include "snac.h"
#include "clientmd5login.h"
#include "serverloginreply.h"
#include "servicessetup.h"
#include "buffer.h"

#include "snacchannel.h"

snacChannel::snacChannel(QTcpSocket *s, icqBuffer *buff, quint16 seq, const QString &uin,
		const QString &profile_name, QObject *parent) 
: QObject(parent)
, icqUin(uin)
, tcpSocket(s)
, socket(buff)
, m_profile_name(profile_name)
{
	snacReqId = 0x00000000;
	flapSequence = seq;
	reqSeq = 0x0000;
        m_connect_bos = false;
	

}

snacChannel::~snacChannel()
{
	
}

quint32 snacChannel::returnSnacReqId()
{
        if(!m_connect_bos)
            return 0x00000000;
	if ( snacReqId != 0xffffffff )
		snacReqId++;
	else
		snacReqId = 0x00000000;
	return snacReqId;
}

void snacChannel::sendIdent(const quint16 &flapSeq)
{
	
	QByteArray authReq;
	authReq.append(convertToByteArray((quint8)0x2A));
	authReq.append(convertToByteArray((quint8)0x01));
	authReq.append(convertToByteArray((quint16)flapSeq));
        authReq.append(convertToByteArray((quint16)0x000c));
        authReq.append(convertToByteArray((quint32)0x00000001));
        authReq.append(convertToByteArray((quint32)0x80030004));
        authReq.append(convertToByteArray((quint32)0x00100000));
	emit incFlapSeq();
	authReq.append(convertToByteArray((quint8)0x2A));
	authReq.append(convertToByteArray((quint8)0x02));
	authReq.append(convertToByteArray((quint16)flapSeq));
	tlv screenName;
	screenName.setType(0x0001);
	screenName.setData(icqUin);
	quint16 length = 18;
	length += screenName.getLength();
	authReq.append(convertToByteArray(length));
	snac snacPacket;
	snacPacket.setFamily(0x0017);
	snacPacket.setSubType(0x0006);
	snacPacket.setReqId(returnSnacReqId());
	authReq.append(snacPacket.getData());
	authReq.append(screenName.getData());
	authReq.append(convertToByteArray((quint16)0x004B));
	authReq.append(convertToByteArray((quint16)0x0000));
	authReq.append(convertToByteArray((quint16)0x005A));
	authReq.append(convertToByteArray((quint16)0x0000));
	tcpSocket->write(authReq);
	emit incFlapSeq();
}


QByteArray snacChannel::convertToByteArray(const quint8 &d)
{
	QByteArray packet;
	packet[0] = d;
	return packet;
}

QByteArray snacChannel::convertToByteArray(const quint16 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x100);
	packet[1] = (d % 0x100);
	return packet;
}

QByteArray snacChannel::convertToByteArray(const quint32 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x1000000);
	packet[1] = (d / 0x10000);
	packet[2] = (d / 0x100);
	packet[3] = (d % 0x100);
	return packet;
}

void snacChannel::readData(quint16 length)
{
	snac snacPacket;
	snacPacket.readData(socket);
	length -= 10;
	switch ( snacPacket.getFamily())
	{
	case 0x00001:
		switch(snacPacket.getSubType())
		{
		case 0x0003:
			getServicesList(length);			
			break;
		case 0x0005:	
			emit readSSTserver(length);
			length = 0;
			break;
		case 0x0007:
			serverRateLimitInfo(length);
			break;
		case 0x0018:
			clientRatesRequest();
			break;
		case 0x000f:

			break;
			
		case 0x0021:
			emit getUploadIconData(length);
			length = 0;
			break;
		default:
//			qDebug()<<"Unknown 0x0001 snac sybtype:"<<snacPacket.getSubType();
			break;
		}
		break;
	case 0x0002:
		switch(snacPacket.getSubType())
		{
		case 0x0006:		
			emit getStatusCheck(length);
			length = 0;
			break;
		default:
			;
		}
		break;
	case 0x0003:
		switch(snacPacket.getSubType())
		{
		case 0x000b:

			getOncomingBuddy(length);
			length = 0;
			break;
			
		case 0x000c:

			getOfflineBuddy(length);
			length = 0;
			break;
		default:
			break;
		}
		
		break;
	case 0x0004:
		switch( snacPacket.getSubType() )
		{
		case 0x0007:

			emit getMessage(length);
			length = 0;
			break;
		case 0x000b:
			emit getAwayMessage(length);
			length = 0;
			break;
		case 0x000c:
			emit getMessageAck(length);
			length = 0;
			break;
		case 0x0014:

			emit getTypingNotif(length);
			length = 0;
			break;
		default:
			break;
		}
		break;
	case 0x0013:
		switch( snacPacket.getSubType() )
		{
		case 0x0006:

			getContactList(length, snacPacket.aloneSnac());
			break;
		case 0x000e:

			emit getModifyItemFromServer(length);
			length = 0;
			break;	
			
		case 0x001c:
			emit youWereAdded(length);
			length = 0;
			break;
			
		case 0x0019:
			emit getAuthorizationRequest(length);
			length = 0;
			break;
			
		case 0x001b:
			emit authorizationAcceptedAnswer(length);
			length = 0;
			break;
		default:
//			qDebug()<<"Unknown 0x0013 snac sybtype:"<<snacPacket.getSubType();
			break;
		}
	break;
	case 0x0015:
		switch( snacPacket.getSubType() )
		{
		case 0x0003:

			getMetaData(snacPacket.aloneSnac());
			length = 0;
		break;
		
		default:
		break;
		}
		break;
	case 0x0017:
		switch(snacPacket.getSubType())
		{
		case 0x0003:

			getServerLoginReply(length);
			break;
		case 0x0007:

			readAuthKey(length);
		break;
		default:
			break;
		}
	break;
	default:

//		qDebug()<<"Unknown snac family:"<<snacPacket.getFamily();
		break;
	}
	
	if (length)
		socket->read(length);
	if ( socket->bytesAvailable() )
	{
		
		emit rereadSocket();	
	}
}

quint16 snacChannel::convertToInt16(const QByteArray &packet)
{
	bool ok;
	return packet.toHex().toUInt(&ok,16);
}
quint8 snacChannel::convertToInt8(const QByteArray &packet)
{
	bool ok;
	return packet.toHex().toUInt(&ok,16);
}


quint32 snacChannel::convertToInt32(const QByteArray &array)
{
	bool ok;
	return array.toHex().toULong(&ok,16);
}

void snacChannel::readAuthKey(quint16 &length)
{
	quint16 authLength = convertToInt16(socket->read(2));
	length -=2;
	length -= authLength;
	emit sendAuthKey(socket->read(authLength));
}

void snacChannel::md5Login(const QString &password, const QString &authKey, const quint16 & flapSeq)
{
	clientMd5Login login;
	login.setScreenName(icqUin);
	login.setPassword(password, authKey);
	login.sendPacket(tcpSocket,flapSeq,returnSnacReqId());
	emit incFlapSeq();
}

void snacChannel::getServerLoginReply(quint16 &length)
{
	serverLoginReply loginReply;
	loginReply.readData(tcpSocket, socket, icqUin);
	if ( loginReply.isSnacError )
	{
		errorMessage(loginReply.errorMessage);
	} else {
		emit sendBosServer(QHostAddress(loginReply.bosIp));
		emit sendBosPort(loginReply.bosPort);
		emit sendCookie(loginReply.cookie);
	}
	length = 0;
}

void snacChannel::errorMessage(quint16 error)
{
	switch(error)
			{
			case 0x01:
				emit systemMessage(tr("Invalid nick or password"));
				emit blockRateLimit();
				break;
			case 0x02:
				emit systemMessage(tr("Service temporarily unavailable"));
				break;
			case 0x04:
				emit systemMessage(tr("Incorrect nick or password"));
				emit blockRateLimit();
				break;
			case 0x05:
				emit systemMessage(tr("Mismatch nick or password"));
				emit blockRateLimit();
				break;
			case 0x06:
				emit systemMessage(tr("Internal client error (bad input to authorizer)"));
				break;
			case 0x07:
				emit systemMessage(tr("Invalid account"));
				emit blockRateLimit();
				break;
			case 0x08:
				emit systemMessage(tr("Deleted account"));
				emit blockRateLimit();
				break;
			case 0x09:
				emit systemMessage(tr("Expired account"));
				emit blockRateLimit();
				break;
			case 0x0A:
				emit systemMessage(tr("No access to database"));
				break;
			case 0x0B:
				emit systemMessage(tr("No access to resolver"));
				break;
			case 0x0C:
				emit systemMessage(tr("Invalid database fields"));
				break;
			case 0x0D:
				emit systemMessage(tr("Bad database status"));
				break;
			case 0x0E:
				emit systemMessage(tr("Bad resolver status"));
				break;
			case 0x0F:
				emit systemMessage(tr("Internal error"));
				break;
			case 0x10:
				emit systemMessage(tr("Service temporarily offline"));
				break;
			case 0x11:
				emit systemMessage(tr(" Suspended account"));
				break;
			case 0x12:
				emit systemMessage(tr("DB send error"));
				break;
			case 0x13:
				emit systemMessage(tr("DB link error"));
				break;
			case 0x14:
				emit systemMessage(tr("Reservation map error"));
				break;
			case 0x15:
				emit systemMessage(tr("The users num connected from this IP has reached the maximum"));
				break;
			case 0x17:
				emit systemMessage(tr(" The users num connected from this IP has reached the maximum (reservation)"));
				break;
			case 0x18:
				emit systemMessage(tr("Rate limit exceeded (reservation). Please try to reconnect in a few minutes"));
				emit blockRateLimit();
				break;
			case 0x19:
				emit systemMessage(tr("User too heavily warned"));
				break;
			case 0x1A:
				emit systemMessage(tr("Reservation timeout"));
				break;
			case 0x1B:
				emit systemMessage(tr("You are using an older version of ICQ. Upgrade required"));
				break;
			case 0x1C:
				emit systemMessage(tr("You are using an older version of ICQ. Upgrade recommended"));
				break;
			case 0x1D:
				emit systemMessage(tr("Rate limit exceeded. Please try to reconnect in a few minutes"));
				emit blockRateLimit();
				break;
			case 0x1E:
				emit systemMessage(tr("Can't register on the ICQ network. Reconnect in a few minutes"));
				break;
			case 0x20:
				emit systemMessage(tr("Invalid SecurID"));
				break;
			case 0x22:
				emit systemMessage(tr("Account suspended because of your age (age < 13)"));
				break;
			default:
				emit systemMessage(tr("Connection Error: %1").arg(error));
			}
}

void snacChannel::incFlap()
{
	if ( flapSequence != 0x8000 )
		flapSequence++;
	else
		flapSequence= 0x0000;
}

void snacChannel::getServicesList(quint16 &length)
{
	
	int arrayLength = length / 2;
//	quint16 familyList[arrayLength];
	
	QList<quint16 > familyList;
	for ( int i = 0; length; ++i)
	{
//		familyList[i] = convertToInt16(socket->read(2));
		familyList.append(convertToInt16(socket->read(2)));
		length -= 2;
	}
	QByteArray famVers;
	for (int i = 0; i < arrayLength; i++)
	{
		famVers.append(convertToByteArray(familyList[i]));
		if ( familyList.at(i) == 0x0001 )
			famVers.append(convertToByteArray((quint16)0x0004));
		else if ( familyList.at(i) == 0x0013 )
			famVers.append(convertToByteArray((quint16)0x0004));
		else
			famVers.append(convertToByteArray((quint16)0x0001));
	}
	
	if ( length )
		socket->readAll();
	quint16 flapLength = 10 + arrayLength * 4;
	QByteArray clientFamVers;
	clientFamVers[0] = 0x2A;
	clientFamVers[1] = 0x02;
	clientFamVers.append(convertToByteArray(flapSequence));
	clientFamVers.append(convertToByteArray(flapLength));
	snac fv;
	fv.setFamily(0x0001);
	fv.setSubType(0x0017);
	fv.setReqId(returnSnacReqId());
	clientFamVers.append(fv.getData());
	clientFamVers.append(famVers);
	tcpSocket->write(clientFamVers);
	emit incFlapSeq();
	
}

void snacChannel::clientRatesRequest()
{
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flapSequence));
	packet.append(convertToByteArray((quint16)10));
	snac snacPacket;
	snacPacket.setFamily(0x0001);
	snacPacket.setSubType(0x0006);
	snacPacket.setReqId(returnSnacReqId());
	packet.append(snacPacket.getData());
	tcpSocket->write(packet);
	emit incFlapSeq();
}

void snacChannel::serverRateLimitInfo(quint16 &length)
{
	quint16 arrayLength = convertToInt16(socket->read(2));
	length  = 0;
//	quint16 classArray[arrayLength];
	QList<quint16> classArray;
	for (int i = 0; i < arrayLength; i++)
	{
//		classArray[i] = convertToInt16(socket->read(2));
		classArray.append(convertToInt16(socket->read(2)));
		socket->read(33);
	}
	socket->readAll();
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x02;
	packet.append(convertToByteArray(flapSequence));
	quint16 flapLength = 10 + arrayLength * 2;
	packet.append(convertToByteArray(flapLength));
	snac infoSnac;
	infoSnac.setFamily(0x0001);
	infoSnac.setSubType(0x0008);
	infoSnac.setReqId(returnSnacReqId());
	packet.append(infoSnac.getData());
	for (int i = 0; i < arrayLength; i++)
			packet.append(convertToByteArray(classArray[i]));
	tcpSocket->write(packet);

	emit incFlapSeq();
	
	
	
	//send servicesSetup;
	
	servicesSetup servSetup(icqUin, m_profile_name);
	
	servSetup.flap011eseq = flapSequence;
	emit incFlapSeq();
	servSetup.snac011eseq = returnSnacReqId();
	
	
	servSetup.flap0202seq = flapSequence;
	emit incFlapSeq();
	servSetup.snac0202seq = returnSnacReqId();
	
	servSetup.flap0204seq = flapSequence;
	emit incFlapSeq();
	servSetup.snac0204seq = returnSnacReqId();
	
	servSetup.flap0302seq = flapSequence;
	emit incFlapSeq();
	servSetup.snac0302seq = returnSnacReqId();
	
	servSetup.flap0404seq = flapSequence;
	emit incFlapSeq();
	servSetup.snac0404seq = returnSnacReqId();
	
	servSetup.flap0402seq = flapSequence;
	emit incFlapSeq();
	servSetup.snac0402seq = returnSnacReqId();
	
	servSetup.flap0402seq02 = flapSequence;
	emit incFlapSeq();
	servSetup.snac0402seq02 = returnSnacReqId();
	
	servSetup.flap0902seq = flapSequence;
	emit incFlapSeq();
	servSetup.snac0902seq = returnSnacReqId();
	
	servSetup.flap1302seq = flapSequence;
	emit incFlapSeq();
	servSetup.snac1302seq = returnSnacReqId();
		
	servSetup.flap1305seq = flapSequence;
	emit incFlapSeq();
	servSetup.snac1305seq = returnSnacReqId();
		
	servSetup.setStatus(status);
	servSetup.sendData(tcpSocket, icqUin);
	emit connected();
}

void snacChannel::changeStatus(accountStatus status)
{
	servicesSetup servSetup(icqUin, m_profile_name);
	servSetup.flap011eseq = flapSequence;
	emit incFlapSeq();
	servSetup.snac011eseq = returnSnacReqId();
	servSetup.changeStatus(status, tcpSocket, icqUin);
}

void snacChannel::getContactList(quint16 &length,bool aloneSnac)
{
	
//	quint16 tmpL = socket->bytesAvailable() - length;
//	emit clearPrivacyLists();
	emit getList(!aloneSnac);
//	if ( socket->bytesAvailable() != tmpL )
//	{
//		
//		socket->read(socket->bytesAvailable() - tmpL);
//	}
	
//	socket->read(length);
	length = 0;
	if ( !aloneSnac )
	{
		servicesSetup servSetup(icqUin, m_profile_name);
		servSetup.flap1307seq = flapSequence;
		emit incFlapSeq();
		servSetup.snac1307seq = returnSnacReqId();
				
		servSetup.flap0102seq = flapSequence;
		emit incFlapSeq();
		servSetup.snac0102seq = returnSnacReqId();
		
		bool ok;
		servSetup.uin = icqUin.toUInt(&ok,10);
		
		servSetup.flap1502seq = flapSequence;
		emit incFlapSeq();
		servSetup.snac1502seq = returnSnacReqId();
		
		servSetup.req1502seq = reqSeq;
		emit incReqSeq();
		
		servSetup.answerToList(tcpSocket);
	} 
	
	
}

void snacChannel::getOncomingBuddy(quint16 &length)
{
	quint8 uinLength = convertToInt8(socket->read(1));
	length = length - 1 - uinLength;
	QString uin = QString::fromUtf8(socket->read(uinLength));
	emit oncomingBuddy(uin, length);
}

void snacChannel::getOfflineBuddy(quint16 &length)
{
	quint8 uinLength = convertToInt8(socket->read(1));
	length = length - 1 - uinLength;
	QString uin = QString::fromUtf8(socket->read(uinLength));
	emit offlineBuddy(uin, length);
}

void snacChannel::getMetaData(bool notAlone)
{
	socket->read(2);
	quint16 length = convertToInt16(socket->read(2)) - 2;
	socket->read(2);
	QByteArray targetUin = socket->read(4);
	quint16 dataType = convertToInt16(socket->read(2));
	if ( dataType == 0x4200)
	{
		socket->read(3);
		QByteArray packet;
		packet[0] = 0x2a;
		packet[1] = 0x02;
		packet.append(convertToByteArray((quint16)flapSequence));
		packet.append(convertToByteArray((quint16)24));
		incFlapSeq();
		snac snac1502;
		snac1502.setFamily(0x0015);
		snac1502.setSubType(0x0002);
		snac1502.setReqId(returnSnacReqId());
		packet.append(snac1502.getData());
		packet.append(convertToByteArray((quint16)0x0001));
		packet.append(convertToByteArray((quint16)0x000a));
		packet.append(convertToByteArray((quint16)0x0800));
		packet.append(targetUin);
		packet.append(convertToByteArray((quint16)0x3e00));
		packet.append(convertToByteArray((quint16)reqSeq));
		incReqSeq();
		tcpSocket->write(packet);
		
	} else if ( dataType == 0x4100 )
	{
		emit getOfflineMessage();
	} else  if ( dataType == 0xda07 )
	{
		emit readMetaData(length - 6, notAlone);
	}else {
		socket->read(length - 6);
	}
	
}

void snacChannel::resendCapabilities()
{
	servicesSetup servSetup(icqUin, m_profile_name);
	servSetup.flap0204seq = flapSequence;
	emit incFlapSeq();
	servSetup.snac0204seq = returnSnacReqId();
	servSetup.sendCapabilities(tcpSocket);
	
	servSetup.flap011eseq = flapSequence;
	emit incFlapSeq();
	servSetup.snac011eseq = returnSnacReqId();
	servSetup.changeStatus(status, tcpSocket, icqUin);
}

void snacChannel::sendOnlyCapabilities()
{
	servicesSetup servSetup(icqUin, m_profile_name);
	servSetup.flap0204seq = flapSequence;
	emit incFlapSeq();
	servSetup.snac0204seq = returnSnacReqId();
	servSetup.sendCapabilities(tcpSocket);
	
	servSetup.flap011eseq = flapSequence;
	emit incFlapSeq();
	servSetup.snac011eseq = returnSnacReqId();
	servSetup.sendXStatusAsAvailableMessage(tcpSocket);
}
