/*
    flapPacket

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

//#include <QtGui/QWidget>
//#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>

#include "buffer.h"
#include "flap.h"

flapPacket::flapPacket()
{
	
}

flapPacket::~flapPacket()
{
	
}

bool flapPacket::readFromSocket(icqBuffer *socket)
{
	if ( byteArrayToInt8(socket->read(1)) != 0x2a )
		return false;
	channel = byteArrayToInt8(socket->read(1));
	seqNumber = byteArrayToInt16(socket->read(2));
	length = byteArrayToInt16(socket->read(2));
	return true;
}

quint16 flapPacket::byteArrayToInt16(const QByteArray &array) const
{
	bool ok;
	return array.toHex().toUInt(&ok,16);
}

quint8 flapPacket::byteArrayToInt8(const QByteArray &array) const
{
	bool ok;
	return array.toHex().toUInt(&ok,16);
}
