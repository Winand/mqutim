/*
    serverLoginReply

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
#include <QStringList>
#include "tlv.h"
#include "buffer.h"

#include "serverloginreply.h"

serverLoginReply::serverLoginReply()
{
	isSnacError = false;
}

serverLoginReply::~serverLoginReply()
{
	
}

void serverLoginReply::readData(QTcpSocket *tcpSocket, icqBuffer *socket, const QString &uin)
{
	tlv icqUin;
		forever
		{
			icqUin.readData(socket);
			if ( icqUin.getTlvType() == 0x0001 ) 
				break;
		}
		if( uin != QString(icqUin.getTlvData())) 
			return;
		tlv tmp;
		forever
		{
			tmp.readData(socket);
			if ( tmp.getTlvType() == 0x0004 ) 
				break;
			if ( tmp.getTlvType() == 0x0005 ) 
				break;
			if ( tmp.getTlvType() == 0x0008 ) 
				break;
		}
		
		if ( tmp.getTlvType() == 0x0004 )
		{
			getError(socket);
			tcpSocket->disconnectFromHost();
		}
		
		if ( tmp.getTlvType() == 0x0008 )
		{
			isSnacError = true;
			errorMessage = tmp.getTlvData().at(1);
			socket->readAll();
			tcpSocket->disconnectFromHost();
		}
		
		if ( tmp.getTlvType() == 0x0005 )
		{
			getBosServer(QString(tmp.getTlvData()));
			getCookie(socket);
		}
}

void serverLoginReply::getError(icqBuffer *socket)
{
	isSnacError = true;
	tlv error;
	error.readData(socket);
		if ( error.getTlvType() == 0x0008 )
		{
			errorMessage = static_cast<quint16>(error.getTlvData().at(1));
			socket->readAll();
			
		}
}

void serverLoginReply::getBosServer(const QString &bosServer)
{
	QStringList splitServer = bosServer.split(":");
	bosIp = splitServer.at(0);
	bosPort = (quint16)splitServer.at(1).toUInt();
}

void serverLoginReply::getCookie(icqBuffer *socket)
{
	isSnacError = false;
	tlv getCookie;
	getCookie.readData(socket);
	cookie = getCookie.getTlvData();
	
}
