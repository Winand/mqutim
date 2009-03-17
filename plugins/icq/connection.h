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


#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
/*#include <QNetworkProxy>*/
#include <QHostInfo>
/*#include <QAuthenticator>*/
#include "buffer.h"
#include "clientIdentification.h"
#include "tlv.h"

class tcpSocket;
class icqBuffer;

class connection : public QObject
{
    Q_OBJECT

public:
    connection(QTcpSocket *, icqBuffer *,const QString &, const QString &profile_name, QObject *parent = 0);
    ~connection();
    void connectToServer(QTcpSocket *);
    void readData(const quint16 &);
    void sendIdent(const QString &);
    void connectToBos(const QHostAddress &, const quint16 &, const QByteArray, const quint16 &);

signals:
	void connected();
	void sendLogin();
	void connectingToBos();
private slots:
/*        void proxyAuthenticationRequired ( const QNetworkProxy & proxy, QAuthenticator * authenticator );*/
    void dnsResults(QHostInfo);
private:
//	QNetworkProxy currentProxy;
    bool getProxy();
	QString icqUin;
	QTcpSocket *socket;
	icqBuffer *buffer;
	QByteArray protocolVersion;
	bool connectedToBos;
//	void setCurrentProxy(QNetworkProxy &);
	QString m_profile_name;
};

#endif // CONNECTION_H
