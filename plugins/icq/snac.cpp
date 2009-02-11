/*
    snac

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

#include "snac.h"

snac::snac()
{
	snacFlags = 0x0000;
	familyId = 0x0000;
	subTypeId = 0x0000;
	reqId = 0x00000000;
}

snac::~snac()
{
	
}

QByteArray snac::getData()
{
	QByteArray packet;
	packet.append(convertToByteArray(familyId));
	packet.append(convertToByteArray(subTypeId));
	packet.append(convertToByteArray(snacFlags));
	packet.append(convertToByteArray(reqId));
	return packet;
}

void snac::readData(icqBuffer *socket)
{
	familyId = byteArrayToInt16(socket->read(2));
	subTypeId = byteArrayToInt16(socket->read(2));
	snacFlags = byteArrayToInt16(socket->read(2));
	reqId = byteArrayToInt32(socket->read(4));
}

QByteArray snac::convertToByteArray(const quint16 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x100);
	packet[1] = (d % 0x100);
	return packet;
}

QByteArray snac::convertToByteArray(const quint32 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x1000000);
	packet[1] = (d / 0x10000);
	packet[2] = (d / 0x100);
	packet[3] = (d % 0x100);
	return packet;
}

quint16 snac::byteArrayToInt16(const QByteArray &array)
{
	bool ok;
	return array.toHex().toUInt(&ok,16);
}

quint32 snac::byteArrayToInt32(const QByteArray &array)
{
	bool ok;
	return array.toHex().toULong(&ok,16);
}

bool snac::aloneSnac()
{
	return snacFlags & 0x0001;

}
