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


#ifndef SERVERLOGINREPLY_H_
#define SERVERLOGINREPLY_H_

#include <QString>

class QTcpSocket;
class icqBuffer;

class serverLoginReply
{
public:
	serverLoginReply();
	~serverLoginReply();
	void readData(QTcpSocket *,icqBuffer *, const QString &);
	quint16 errorMessage;
	QString bosIp;
	quint16 bosPort;
	QByteArray cookie;
	bool isSnacError;
private:
	void getBosServer(const QString &);
	void getCookie(icqBuffer *);
	void getError(icqBuffer *);


};

#endif /*SERVERLOGINREPLY_H_*/
