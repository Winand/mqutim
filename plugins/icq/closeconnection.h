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


#ifndef CLOSECONNECTION_H_
#define CLOSECONNECTION_H_

#include <QObject>

class QTcpSocket;
class QString;
class QHostAddress;

class icqBuffer;

class closeConnection : public QObject
{
	Q_OBJECT
public:
	closeConnection(QObject *parent = 0);
	~closeConnection();
	void readData(QTcpSocket *, icqBuffer *, const QString &);
	
signals:
	void systemMessage(const QString &);
	void sendCookie(const QByteArray);
	void sendBosServer(const QHostAddress &);
	void sendBosPort(const quint16 &);
	void blockRateLimit();
	
private:
	void disconnectTakeUin(QTcpSocket *);
	void errorMessage(const quint16);
	void getError(QTcpSocket *, icqBuffer *);
	void getLuck(icqBuffer *);
	void getBosServer(const QString &);
};

#endif /*CLOSECONNECTION_H_*/
