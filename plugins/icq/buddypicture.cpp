/*
    buddyPicture

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

#include <QSettings>
#include <QDir>
#include <QTcpSocket>
#include <QHostAddress>

#include "buffer.h"
#include "flap.h"
#include "snac.h"
#include "tlv.h"

#include "buddypicture.h"

buddyPicture::buddyPicture(const QString &profile_name, const QString &mine_uin,
		QObject *parent) 
: QObject(parent)
, m_profile_name(profile_name)
, m_mine_uin(mine_uin)
{
	readyToReadFlap = true;
	flapSeqNum = rand() % 0x8000;
	snacSeqNum = 0x00000000;
	connectedToServ = false;
	canSendReqForAvatars = false;
	alreadySentCap = false;
	refNum = 1;
	
	tcpSocket = new QTcpSocket(this);
	connect ( tcpSocket, SIGNAL(readyRead()),
				this, SLOT(readDataFromSocket()));
	connect ( tcpSocket, SIGNAL(disconnected()),
				this, SLOT(socketDisconnected()));
	connect ( tcpSocket, SIGNAL(connected()),
					this, SLOT(socketConnected()));
	buffer = new icqBuffer(this);
	buffer->open(QIODevice::ReadWrite);
}

buddyPicture::~buddyPicture()
{
	
}

void buddyPicture::socketDisconnected()
{
	connectedToServ = false;
	canSendReqForAvatars = false;
	alreadySentCap = false;
	buffer->readAll();
}
void buddyPicture::sendHash(const QString &uin, const QByteArray &hash)
{
	
	if ( tcpSocket->state() == QAbstractSocket::ConnectedState )
	{	
		QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapSeqNum));
	incFlapSeq();
	quint16 length = 32 + uin.length();
	
	packet.append(convertToByteArray((quint16)length));
	snac snac1006;
	snac1006.setFamily(0x0010);
	snac1006.setSubType(0x0006);
	snac1006.setReqId(snacSeqNum);
	incSnacSeq();
	packet.append(snac1006.getData());
	
	packet.append(convertToByteArray((quint8)uin.length()));
	packet.append(uin);
	packet.append(convertToByteArray((quint8)0x01));
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint16)0x0110));
	packet.append(hash);
	tcpSocket->write(packet);
	
	}
	
}

QByteArray buddyPicture::convertToByteArray(const quint8 &d)
{
	QByteArray packet;
	packet[0] = d;
	return packet;
}

QByteArray buddyPicture::convertToByteArray(const quint16 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x100);
	packet[1] = (d % 0x100);
	return packet;
}

QByteArray buddyPicture::convertToByteArray(const quint32 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x1000000);
	packet[1] = (d / 0x10000);
	packet[2] = (d / 0x100);
	packet[3] = (d % 0x100);
	return packet;
}

void buddyPicture::incFlapSeq()
{
	if ( flapSeqNum != 0x8000 )
			flapSeqNum++;
	else
		flapSeqNum = 0x0000;
}

void buddyPicture::incSnacSeq()
{
	if ( snacSeqNum != 0xffffffff )
		snacSeqNum++;
	else
		snacSeqNum = 0x00000000;
}

void buddyPicture::readDataFromSocket()
{
		buffer->write(tcpSocket->readAll());
		if ( readyToReadFlap )
		{
			
			flapPacket flap;
			if ( !flap.readFromSocket( buffer ) )
			{
				return;
			}
				
		
			channel = flap.getChannel(); 
			flapLength = flap.getLength();
			
		}
		
		if ( buffer->bytesAvailable() < flapLength )
		{
			readyToReadFlap = false;
			return;
		}
		readyToReadFlap = true;
			if ( channel == 0x01)
			{
//				qDebug()<<"hi";
//				alreadySentCap = true;
				buffer->read(flapLength);
//				sendCapab();
			}
				
			if ( channel == 0x02 )
			{
				readSnac(flapLength);

			}
			
			if ( channel == 0x03 )
				buffer->read(flapLength);
			
			if ( channel == 0x04)
				buffer->read(flapLength);
			if ( channel >= 0x05 )
				buffer->read(flapLength);
			
		if (buffer->bytesAvailable())
			readDataFromSocket();
}

void buddyPicture::readSnac(quint16 length)
{
	snac snacPacket;
	snacPacket.readData(buffer);
	length -= 10;
	
	switch ( snacPacket.getFamily())
	{
	case 0x0001:
		switch(snacPacket.getSubType())
		{
		case 0x0003:
			buffer->read(length);
			length = 0;
			if ( !alreadySentCap)
				sendCapab();
			break;
		case 0x0007:
			buffer->read(length);
			length = 0;
			sendRateInfoClientReady();
			break;
		case 0x0018:
			buffer->read(length);
			length = 0;
			sendInfoReq();
			break;
		default:
			;
		}
		break;
	case 0x0010:
		switch(snacPacket.getSubType())
		{
		case 0x0007:
			saveAvatar(length);
			length = 0;
			break;
		default:
			;
		}
		break;
	default:
		;
	}
	
	if (length)
			buffer->read(length);
	if ( buffer->bytesAvailable() )
	{
			
			readDataFromSocket();	
	}
}

void buddyPicture::sendCapab()
{
	if ( tcpSocket->state() == QAbstractSocket::ConnectedState )
	{	
	QByteArray packet;
		packet[0] = 0x2a;
		packet[1] = 0x02;
		packet.append(convertToByteArray((quint16)flapSeqNum));
		incFlapSeq();
		
		packet.append(convertToByteArray((quint16)18));
		snac snac0117;
		snac0117.setFamily(0x0001);
		snac0117.setSubType(0x0017);
		snac0117.setReqId(snacSeqNum);
		incSnacSeq();
		packet.append(snac0117.getData());
		
		packet.append(convertToByteArray((quint16)0x0001));
		packet.append(convertToByteArray((quint16)0x0003));
		packet.append(convertToByteArray((quint16)0x0010));
		packet.append(convertToByteArray((quint16)0x0001));
		tcpSocket->write(packet);
	}
}

void buddyPicture::sendInfoReq()
{
	if ( tcpSocket->state() == QAbstractSocket::ConnectedState )
	{
			QByteArray packet;
			packet[0] = 0x2a;
			packet[1] = 0x02;
			packet.append(convertToByteArray((quint16)flapSeqNum));
			
			incFlapSeq();
			packet.append(convertToByteArray((quint16)10));
			snac snac0106;
			snac0106.setFamily(0x0001);
			snac0106.setSubType(0x0006);
			snac0106.setReqId(snacSeqNum);

			packet.append(snac0106.getData());
			incSnacSeq();
			tcpSocket->write(packet);
	}
}

void buddyPicture::sendRateInfoClientReady()
{
	if ( tcpSocket->state() == QAbstractSocket::ConnectedState )
	{
		connectedToServ = true;
	QByteArray fullPacket;
	
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapSeqNum));
				
	incFlapSeq();
	packet.append(convertToByteArray((quint16)20));
	snac snac0108;
	snac0108.setFamily(0x0001);
	snac0108.setSubType(0x0008);
	snac0108.setReqId(snacSeqNum);

	packet.append(snac0108.getData());
	incSnacSeq();
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint16)0x0002));
	packet.append(convertToByteArray((quint16)0x0003));
	packet.append(convertToByteArray((quint16)0x0004));
	packet.append(convertToByteArray((quint16)0x0005));
	
	fullPacket.append(packet);
	
	QByteArray packet2;
	packet2[0] = 0x2a;
	packet2[1] = 0x02;
	packet2.append(convertToByteArray((quint16)flapSeqNum));
				
	incFlapSeq();
	packet2.append(convertToByteArray((quint16)26));
	snac snac0102;
	snac0102.setFamily(0x0001);
	snac0102.setSubType(0x0002);
	snac0102.setReqId(snacSeqNum);

	packet2.append(snac0102.getData());
	incSnacSeq();
	packet2.append(convertToByteArray((quint16)0x0001));
	packet2.append(convertToByteArray((quint16)0x0003));
	packet2.append(convertToByteArray((quint16)0x0110));
	packet2.append(convertToByteArray((quint16)0x047b));
	packet2.append(convertToByteArray((quint16)0x0010));
	packet2.append(convertToByteArray((quint16)0x0001));
	packet2.append(convertToByteArray((quint16)0x0110));
	packet2.append(convertToByteArray((quint16)0x047b));
	
	fullPacket.append(packet2);
	
	
	tcpSocket->write(fullPacket);
	canSendReqForAvatars = true;
	emit emptyAvatarList();
	}
}

void buddyPicture::connectToServ(const QString &addr, const quint16 &port, QByteArray cookie/*, const QNetworkProxy &proxy*/)
{
	
	QHostAddress hostAddr = QHostAddress(addr);
	if ( !hostAddr.isNull() )
	{
		connectedToServ = true;
//		tcpSocket->setProxy(proxy);
		tcpSocket->connectToHost(hostAddr, port);
		SSTcookie = cookie;
		
	}
	
	
	
}

void buddyPicture::disconnectFromSST()
{
	tcpSocket->disconnectFromHost();
	connectedToServ = false;
	canSendReqForAvatars = false;
	alreadySentCap = false;
	
}

quint8 buddyPicture::convertToInt8(const QByteArray &packet)
{
	bool ok;
	return packet.toHex().toUInt(&ok,16);
}

quint16 buddyPicture::convertToInt16(const QByteArray &packet)
{
	bool ok;
	return packet.toHex().toUInt(&ok,16);
}

quint32 buddyPicture::convertToInt32(const QByteArray &array)
{
	bool ok;
	return array.toHex().toULong(&ok,16);
}

void buddyPicture::saveAvatar(quint16 length)
{
	quint8 uinLength = convertToInt8(buffer->read(1));
	length -= 1;
	
	
	QString uin = QString::fromUtf8(buffer->read(uinLength));
	length -= uinLength;
	
	
	buffer->read(4);
	
	length -= 4;
	
	QByteArray hash = buffer->read(16);
	
	length -= 16;
	
	buffer->read(21);
	
	length -= 21;
	
	quint16 iconLength = convertToInt16(buffer->read(2));
	length -= 2;
	
	if ( iconLength )
	{
                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+m_mine_uin, "contactlist");
		settings.beginGroup(uin);
		settings.setValue("iconhash", hash.toHex());
		settings.endGroup();
		
		QString iconPath = settings.fileName().section('/', 0, -3) + "/icqicons";
		
		QDir iconDir(iconPath);
			if ( !iconDir.exists() )
				iconDir.mkpath(iconPath);
		
		QFile iconFile(iconPath + "/" + hash.toHex());
		
		
		if ( iconFile.open(QIODevice::WriteOnly) )
		{
			iconFile.write(buffer->read(iconLength));
		}
			
		emit updateAvatar(uin, hash);
	}
	length -= iconLength;
	
	if ( length )
		buffer->read(length);
}

void buddyPicture::socketConnected()
{
	
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x01;
	packet.append(convertToByteArray((quint16)flapSeqNum));
			
	incFlapSeq();

			
	tlv tlvCookie;
	tlvCookie.setType(0x0006);
	tlvCookie.setData(SSTcookie);
	packet.append(convertToByteArray((quint16)( tlvCookie.getLength() + 4)));
	packet.append(convertToByteArray((quint16)0x0000));
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(tlvCookie.getData());
	tcpSocket->write(packet);
}

void buddyPicture::uploadIcon(const QString &iconPath)
{
	if ( QFile::exists(iconPath))
	{
		QFile iconFile(iconPath);
		if ( iconFile.open(QIODevice::ReadOnly))
		{
			QByteArray packet;
			packet[0] = 0x2a;
			packet[1] = 0x02;
			packet.append(convertToByteArray((quint16)flapSeqNum));
						
			incFlapSeq();
			
			
			
			
			packet.append(convertToByteArray((quint16)(14 + iconFile.size())));
			snac snac1002;
			snac1002.setFamily(0x0010);
			snac1002.setSubType(0x0002);
			snac1002.setReqId(snacSeqNum);
		
			packet.append(snac1002.getData());
			incSnacSeq();
			
			packet.append(convertToByteArray((quint16)0x0001));
			refNum++;
			packet.append(convertToByteArray((quint16)iconFile.size()));
			packet.append(iconFile.readAll());

			
			tcpSocket->write(packet);
		}
	}
}
