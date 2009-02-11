/*
    snacChannel

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


#ifndef SNACCHANNEL_H_
#define SNACCHANNEL_H_

#include <QObject>

#include "quticqglobals.h"

class QTcpSocket;
class QHostAddress;
class icqBuffer;

class snacChannel : public QObject
{
	Q_OBJECT
public:
	snacChannel(QTcpSocket *, icqBuffer *,quint16, const QString &, const QString &profile_name, QObject *parent = 0);
	~snacChannel();
	void sendIdent(const quint16 &);
	void readData(const quint16);
	void md5Login(const QString &, const QString &, const quint16 &);
	void incFlap();
	
	void setStatus(accountStatus s) { status = s; } 
	void changeStatus(accountStatus);
	quint32 snacReqId;
	quint16 reqSeq;
	quint16 flapSequence;
	void resendCapabilities();
        void sendOnlyCapabilities();
        void setConnectBOS(bool bos) { m_connect_bos=bos; resetReqSnaqId(); }
	
signals:
	void incFlapSeq();
	void incReqSeq();
	void rereadSocket();
	void sendAuthKey(const QByteArray &);
	void systemMessage(const QString &);
	void userMessage(const QString &);
	void sendBosServer(const QHostAddress &);
	void sendBosPort(const quint16 &);
	void sendCookie(const QByteArray);
	void connected();
	void getList(bool);
	void oncomingBuddy(const QString &, quint16);
	void offlineBuddy(const QString &, quint16);
	void getMessage(quint16);
	void getOfflineMessage();
	void readMetaData(quint16, bool);
	void getTypingNotif(quint16);
	void readSSTserver(quint16);
	void blockRateLimit();
	void getStatusCheck(quint16);
	void getModifyItemFromServer(quint16);
	void youWereAdded(quint16);
	void getUploadIconData(quint16);
	void getAwayMessage(quint16);
	void getAuthorizationRequest(quint16);
	void authorizationAcceptedAnswer(quint16);
	void getMessageAck(quint16);
	void clearPrivacyLists();
private slots:
	quint32 returnSnacReqId();
        void resetReqSnaqId() { snacReqId = 0x00000000; }
	void incReq() { reqSeq++;}
	
private:
	void getMetaData(bool);
	void getServerLoginReply(quint16 &);
	void readAuthKey(quint16 &);
	void errorMessage(const quint16);
	void getServicesList(quint16 &);
	void clientRatesRequest();
	void serverRateLimitInfo(quint16 &);
	void getContactList(quint16 &,bool);
	void getOncomingBuddy(quint16 &);
	void getOfflineBuddy(quint16 &);
	
	QByteArray convertToByteArray(const quint8 &);
	QByteArray convertToByteArray(const quint16 &);
	QByteArray convertToByteArray(const quint32 &);
	quint16 convertToInt16(const QByteArray &);
	quint8 convertToInt8(const QByteArray &);
	quint32 convertToInt32(const QByteArray &);
	
	QString icqUin;
	QTcpSocket *tcpSocket;
	icqBuffer *socket;

	accountStatus status;
	QString m_profile_name;
        bool m_connect_bos;
	
};

#endif /*SNACCHANNEL_H_*/
