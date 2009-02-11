/*
    closeConnection

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
#include <QHostAddress>
#include <QStringList>

#include "tlv.h"
#include "buffer.h"

#include "closeconnection.h"

closeConnection::closeConnection(QObject *parent) : QObject(parent)
{
	
}

closeConnection::~closeConnection()
{
	
}

void closeConnection::readData(QTcpSocket * tcpSocket, icqBuffer *socket, const QString &uin)
{
	tlv icqUin;
	for ( ;socket->bytesAvailable();)
	{
		icqUin.readData(socket);
		if ( icqUin.getTlvType() == 0x0001 ) 
			break;
		if ( icqUin.getTlvType() == 0x0009 ) 
			break;
	}
	if ( icqUin.getTlvType() == 0x0009)
	{
		disconnectTakeUin(tcpSocket);
		return;
	}
	
	if( uin != QString(icqUin.getTlvData())) 
		return;
	tlv tmp;
	for(;socket->bytesAvailable();)
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
		getError(tcpSocket, socket);
	}
	
	if ( tmp.getTlvType() == 0x0008 )
	{
		char errorCode = tmp.getTlvData().at(1);
		errorMessage(static_cast<quint16>(errorCode));
		socket->readAll();
		tcpSocket->disconnectFromHost();
	}
	
	if ( tmp.getTlvType() == 0x0005 )
	{
		getBosServer(QString(tmp.getTlvData()));
		getLuck(socket);
	}
}

void closeConnection::getError(QTcpSocket *tcpSocket,icqBuffer *socket)
{
	tlv error;
	error.readData(socket);
	if ( error.getTlvType() == 0x0008 )
	{
		char errorCode = error.getTlvData().at(1);
		errorMessage(static_cast<quint16>(errorCode));
		
	}
	socket->readAll();
	tcpSocket->disconnectFromHost();
}

void closeConnection::errorMessage(const quint16 error)
{
	switch(error)
			{
			case 0x01:
				systemMessage(tr("Invalid nick or password"));
				break;
			case 0x02:
				systemMessage(tr("Service temporarily unavailable"));
				break;
			case 0x04:
				systemMessage(tr("Incorrect nick or password"));
				break;
			case 0x05:
				systemMessage(tr("Mismatch nick or password"));
				break;
			case 0x06:
				systemMessage(tr("Internal client error (bad input to authorizer)"));
				break;
			case 0x07:
				systemMessage(tr("Invalid account"));
				break;
			case 0x08:
				systemMessage(tr("Deleted account"));
				break;
			case 0x09:
				systemMessage(tr("Expired account"));
				break;
			case 0x0A:
				systemMessage(tr("No access to database"));
				break;
			case 0x0B:
				systemMessage(tr("No access to resolver"));
				break;
			case 0x0C:
				systemMessage(tr("Invalid database fields"));
				break;
			case 0x0D:
				systemMessage(tr("Bad database status"));
				break;
			case 0x0E:
				systemMessage(tr("Bad resolver status"));
				break;
			case 0x0F:
				systemMessage(tr("Internal error"));
				break;
			case 0x10:
				systemMessage(tr("Service temporarily offline"));
				break;
			case 0x11:
				systemMessage(tr(" Suspended account"));
				break;
			case 0x12:
				systemMessage(tr("DB send error"));
				break;
			case 0x13:
				systemMessage(tr("DB link error"));
				break;
			case 0x14:
				systemMessage(tr("Reservation map error"));
				break;
			case 0x15:
				systemMessage(tr("Reservation link error"));
				break;
			case 0x16:
				systemMessage(tr("The users num connected from this IP has reached the maximum"));
				break;
			case 0x17:
				systemMessage(tr(" The users num connected from this IP has reached the maximum (reservation)"));
				break;
			case 0x18:
				systemMessage(tr("Rate limit exceeded (reservation). Please try to reconnect in a few minutes"));
				break;
			case 0x19:
				systemMessage(tr("User too heavily warned"));
				break;
			case 0x1A:
				systemMessage(tr("Reservation timeout"));
				break;
			case 0x1B:
				systemMessage(tr("You are using an older version of ICQ. Upgrade required"));
				break;
			case 0x1C:
				systemMessage(tr("You are using an older version of ICQ. Upgrade recommended"));
				break;
			case 0x1D:
				systemMessage(tr("Rate limit exceeded. Please try to reconnect in a few minutes"));
				break;
			case 0x1E:
				systemMessage(tr("Can't register on the ICQ network. Reconnect in a few minutes"));
				break;
			case 0x20:
				systemMessage(tr("Invalid SecurID"));
				break;
			case 0x22:
				systemMessage(tr("Account suspended because of your age (age < 13)"));
				break;
			default:
				systemMessage(tr("Connection Error"));
			}
	
		
}

void closeConnection::getBosServer(const QString &bosServer)
{
	QStringList splitServer = bosServer.split(":");
	emit sendBosServer(QHostAddress(splitServer.at(0)));
	emit sendBosPort((quint16)splitServer.at(1).toUInt());
}

void closeConnection::getLuck(icqBuffer *socket )
{
	tlv cookie;
	cookie.readData(socket);
	emit sendCookie(cookie.getTlvData());
}

void closeConnection::disconnectTakeUin(QTcpSocket *socket)
{
	emit blockRateLimit();
	socket->disconnectFromHost();

	systemMessage(tr("Another client is loggin with this uin"));

	
}
