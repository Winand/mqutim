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


#ifndef ICQMESSAGE_H_
#define ICQMESSAGE_H_

#include <QString>
#include <QDateTime>

class QTcpSocket;

class icqBuffer;
class tlv;

class messageFormat
{
public:
	QString from;
	QString fromUin;
	QString message;
	QDateTime date;
	quint64 position;
};

class icqMessage
{
public:
	icqMessage(const QString &);
	~icqMessage();
	void readData(icqBuffer *,quint16);
	QString from;
	QByteArray byteArrayMsg;
	QString msg;
	void sendMessage(QTcpSocket *, const messageFormat &, quint16, quint32, bool);
	void sendMessageChannel2(QTcpSocket *, const messageFormat &, quint16, quint32, bool);
	QTextCodec *codec;
	quint16 messageType;
	QByteArray msgCookie;
	QByteArray downCounter1;
	QByteArray downCounter2;
	void sendAutoreply(QTcpSocket *, const QString &message, quint16, quint32);
	void requestAutoreply(QTcpSocket *, const QString &, quint16, quint32);
	void requestXStatus(QTcpSocket *, const QString &, const QString &mine_uin, quint16, quint32);
	void sendXstatusReply(QTcpSocket *, const QString &, const QString &profile_name, quint16, quint32);
	void getAwayMessage(icqBuffer *, quint16);
	void sendMessageRecieved(QTcpSocket *socket,const QString &uin, const QByteArray &cookie,quint16, quint32);
	quint8 awayType;
	
	quint8 msgType;
	quint16 reason;
/*	bool fileAnswer;*/
	quint16 connectToPeer;
	
	quint32 peerIP;
	quint16 peerPort;
	QString fileName;
	quint32 fileSize;
	quint32 aolProxyIP;
	void sendImage(QTcpSocket *, const QString &, const QByteArray &image_raw, quint16, quint32);
        bool isValidUtf8(const QByteArray &array);
	
private:
	quint16 channel;
	QByteArray msgIdCookie;
	QByteArray serverRelaying();
	QString unicodeToUtf8( const QByteArray &);
	QByteArray utf8toUnicode( const QString &);
		QByteArray utf8toUnicodeLE( const QString &);
	quint16 readPlainText(icqBuffer *);
	quint16 readRendezvousData( tlv );
	QByteArray convertToByteArray(const quint8 &);
	QByteArray convertToByteArray(const quint16 &);
	QByteArray convertLEToByteArray(const quint16 &);
	QByteArray convertToByteArray(const quint32 &);
	quint8 byteArrayToInt8(const QByteArray &);
	quint16 byteArrayToInt16(const QByteArray &);
	quint32 byteArrayToInt32(const QByteArray &);
	quint16 byteArrayToLEInt16(const QByteArray &);
};

#endif /*ICQMESSAGE_H_*/
