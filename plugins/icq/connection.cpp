/*
    connection

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
#include <QDebug>
#include <QSettings>
/*#include <QAuthenticator>*/
//#include "buffer.h"
//#include "clientIdentification.h"
//#include "tlv.h"

#include "connection.h"

connection::connection(QTcpSocket *tcpSocket, icqBuffer *buff, const QString &account, const QString &profile_name, QObject *parent)
    : QObject(parent)
    , icqUin(account)
    , socket(tcpSocket)
    , buffer(buff)
    , m_profile_name(profile_name)
{
	connectedToBos = false;
/*        connect(socket, SIGNAL(proxyAuthenticationRequired ( const QNetworkProxy & , QAuthenticator * ) ), this,
                        SLOT(proxyAuthenticationRequired ( const QNetworkProxy & , QAuthenticator *) ));*/
}

connection::~connection()
{
	
}

void connection::connectToServer(QTcpSocket *socket)
{
	
	if ( getProxy())
	{
//	socket->setProxy(currentProxy);
//	QNetworkProxy::setApplicationProxy(getProxy());
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	
	QString host = settings.value("connection/host", "login.icq.com").toString();
	quint16 port = settings.value("connection/port", 5190).toInt();
	connectedToBos = false;
	socket->connectToHost(host,port);
	}
	
}

void connection::readData(const quint16 &length)
{
	if ( length < 10 )
	{
		protocolVersion = buffer->read(4);
		if ( (length -4) > 0 )
			buffer->read(length -4);
	}
	if ( !connectedToBos )
		emit sendLogin();
}


void connection::sendIdent(const QString &password)
{
	clientIdentification clientLogin;
	clientLogin.setProcolVersion(protocolVersion);
	clientLogin.setScreenName(icqUin);
	clientLogin.setClientName("ICQ Client");
	clientLogin.setPassword(password);
	clientLogin.sendPacket(socket);
}

void connection::connectToBos(const QHostAddress &bosIp, const quint16 &port, const QByteArray cookie, const quint16 &flapSeq)
{
//	socket->disconnectFromHost();
//	if (socket->state() == QAbstractSocket::UnconnectedState)
//	{
//	socket->abort();
//		socket->setProxy(QNetworkProxy::NoProxy);
	
//	socket->setProxy(getProxy());
	
//	QNetworkProxy prox;
//	prox.set
	

    connectedToBos = true;
	emit connectingToBos();
	socket->connectToHost(bosIp, port);

	quint16 length = 4;
	tlv cookieTlv;
	cookieTlv.setType(0x0006);
	cookieTlv.setData(cookie);
	length += cookieTlv.getLength();
	QByteArray packet;
	packet[0] = 0x2A;
	packet[1] = 0x01;
	packet[2] = flapSeq / 0x100;
	packet[3] = flapSeq % 0x100;
	packet[4] = length / 0x100;
	packet[5] = length % 0x100;
	packet.append(protocolVersion);
	packet.append(cookieTlv.getData());
	socket->write(packet);
//	}
}

bool connection::getProxy()
{
	bool success = true;
/*        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	if ( settings.value("connection/useproxy", false).toBool() )
		{
			quint16 proxyType = settings.value("proxy/proxyType", 0).toInt();
			if ( proxyType )
			{
				QNetworkProxy connectionProxy;
				switch( proxyType )
				{
				case 1:
					connectionProxy.setType(QNetworkProxy::HttpProxy);
					break;
				case 2:
					connectionProxy.setType(QNetworkProxy::Socks5Proxy);
					break;
				default:
					break;
				
				}
				
				QString hostN = settings.value("proxy/host").toString();
				
				QHostAddress addr(hostN);
				if ( addr.isNull() )
				{
					success = false;
			         QHostInfo::lookupHost(hostN, const_cast<connection*>(this), SLOT(dnsResults(QHostInfo)));
					
				}
				
				connectionProxy.setHostName(hostN);
				connectionProxy.setPort(settings.value("proxy/port", 1).toInt());
				if ( settings.value("proxy/auth", false).toBool() )
				{

					connectionProxy.setUser(settings.value("proxy/user").toString());
					connectionProxy.setPassword(settings.value("proxy/pass").toString());
				}
				

				setCurrentProxy(connectionProxy);
//				currentProxy.setType(connectionProxy.type());
//				currentProxy.setHostName(connectionProxy.hostName());
//				currentProxy.setHostName("gggg");
//				currentProxy.setPort(connectionProxy.port());
//				currentProxy.setUser(connectionProxy.user());
//				currentProxy.setPassword(connectionProxy.password());
			}
//			else {
//				currentProxy.setType(QNetworkProxy::HttpCachingProxy);
//			}
		}
//	currentProxy.setType(QNetworkProxy::NoProxy);*/
	return success;
}

/*void connection::proxyAuthenticationRequired ( const QNetworkProxy &  proxy ,
        QAuthenticator * authenticator  )
{
//	qDebug()<<socket->proxy().user();
//	qDebug()<<socket->proxy().password();
//	authenticator->setUser(socket->proxy().user());
//	authenticator->setPassword(socket->proxy().password());
//	
//	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/ICQ."+icqUin, "account");
//	authenticator->setUser(settings.value("proxy/user").toString());
//	authenticator->setPassword(settings.value("proxy/pass").toString());
}*/

void connection::dnsResults(QHostInfo info)
{
	if (info.addresses().count() > 0)
	{
//		currentProxy.setHostName(info.addresses().at(0).toString());
	
//		socket->setProxy(currentProxy);
                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
		
		QString host = settings.value("connection/host", "login.icq.com").toString();
		quint16 port = settings.value("connection/port", 5190).toInt();
		connectedToBos = false;
		socket->connectToHost(host,port);
	}
}

/*void connection::setCurrentProxy(QNetworkProxy &proxyToCopy)
{
	currentProxy.setType(proxyToCopy.type());
	currentProxy.setHostName(proxyToCopy.hostName());
	currentProxy.setPort(proxyToCopy.port());
	currentProxy.setUser(proxyToCopy.user());
	currentProxy.setPassword(proxyToCopy.password());
	
}
*/
