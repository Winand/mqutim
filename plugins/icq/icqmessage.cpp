/*
    icqMessage

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

#include <QFile>
#include <QSettings>
#include <QTcpSocket>
#include <QTextCodec>

#include "tlv.h"
#include "buffer.h"
#include "snac.h"

#include "icqmessage.h"

icqMessage::icqMessage(const QString &codepage)
{
	codec = QTextCodec::codecForName(codepage.toLocal8Bit());
	downCounter1 = convertToByteArray((quint16)1);
	downCounter2 = convertToByteArray((quint16)1);
	msgType = 0;
	reason = 0;
/*	fileAnswer = false;*/
	peerIP = 0;
	peerPort = 0;
	fileSize = 0;
	aolProxyIP = 0;
}

icqMessage::~icqMessage()
{
	
}

QByteArray icqMessage::convertToByteArray(const quint8 &d)
{
	QByteArray packet;
	packet[0] = d;
	return packet;
}

QByteArray icqMessage::convertToByteArray(const quint16 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x100);
	packet[1] = (d % 0x100);
	return packet;
}

QByteArray icqMessage::convertLEToByteArray(const quint16 &d)
{
	QByteArray packet;
	packet[1] = (d / 0x100);
	packet[0] = (d % 0x100);
	return packet;
}

QByteArray icqMessage::convertToByteArray(const quint32 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x1000000);
	packet[1] = (d / 0x10000);
	packet[2] = (d / 0x100);
	packet[3] = (d % 0x100);
	return packet;
}

quint8 icqMessage::byteArrayToInt8(const QByteArray &array)
{
	bool ok;
	return array.toHex().toUInt(&ok,16);
}

quint16 icqMessage::byteArrayToInt16(const QByteArray &array)
{
	bool ok;
	return array.toHex().toUInt(&ok,16);
}

quint32 icqMessage::byteArrayToInt32(const QByteArray &array)
{
	bool ok;
	return array.toHex().toULong(&ok,16);
}

quint16 icqMessage::byteArrayToLEInt16(const QByteArray &array)
{
	bool ok;
	quint16 tmp = array.toHex().toUInt(&ok,16);
	return ((tmp % 0x100) * 0x100 + (tmp)/ 0x100);	
}

void icqMessage::readData(icqBuffer *socket, quint16 l)
{
	msgCookie = msgIdCookie = socket->read(8);
	quint16 channel = byteArrayToInt16(socket->read(2));
	quint8 length = byteArrayToInt8(socket->read(1));
	from = socket->read(length);

	socket->read(2);
	quint16 numberOfTlv = byteArrayToInt16(socket->read(2));
	l = l - length - 15;
	tlv rendezvousData;
	for ( int i = 0; i < numberOfTlv; i++)
	{
		tlv newTlv;
		newTlv.readData(socket);
		l -= newTlv.getLength();
	}
	
	
	if ( channel == 0x0002)
	{
		for ( ; l > 0;)
	
		{
			
			tlv newTlv;
			newTlv.readData(socket);
			l -= newTlv.getLength();
			if (newTlv.getTlvType() == 0x0005)
					rendezvousData = newTlv;
		}
	}
	
	
	switch ( channel )
	{
	case 0x0001:
		messageType = 0;
		l -= readPlainText(socket);
		
		break;
		
	case 0x0002:
		
		readRendezvousData(rendezvousData);
		break;
	default:
		break;
	}
	if ( l )
	{	
		socket->read(l);
	}
}

quint16 icqMessage::readPlainText(icqBuffer *socket)
{
	quint16 length = 0;
	length += 2;
	quint16 msgData = byteArrayToInt16(socket->read(2));
	if ( msgData != 0x0002 )
		return length;
	length += 6;
	socket->read(4);
	quint16 capLength = byteArrayToInt16(socket->read(2));
	socket->read(capLength);
	length += capLength;
	socket->read(2);
	quint16 msgLength = byteArrayToInt16(socket->read(2));
	length += 4;	
	quint16 charset = byteArrayToInt16(socket->read(2));
	socket->read(2);
//	QTextCodec *codec = 
	if ( charset != 0x0002 )
		msg = codec->toUnicode(socket->read(msgLength - 4));
	else if ( charset == 0x0002 )
	{
		msg = unicodeToUtf8(socket->read(msgLength - 4));
	}
//	msg = QString::fromUtf8(socket->read(msgLength - 4));
	length += msgLength;
	return length;
}

QString icqMessage::unicodeToUtf8(const QByteArray &array)
{
	QByteArray msg;
	 for ( int i = 0; i < array.size(); i +=2)
	 {
		 quint16 symbol = static_cast<quint16>(array.at(i) * 0x100 + array.at(i+1));
		 if ( symbol < 0x0080)
			 msg.append((unsigned char) symbol);
		 else if (symbol < 0x0800) {
			 msg.append(0xc0 | ( symbol >> 6));
			 msg.append(0x80 | (symbol & 0x3f));
		 } else {
			 msg.append(0xe0 | ( symbol >> 12));
			 msg.append(0x80 | ((symbol >>6 )& 0x3f));
			 msg.append(0x80 | (symbol & 0x3f));
		 }
	 }
	 return QString::fromUtf8(msg);
}

QByteArray icqMessage::utf8toUnicode(const QString &message)
{
	QByteArray msg;
	const ushort *array = message.utf16();
	for(;*array;array++)
			msg.append(convertToByteArray((quint16)*array));
	return msg;
}

QByteArray icqMessage::utf8toUnicodeLE(const QString &message)
{
	QByteArray msg;
	const ushort *array = message.utf16();
	for(;*array;array++)
			msg.append(convertLEToByteArray((quint16)*array));
	return msg;
}


void icqMessage::sendMessage(QTcpSocket *tcpSocket, const messageFormat &message, quint16 flapSeq, quint32 snacSeq, bool utf8)
{
	
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapSeq));
	QByteArray messageSnac;
	snac snac0406;
	snac0406.setFamily(0x0004);
	snac0406.setSubType(0x0006);
	snac0406.setReqId(snacSeq);
	messageSnac.append(snac0406.getData());
	quint32 msgcookie = QTime::currentTime().hour() * QTime::currentTime().minute()
	* QTime::currentTime().second() * QTime::currentTime().msec();
	msgCookie.append(convertToByteArray(msgcookie));
	
//	messageSnac.append(convertToByteArray((quint32)0x41C01C9E));
//	messageSnac.append(convertToByteArray(msgcookie));
	msgcookie = qrand();
	msgCookie.append(convertToByteArray(msgcookie));
	messageSnac.append(msgCookie);
//	messageSnac.append(convertToByteArray((quint32)0x2DD4DA21));
	messageSnac.append(convertToByteArray((quint16)0x0001));
	messageSnac.append(convertToByteArray((quint8)message.fromUin.size()));
	messageSnac.append(message.fromUin);
	tlv messageData;
	messageData.setType(0x0002);
	QByteArray tlvMessage;
	tlvMessage.append(convertToByteArray((quint16)0x0501));
	tlvMessage.append(convertToByteArray((quint16)0x0001));
	tlvMessage.append(convertToByteArray((quint8)0x01));
	tlvMessage.append(convertToByteArray((quint16)0x0101));
	
	QByteArray msg;
	if ( utf8 )
		msg.append(utf8toUnicode(message.message));
	else
		msg.append(codec->fromUnicode(message.message));
	
	tlvMessage.append(convertToByteArray((quint16)(4 + msg.size())));
	if ( utf8 )
		tlvMessage.append(convertToByteArray((quint16)0x0002));
	else
		tlvMessage.append(convertToByteArray((quint16)0x0000));
	tlvMessage.append(convertToByteArray((quint16)0x0000));
	tlvMessage.append(msg);
	messageData.setData(tlvMessage);
	messageSnac.append(messageData.getData());
	messageSnac.append(convertToByteArray((quint16)0x0003));
	messageSnac.append(convertToByteArray((quint16)0x0000));
	messageSnac.append(convertToByteArray((quint16)0x0006));
	messageSnac.append(convertToByteArray((quint16)0x0000));
	packet.append(convertToByteArray((quint16)messageSnac.size()));
	packet.append(messageSnac);
	tcpSocket->write(packet);
	
}

void icqMessage::sendMessageChannel2(QTcpSocket *tcpSocket, const messageFormat &message, quint16 flapSeq, quint32 snacSeq, bool utf8)
{
	
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapSeq));
	QByteArray messageSnac;
	snac snac0406;
	snac0406.setFamily(0x0004);
	snac0406.setSubType(0x0006);
	snac0406.setReqId(snacSeq);
	messageSnac.append(snac0406.getData());
	quint32 msgcookie = QTime::currentTime().hour() * QTime::currentTime().minute()
	* QTime::currentTime().second() * QTime::currentTime().msec();
	msgCookie.append(convertToByteArray(msgcookie));
	
//	messageSnac.append(convertToByteArray((quint32)0x41C01C9E));
//	messageSnac.append(convertToByteArray(msgcookie));
	msgcookie = qrand();
	msgCookie.append(convertToByteArray(msgcookie));
	messageSnac.append(msgCookie);
//	messageSnac.append(convertToByteArray((quint32)0x2DD4DA21));
	messageSnac.append(convertToByteArray((quint16)0x0002));
	messageSnac.append(convertToByteArray((quint8)message.fromUin.size()));
	messageSnac.append(message.fromUin);
	tlv messageData;
	messageData.setType(0x0005);
	QByteArray tlvMessage;
	tlvMessage.append(convertToByteArray((quint16)0x0000));
	tlvMessage.append(msgCookie);
	tlvMessage.append(QByteArray::fromHex("094613494c7f11d18222444553540000"));
	
	
	tlv tlv0a;
	tlv0a.setType(0x000a);
	tlv0a.setData((quint16)1);
	tlvMessage.append(tlv0a.getData());


	tlvMessage.append(convertToByteArray((quint16)0x000f));
	tlvMessage.append(convertToByteArray((quint16)0x0000));

	tlvMessage.append(convertToByteArray((quint16)0x2711));
	
	QByteArray extentionData;
	extentionData.append(convertToByteArray((quint16)0x1b00));
	extentionData.append(convertToByteArray((quint16)0x0a00));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertToByteArray((quint32)0x03000000));
	extentionData.append(convertToByteArray((quint8)0x00));
	extentionData.append(downCounter1);
	extentionData.append(convertToByteArray((quint16)0x0e00));
	extentionData.append(downCounter2);
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint8)0x01));
	extentionData.append(convertToByteArray((quint8)0x00));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertToByteArray((quint16)0x0400));

	QByteArray converted_message = message.message.toUtf8();
	converted_message.append(QChar(0x00));

	extentionData.append(convertLEToByteArray((quint16)converted_message.length()));
	extentionData.append(converted_message);

		
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0xffffff00));
	extentionData.append(convertToByteArray((quint32)0x26000000));
	extentionData.append(convertToByteArray((quint32)0x7b303934));
	extentionData.append(convertToByteArray((quint32)0x36313334));
	extentionData.append(convertToByteArray((quint32)0x452d3443));
	extentionData.append(convertToByteArray((quint32)0x37462d31));
	extentionData.append(convertToByteArray((quint32)0x3144312d));
	extentionData.append(convertToByteArray((quint32)0x38323232));
	extentionData.append(convertToByteArray((quint32)0x2d343434));
	extentionData.append(convertToByteArray((quint32)0x35353335));
	extentionData.append(convertToByteArray((quint32)0x34303030));
	extentionData.append(convertToByteArray((quint16)0x307d));

	tlvMessage.append(convertToByteArray((quint16)extentionData.length()));
	tlvMessage.append(extentionData);
	

	
	messageData.setData(tlvMessage);
	messageSnac.append(messageData.getData());
	messageSnac.append(convertToByteArray((quint16)0x0003));
	messageSnac.append(convertToByteArray((quint16)0x0000));
	messageSnac.append(convertToByteArray((quint16)0x0006));
	messageSnac.append(convertToByteArray((quint16)0x0000));
	packet.append(convertToByteArray((quint16)messageSnac.size()));
	packet.append(messageSnac);
	tcpSocket->write(packet);
	
}

QByteArray icqMessage::serverRelaying()
{
	QByteArray packet;
	packet.append(convertToByteArray((quint32)0x09461349));
	packet.append(convertToByteArray((quint32)0x4C7F11D1));
	packet.append(convertToByteArray((quint32)0x82224445));
	packet.append(convertToByteArray((quint32)0x53540000));
	return packet;
}

quint16 icqMessage::readRendezvousData(tlv tlv05)
{
	
	
	QByteArray data05 = tlv05.getTlvData();
	quint16 length = 26;
	reason = byteArrayToInt16(data05.left(2));
	data05 = data05.right(data05.size() - 2);
	msgCookie = data05.left(8);
	
	data05 = data05.right(data05.size() - 8);
	
	QByteArray msgCapab = data05.left(16);
	data05 = data05.right(data05.size() - 16);
		
/*	if ( msgCapab == QByteArray::fromHex("094613434c7f11d18222444553540000"))
	{
		fileAnswer = true;
		if ( reason )
			return length;
        }*/
	
	
		quint16 leftsize = data05.size();
		bool extentionDataPresent = false;
		QByteArray extentionData;
		
		for(;leftsize > 0;)
		{
			quint16 tlvType = byteArrayToInt16(data05.left(2));
			data05 = data05.right(data05.size() - 2);
			
			quint16 tlvLength = byteArrayToInt16(data05.left(2));
			data05 = data05.right(data05.size() - 2);
			
			QByteArray tlvData = data05.left(tlvLength);
			data05 = data05.right(data05.size() - tlvLength);
			
			leftsize  = leftsize - 4 - tlvData.size();
			length = length + 4 + tlvData.size();
			
			if ( tlvType == 0x000a )
			{
				connectToPeer = byteArrayToInt16(tlvData);
			}
			
			if ( tlvType == 0x2711 )
			{
				extentionData = tlvData;
				extentionDataPresent = true;
			}
			
			if ( tlvType == 0x0003)
				peerIP = byteArrayToInt32(tlvData);
			
			if ( tlvType == 0x0002)
				aolProxyIP = byteArrayToInt32(tlvData);
			
			if ( tlvType == 0x0005)
				peerPort = byteArrayToInt16(tlvData);
			
		}
		
		
		
/*		if ( fileAnswer && extentionDataPresent)
		{
			quint16 folowingLength = extentionData.size();
			
			
			extentionData = extentionData.right(extentionData.size() - 4 );
			
			fileSize = byteArrayToInt32(extentionData.left(4));
			extentionData = extentionData.right(extentionData.size() - 4 );
			
	
			fileName = QString::fromUtf8(extentionData.left(folowingLength - 9));
			extentionData = extentionData.right(extentionData.size() - 8 - folowingLength );
			
			return length;
			
			
                }*/
		
//		if ( !extentionDataPresent )
//			return length;
		
		quint16 folowingLength = byteArrayToLEInt16(extentionData.left(2));
		extentionData = extentionData.right(extentionData.size() - (folowingLength ));
		
		downCounter1 = extentionData.left(2);
		extentionData = extentionData.right(extentionData.size() - 2 );
		
		quint16 folowingLength2 = byteArrayToLEInt16(extentionData.left(2));
		
		downCounter2 = extentionData.left(2);
		extentionData = extentionData.right(extentionData.size() - 2 );
		
		extentionData = extentionData.right(extentionData.size() - (folowingLength2 ));
				
		
		
		length += 4 + folowingLength + folowingLength2;
		
		msgType = byteArrayToInt8(extentionData.left(1));
		extentionData = extentionData.right(extentionData.size() - 1);
		
		
		if ( msgType == 0xe8  || msgType == 0xE9 || msgType ==  0xEA ||  msgType ==  0xEB )
		{
			
			messageType = 1;
			extentionData = extentionData.right(extentionData.size() - 5);
			
			quint16 messageLength = byteArrayToLEInt16(extentionData.left(2));
			extentionData = extentionData.right(extentionData.size() - 2);
			
			
			extentionData = extentionData.right(extentionData.size() - messageLength);
			
		} else if (msgType == 0x06)
		{
			messageType = 2;
		} else if ( msgType == 0x04 )
		{
			messageType = 3;
		} else if ( msgType == 0x01)
		{
			messageType = 4;
			extentionData = extentionData.right(extentionData.size() - 5);
			
			quint16 messageLength = byteArrayToLEInt16(extentionData.left(2));
			extentionData = extentionData.right(extentionData.size() - 2);
			
			msg = QString::fromUtf8(extentionData.left(messageLength - 1));
			byteArrayMsg = extentionData.left(messageLength - 1);
			extentionData = extentionData.right(extentionData.size() - messageLength);
			
		} else if (msgType == 0x25 && msgCapab != QByteArray::fromHex("69716d7561746769656d000000000000"))
		{
			messageType = 4;
			
			extentionData = extentionData.right(extentionData.size() - 5);
			
			quint16 messageLength = byteArrayToLEInt16(extentionData.left(2));
			extentionData = extentionData.right(extentionData.size() - 2);
			
			
			byteArrayMsg = extentionData.left(messageLength);
			
			extentionData = extentionData.right(extentionData.size() - messageLength);
		} else if (msgType == 0x1a)
		{
			if ( msgCapab == QByteArray::fromHex("094613494c7f11d18222444553540000"))
				messageType = 7;
		} else if (msgType == 0x25 && msgCapab == QByteArray::fromHex("69716d7561746769656d000000000000"))
		{
			messageType = 8;
			extentionData = extentionData.right(extentionData.size() - 5);
			
			quint16 messageLength = byteArrayToLEInt16(extentionData.left(2));
			extentionData = extentionData.right(extentionData.size() - 2);
			
			
			byteArrayMsg = extentionData.left(messageLength);
			
			extentionData = extentionData.right(extentionData.size() - messageLength);
		}
		else
			messageType = 9;
		
	
		
		return length;
}

void icqMessage::sendAutoreply(QTcpSocket *tcpSocket, const QString &message,quint16 flapSeq, quint32 snacSeq)
{

	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapSeq));
	QByteArray messageSnac;
	snac snac040b;
	snac040b.setFamily(0x0004);
	snac040b.setSubType(0x000b);
	snac040b.setReqId(snacSeq);
	messageSnac.append(snac040b.getData());
	messageSnac.append(msgCookie);
	
	messageSnac.append(convertToByteArray((quint16)0x0002));
	messageSnac.append(convertToByteArray((quint8)from.size()));
	messageSnac.append(from);
	messageSnac.append(convertToByteArray((quint16)0x0003));
	QByteArray extentionData;
	extentionData.append(convertToByteArray((quint16)0x1b00));
	extentionData.append(convertToByteArray((quint16)0x0800));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertToByteArray((quint32)0x03000000));
	extentionData.append(convertToByteArray((quint8)0x00));
	extentionData.append(downCounter1);
	extentionData.append(convertToByteArray((quint16)0x0e00));
	extentionData.append(downCounter2);
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint8)msgType));
	extentionData.append(convertToByteArray((quint8)0x03));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertLEToByteArray((quint16)message.size()));
	extentionData.append(codec->fromUnicode(message));
	messageSnac.append(extentionData);
	
	
	packet.append(convertToByteArray((quint16)messageSnac.size()));
	packet.append(messageSnac);
	
	tcpSocket->write(packet);
	
}

void icqMessage::requestAutoreply(QTcpSocket *tcpSocket, const QString &uin,quint16 flapSeq, quint32 snacSeq)
{
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapSeq));
	QByteArray messageSnac;
	snac snac0406;
	snac0406.setFamily(0x0004);
	snac0406.setSubType(0x0006);
	snac0406.setReqId(snacSeq);
	messageSnac.append(snac0406.getData());
	
	quint32 msgcookie = QTime::currentTime().hour() * QTime::currentTime().minute()
	* QTime::currentTime().second() * QTime::currentTime().msec();
	
	msgCookie.clear();
	
	msgCookie.append(convertToByteArray((quint32)msgcookie));
	
	
	
	msgcookie = qrand();
	
	msgCookie.append(convertToByteArray((quint32)msgcookie));
	
	messageSnac.append(msgCookie);
	
	messageSnac.append(convertToByteArray((quint16)0x0002));
	
	
	messageSnac.append(convertToByteArray((quint8)uin.size()));
	messageSnac.append(uin);
	
	
	
	
	QByteArray extentionData;
	extentionData.append(convertToByteArray((quint16)0x1b00));
	extentionData.append(convertToByteArray((quint16)0x0800));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertToByteArray((quint32)0x03000000));
	extentionData.append(convertToByteArray((quint8)0x00));
	extentionData.append(downCounter1);
	extentionData.append(convertToByteArray((quint16)0x0e00));
	extentionData.append(downCounter2);
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	if ( awayType != 0x1a )
	{
		extentionData.append(convertToByteArray((quint8)awayType));
		extentionData.append(convertToByteArray((quint8)0x03));
		extentionData.append(convertToByteArray((quint16)0x0000));
		extentionData.append(convertToByteArray((quint16)0x0000));
		extentionData.append(convertLEToByteArray((quint16)1));
		extentionData.append(convertToByteArray((quint8)0));
	} else {
		extentionData.append(convertToByteArray((quint8)0x1a));
		extentionData.append(convertToByteArray((quint8)0));
		extentionData.append(convertToByteArray((quint16)0x0000));
		extentionData.append(convertToByteArray((quint16)0x0001));
		extentionData.append(convertToByteArray((quint16)0x0000));
		extentionData.append(QByteArray::fromHex("3a00811a18bc0e6c1847a5916f18dcc76f1a01001"
				"30000004177617920537461747573204d6573736167650100000000000000000000000000000"
				"00015000000000000000d000000746578742f782d616f6c727466"));
	}
	
	
	messageSnac.append(convertToByteArray((quint16)0x0005));
	
	messageSnac.append(convertToByteArray((quint16)(40 + extentionData.length())));
	
	messageSnac.append(convertToByteArray((quint16)0x0000));
	messageSnac.append(msgCookie);
	messageSnac.append(QByteArray::fromHex("094613494c7f11d18222444553540000"));
	
	messageSnac.append(convertToByteArray((quint16)0x000a));
	messageSnac.append(convertToByteArray((quint16)0x0002));
	messageSnac.append(convertToByteArray((quint16)0x0001));
	messageSnac.append(convertToByteArray((quint16)0x000f));
	messageSnac.append(convertToByteArray((quint16)0x0000));
	
	messageSnac.append(convertToByteArray((quint16)0x2711));
	messageSnac.append(convertToByteArray((quint16)extentionData.length()));
	
	messageSnac.append(extentionData);
	
	
	packet.append(convertToByteArray((quint16)messageSnac.size()));
	packet.append(messageSnac);
	
	tcpSocket->write(packet);
}


void icqMessage::getAwayMessage(icqBuffer *socket, quint16 length)
{
	msgCookie = socket->read(8);
	
	length -= 8;
	
	quint16 channel = byteArrayToInt16(socket->read(2));
	length -= 2;
	
	quint8 uinLength = byteArrayToInt8(socket->read(1));
	length--;
	
	from = socket->read(uinLength);
	length -= uinLength;
	
	quint16 reason = byteArrayToInt16(socket->read(2));
	length -= 2;
	
	if ( channel == 0x02)
	{
		socket->read(29);
		length -= 29;
		
		socket->read(16);
		length -= 16;
		
		msgType = byteArrayToInt8(socket->read(1));
		length --;
		
		socket->read(5);
		length -= 5;
		
		quint16 messageLength = byteArrayToLEInt16(socket->read(2));
		length -= 2;
		
		if ( messageLength )
		{
		
			msg = codec->toUnicode(socket->read(messageLength - 1));
			
			socket->read(1);
		}
		length -= messageLength;
		
		if ( msgType == 0x1a)
		{

			byteArrayMsg = socket->read(length);
			length -= length;
			if ( byteArrayMsg.contains(QByteArray::fromHex("4177617920537461747573204d657373616765")))
			{
				byteArrayMsg = byteArrayMsg.right(byteArrayMsg.length() - 68);
//				QString _givenText = byteArrayMsg;
//				QRegExp regExp("[<][B][O][D][Y][>](.+)[<][/][B][O][D][Y][>]"); 
//				int pos = 0; 
//				regExp.indexIn(_givenText, pos); 
//				QString rez = regExp.cap(0);
//
//				qDebug()<<rez.mid(6, rez.length() - 13);
////				qDebug()<<byteArrayMsg.toHex();
//				QString tmpMsg= codec->toUnicode(byteArrayMsg);
//				qDebug()<<tmpMsg;
//				byteArrayMsg = tmpMsg.toUtf8();
				QString tmpMsg= codec->toUnicode(byteArrayMsg);
				byteArrayMsg.clear();
				byteArrayMsg.append(QByteArray::fromHex("4177617920537461747573204d657373616765"));
				
				byteArrayMsg.append(tmpMsg.toUtf8());

			}
		}
		
	}
	
	if ( length )
		socket->read(length);
}

void icqMessage::sendImage(QTcpSocket *tcpSocket, const QString &uin, const QByteArray &image_raw, quint16 flapSeq,
			quint32 snacSeq)
{
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapSeq));
	QByteArray messageSnac;
	snac snac0406;
	snac0406.setFamily(0x0004);
	snac0406.setSubType(0x0006);
	snac0406.setReqId(snacSeq);
	messageSnac.append(snac0406.getData());
	
	QByteArray rndzvCookie;
	quint32 msgcookie = QTime::currentTime().hour() * QTime::currentTime().minute()
	* QTime::currentTime().second() * QTime::currentTime().msec();
	rndzvCookie.append(convertToByteArray(msgcookie));
	msgcookie = qrand();
	rndzvCookie.append(convertToByteArray(msgcookie));
	messageSnac.append(rndzvCookie);
	messageSnac.append(convertToByteArray((quint16)0x0002));
	
	
	messageSnac.append(convertToByteArray((quint8)uin.size()));
	messageSnac.append(uin);
	
	
	
	QByteArray rndzvData;
	
	
	tlv tlv0a;
	tlv0a.setType(0x000a);
	tlv0a.setData((quint16)1);
	rndzvData.append(tlv0a.getData());
	rndzvData.append(convertToByteArray((quint16)0x000f));
	rndzvData.append(convertToByteArray((quint16)0x0000));
	rndzvData.append(convertToByteArray((quint16)0x2711));
	
	

	
	QByteArray extentionData;
	extentionData.append(convertToByteArray((quint16)0x1b00));
	extentionData.append(convertToByteArray((quint16)0x0900));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertToByteArray((quint32)0x03000000));
	extentionData.append(convertToByteArray((quint8)0x00));
	extentionData.append(downCounter1);
	extentionData.append(convertToByteArray((quint16)0x0e00));
	extentionData.append(downCounter2);
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint8)0x25));
	extentionData.append(convertToByteArray((quint8)0x00));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertToByteArray((quint16)0x0000));
	if ( !image_raw.isEmpty() )
	{
		extentionData.append(convertLEToByteArray((quint16)(image_raw.size())));
		extentionData.append(image_raw);
	} else {
		extentionData.append(convertLEToByteArray((quint16)1));
		extentionData.append(convertToByteArray((quint8)0));
	}
	
	rndzvData.append(convertToByteArray((quint16)extentionData.length()));
	rndzvData.append(extentionData);
	
	messageSnac.append(convertToByteArray((quint16)0x0005));
	messageSnac.append(convertToByteArray((quint16)(26 + rndzvData.length())));
	messageSnac.append(convertToByteArray((quint16)0x0000));
	messageSnac.append(rndzvCookie);
	messageSnac.append(QByteArray::fromHex("69716d7561746769656d000000000000"));
	messageSnac.append(rndzvData);
	
	
	packet.append(convertToByteArray((quint16)messageSnac.size()));
	packet.append(messageSnac);
	tcpSocket->write(packet);
}

void icqMessage::sendXstatusReply(QTcpSocket *tcpSocket, const QString &uin, const QString &profile_name, quint16 flapSeq, quint32 snacSeq)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+profile_name+"/ICQ."+uin, "accountsettings");
	
	
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapSeq));
	QByteArray messageSnac;
	snac snac040b;
	snac040b.setFamily(0x0004);
	snac040b.setSubType(0x000b);
	snac040b.setReqId(snacSeq);
	messageSnac.append(snac040b.getData());
	messageSnac.append(msgCookie);
	
	messageSnac.append(convertToByteArray((quint16)0x0002));
	messageSnac.append(convertToByteArray((quint8)from.size()));
	messageSnac.append(from);
	messageSnac.append(convertToByteArray((quint16)0x0003));
	QByteArray extentionData;
	extentionData.append(convertToByteArray((quint16)0x1b00));
	extentionData.append(convertToByteArray((quint16)0x0800));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertToByteArray((quint32)0x03000000));
	extentionData.append(convertToByteArray((quint8)0x00));
	extentionData.append(downCounter1);
	extentionData.append(convertToByteArray((quint16)0x0e00));
	extentionData.append(downCounter2);
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint8)0x1a));
	extentionData.append(convertToByteArray((quint8)0x00));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertLEToByteArray((quint16)1));
	extentionData.append(convertToByteArray((quint8)0));
	QByteArray xstatusreply;
	
	xstatusreply.append(QByteArray::fromHex("53637269707420506c75672d696e3a2052656d6f7465204e6f74696669636174696f6e20417272697665000001000000000000000000000000f2010000ee010000"
			"3c4e523e3c5245533e266c743b726574206576656e743d274f6e52656d6f74654e6f74696669636174696f6e272667743b266c743b7372762667743b266c743b69642667743b634"
			"1776179537276266c743b2f69642667743b266c743b76616c207372765f69643d276341776179537276272667743b266c743b526f6f742667743b266c743b4341535874726153657"
			"4417761794d6573736167652667743b266c743b2f43415358747261536574417761794d6573736167652667743b266c743b75696e2667743b"));
	
	xstatusreply.append(uin);
	xstatusreply.append(QByteArray::fromHex("266c743b2f75696e2667743b266c743b696e6465782667743b"));
	xstatusreply.append(QString::number(settings.value("xstatus/index",0).toInt()));
	xstatusreply.append(QByteArray::fromHex("266c743b2f696e6465782667743b266c743b7469746c652667743b"));
	xstatusreply.append(settings.value("xstatus/caption","").toString().toUtf8());
	xstatusreply.append(QByteArray::fromHex("266c743b2f7469746c652667743b266c743b646573632667743b"));
	xstatusreply.append(settings.value("xstatus/message","").toString().toUtf8());
	xstatusreply.append(QByteArray::fromHex("266c743b2f646573632667743b266c743b2f526f6f742667743b0d0a266c743b2f76616c2667743b266c743b2f7372762667743b266c743b7372762667743b266c743b69642667743b635261"
			"6e646f6d697a6572537276266c743b2f69642667743b266c743b76616c207372765f69643d276352616e646f6d697a6572537276272667743b756e646566696e6564266c743b2f76616c2667743b266c743b2"
			"f7372762667743b266c743b2f7265742667743b3c2f5245533e3c2f4e523e0d0a"));
	
	extentionData.append(QByteArray::fromHex("4f003b60b3efd82a6c45a4 e09c5a5e67e86508002a000000"));
	extentionData.append(xstatusreply);
	messageSnac.append(extentionData);
	
	
	packet.append(convertToByteArray((quint16)messageSnac.size()));
	packet.append(messageSnac);
	
	tcpSocket->write(packet);
}

void icqMessage::requestXStatus(QTcpSocket *tcpSocket, const QString &uin,
		const QString &mine_uin, quint16 flapSeq, quint32 snacSeq)
{
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapSeq));
	QByteArray messageSnac;
	snac snac0406;
	snac0406.setFamily(0x0004);
	snac0406.setSubType(0x0006);
	snac0406.setReqId(snacSeq);
	messageSnac.append(snac0406.getData());
	
	quint32 msgcookie = QTime::currentTime().hour() * QTime::currentTime().minute()
	* QTime::currentTime().second() * QTime::currentTime().msec();
	
	msgCookie.clear();
	
	msgCookie.append(convertToByteArray((quint32)msgcookie));
	
	
	
	msgcookie = qrand();
	
	msgCookie.append(convertToByteArray((quint32)msgcookie));
	
	messageSnac.append(msgCookie);
	
	messageSnac.append(convertToByteArray((quint16)0x0002));
	
	
	messageSnac.append(convertToByteArray((quint8)uin.size()));
	messageSnac.append(uin);
	
	
	
	
	QByteArray extentionData;
	extentionData.append(convertToByteArray((quint16)0x1b00));
	extentionData.append(convertToByteArray((quint16)0x0a00));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertToByteArray((quint32)0x03000000));
	extentionData.append(convertToByteArray((quint8)0x00));
	extentionData.append(downCounter1);
	extentionData.append(convertToByteArray((quint16)0x0e00));
	extentionData.append(downCounter2);
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint32)0x00000000));
	extentionData.append(convertToByteArray((quint8)0x1a));
	extentionData.append(convertToByteArray((quint8)0x00));
	extentionData.append(convertToByteArray((quint16)0x0000));
	extentionData.append(convertToByteArray((quint16)0x0100));
	extentionData.append(convertLEToByteArray((quint16)1));
	extentionData.append(convertToByteArray((quint8)0));
	
	
	
	QByteArray xStatusString;
//	xStatusString.append(QByteArray::fromHex("4f003b60b3efd82a6c45a4 e09c5a5e67e86508002a000000"));
//	
//	xStatusString.append(QByteArray::fromHex("53637269707420506c75672d696e3a2052656d6f7465204e6f74696669636174696f6e2041727269766500000100000000000000000000000016010000120100003c4e3e3c51554552593e266c743b512667743b266c743b506c"
//			"7567696e49442667743b7372764d6e67266c743b2f506c7567696e49442667743b266c743b2f512667743b3c2f51554552593e3c4e4f544946593e266c743b7372762667743b266c743b69642667743b6341776179537276266c743"
//			"b2f69642667743b266c743b7265712667743b266c743b69642667743b4177617953746174266c743b2f69642667743b266c743b7472616e732667743b3135266c743b2f7472616e732667743b266c743b73656e646572"));
	xStatusString.append(QByteArray::fromHex("4f003b60b3efd82a6c45a4e09c5a5e67e86508002a0000005363"
			"7269707420506c75672d696e3a2052656d6f7465204e6f74696669636174696f6"
			"e2041727269766500000100000000000000000000000015010000110100003c4e"
			"3e3c51554552593e266c743b512667743b266c743b506c7567696e49442667743"
			"b7372764d6e67266c743b2f506c7567696e49442667743b266c743b2f51266774"
			"3b3c2f51554552593e3c4e4f544946593e266c743b7372762667743b266c743b6"
			"9642667743b6341776179537276266c743b2f69642667743b266c743b72657126"
			"67743b266c743b69642667743b4177617953746174266c743b2f69642667743b2"
			"66c743b7472616e732667743b39266c743b2f7472616e732667743b266c743b73"
			"656e64657249642667743b"));
	xStatusString.append(mine_uin);
	xStatusString.append(QByteArray::fromHex("266c743b2f73656e64657249642667743b266c743b2f72657126"
			"67743b266c743b2f7372762667743b3c2f4e4f544946593e3c2f4e3e0d0a"));
//	xStatusString.append(QByteArray::fromHex("266c743b2f73656e64657249642667743b266c743b2f7265712667743b266c743b2f7372762667743b3c2f4e4f544946593e3c2f4e3e0d0a"));
	
	
	extentionData.append(xStatusString);
	
	messageSnac.append(convertToByteArray((quint16)0x0005));
	
	messageSnac.append(convertToByteArray((quint16)(40 + extentionData.length())));
	
	messageSnac.append(convertToByteArray((quint16)0x0000));
	messageSnac.append(msgCookie);
	messageSnac.append(QByteArray::fromHex("094613494c7f11d18222444553540000"));
	
	messageSnac.append(convertToByteArray((quint16)0x000a));
	messageSnac.append(convertToByteArray((quint16)0x0002));
	messageSnac.append(convertToByteArray((quint16)0x0001));
	messageSnac.append(convertToByteArray((quint16)0x000f));
	messageSnac.append(convertToByteArray((quint16)0x0000));
	
	messageSnac.append(convertToByteArray((quint16)0x2711));
	messageSnac.append(convertToByteArray((quint16)extentionData.length()));
	
	messageSnac.append(extentionData);
	messageSnac.append(convertToByteArray((quint16)0x0003));
	messageSnac.append(convertToByteArray((quint16)0x0000));
	
	
	packet.append(convertToByteArray((quint16)messageSnac.size()));
	packet.append(messageSnac);
	
	tcpSocket->write(packet);
}

void icqMessage::sendMessageRecieved(QTcpSocket *socket,const QString &uin, const QByteArray &cookie, quint16 flapSeq, quint32 snacSeq)
{

		QByteArray packet;
		packet[0] = 0x2a;
		packet[1] = 0x02;
		packet.append(convertToByteArray((quint16)flapSeq));
		QByteArray messageSnac;
		snac snac040b;
		snac040b.setFamily(0x0004);
		snac040b.setSubType(0x000b);
		snac040b.setReqId(snacSeq);
		messageSnac.append(snac040b.getData());
		
		messageSnac.append(cookie);
		
		messageSnac.append(convertToByteArray((quint16)0x0002));
		
		
		messageSnac.append(convertToByteArray((quint8)uin.size()));
		messageSnac.append(uin);
		
		messageSnac.append(convertToByteArray((quint16)0x0003));
		
		
		QByteArray extentionData;
		extentionData.append(convertToByteArray((quint16)0x1b00));
		extentionData.append(convertToByteArray((quint16)0x0900));
		extentionData.append(convertToByteArray((quint32)0x00000000));
		extentionData.append(convertToByteArray((quint32)0x00000000));
		extentionData.append(convertToByteArray((quint32)0x00000000));
		extentionData.append(convertToByteArray((quint32)0x00000000));
		extentionData.append(convertToByteArray((quint16)0x0000));
		extentionData.append(convertToByteArray((quint32)0x01000000));
		extentionData.append(convertToByteArray((quint8)0x00));
		extentionData.append(downCounter1);
		extentionData.append(convertToByteArray((quint16)0x0e00));
		extentionData.append(downCounter2);
		extentionData.append(convertToByteArray((quint32)0x00000000));
		extentionData.append(convertToByteArray((quint32)0x00000000));
		extentionData.append(convertToByteArray((quint32)0x00000000));

			extentionData.append(convertToByteArray((quint8)0x01));
			extentionData.append(convertToByteArray((quint8)0x00));
			extentionData.append(convertToByteArray((quint16)0x0000));
			extentionData.append(convertToByteArray((quint16)0x0000));
			extentionData.append(convertLEToByteArray((quint16)1));
			extentionData.append(convertToByteArray((quint8)0));
			extentionData.append(convertToByteArray((quint32)0x00000000));
			extentionData.append(convertToByteArray((quint32)0xffffff00));
		
		

		
		messageSnac.append(extentionData);
		
		
		packet.append(convertToByteArray((quint16)messageSnac.size()));
		packet.append(messageSnac);
		
		socket->write(packet);
	
}

bool icqMessage::isValidUtf8(const QByteArray &array)
{
    int i=0, num;
    while(i < array.size())
    {
        if(array[i]&0x80)
            return false;
        else
        {
            num = 0;
            if(array[i]&0xE0 == 0xC0)
                num = 1;
            else if(array[i]&0xF0 == 0xE0)
                num = 2;
            else if(array[i]&0xF8 == 0xE0)
                num = 3;
            else
                return false;
            if(i+num >= array.size())
                return false;
            while(num--)
                if(array[++i]&0xC0 != 0x80)
                    return false;
        }
        i++;
    }
    return true;
}
