/*
    tlv

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

#include <QTcpSocket>
#include "buffer.h"

#include "tlv.h"

tlv::tlv()
{
	length = 0;
	type = 0;
	LE = false;
}

tlv::~tlv()
{
	
}

void tlv::setData(const QString &d)
{
	if ( d.length() < 65536)
	{
		data = d.toUtf8();
		length = (quint16)d.length();
	}	
}

void tlv::setData(const QByteArray &d)
{
	if ( d.length() < 65536)
	{
		data = d;
		length = (quint16)d.length();
	}	
}

void tlv::setData(const quint8 &d)
{
	length = 1;
	data[0] = d;
}

void tlv::setData(const quint16 &d)
{
	length = 2;
	data[0] = (d / 0x100);
	data[1] = (d % 0x100);
}

void tlv::setData(const quint32 &d)
{
	length = 4;
	data[0] = (d / 0x1000000);
	data[1] = (d / 0x10000);
	data[2] = (d / 0x100);
	data[3] = (d % 0x100);
}

quint16 tlv::getLength() const
{
	return 4 + length;
}

QByteArray tlv::getData() const
{
	QByteArray tlvPacket;
	if ( !LE )
	{
		tlvPacket[0] = type / 0x100;
		tlvPacket[1] = type % 0x100;
		tlvPacket[2] = length / 0x100;
		tlvPacket[3] = length % 0x100;
	} else 
	{
		tlvPacket[1] = type / 0x100;
		tlvPacket[0] = type % 0x100;
		tlvPacket[3] = length / 0x100;
		tlvPacket[2] = length % 0x100;
	}
	tlvPacket.append(data);
	return tlvPacket;
}

quint16 tlv::byteArrayToInt16(const QByteArray &array) const
{
	bool ok;
	return array.toHex().toUInt(&ok,16);
}

void tlv::readData(icqBuffer *socket)
{
	type = byteArrayToInt16(socket->read(2));
	length = byteArrayToInt16(socket->read(2));
	data = socket->read(length);
}

