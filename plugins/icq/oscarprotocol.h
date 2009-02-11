/*
    oscarProtocol

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


#ifndef OSCARPROTOCOL_H
#define OSCARPROTOCOL_H

#include <QObject>
//#include <QAbstractSocket> // QAbstractSocket::SocketError
#include <QHostAddress>

#include "quticqglobals.h"

//#include "contactlist.h"

// Qt standard classes
class QTreeWidget;
class QTcpSocket;
class QTimer;
class QAbstractSocket;
enum QAbstractSocket::SocketError;

// qutIM own classes
class connection;
class closeConnection;
//class passwordDialog;
class snacChannel;
class contactListTree;
class icqBuffer;
//class EncryptionManager;


//enum accountStatus { online, ffc, away, na, occupied, dnd, invisible, offline, connecting};

class oscarProtocol : public QObject
{
    Q_OBJECT

public:
    oscarProtocol(const QString &, const QString &profile_name
    		, QObject *parent = 0);
    ~oscarProtocol();
    
    static quint16 secnumGenerator();
    
    void setStatus( accountStatus );
    accountStatus getStatus() { return currentStatus; }
    void removeContactList();
    void readreadMessageStack();
    contactListTree *getContactListClass() const;
    void sendKeepAlive(bool);
    bool userDisconnected;
    bool rateLimit;
    bool reconnectOnDisc;
    void resendCapabilities();
    void sendOnlyCapabilities();
    void setAutoAway();
    void setStatusAfterAutoAway();
    void reservedForFutureAOLHacks();

public slots:
	void onReloadGeneralSettings();

signals:
	void statusChanged(accountStatus currentStatus);
	void systemMessage(const QString &);
	void userMessage(const QString &, const QString &, const QString &, userMessageType, bool);
	void getNewMessage();
	void readAllMessages();
	void addToEventList(bool);
	void updateTranslation();
	void accountConnected(bool);

private slots:
	void disconnected();
	void connected();
	void displayError(QAbstractSocket::SocketError);
	void readDataFromSocket();
	void clearSocket();
	void sendIdentif();
	void sendSystemMessage(const QString &m ) { emit systemMessage(m); }
	void sendUserMessage(const QString &fu, const QString &f, const QString &m, userMessageType t, bool l) { emit userMessage(fu, f , m, t, l); }
	void getBosIp(const QHostAddress &addr) { bosIp = addr; }
	void getBosPort ( const quint16 &port) { bosPort = port; }
	void incFlapSeqNum();
	void incReqSeq();
	void getAuthKey(const QByteArray &);
	void reconnectToBos(const QByteArray);
	void connectingToBos();
	void rereadSocket();
//      void gettingNewMessage() { emit getNewMessage(); }
	void sendAlivePacket();
	void updateChangedStatus();
	void restartAutoAway(bool, quint32);
	void blockRateLimit() { rateLimit = true; }
	void proxyDeleteTimer();

	// Autoaway handling
	void onSecondIdle(int seconds);

private:
	bool checkPassword();

	accountStatus currentStatus;
	accountStatus tempStatus;
	QTcpSocket *connectionSocket;
	connection *connectClass;
	closeConnection *closeConnectionClass;
	snacChannel *snacChannelClass;
	QString icqUin;
	QByteArray password;
	bool connectingAccount;
	QHostAddress bosIp;
	quint16 bosPort;
	quint16 flapSeqNum;
	bool md5Connection;
	contactListTree *contactListClass;
	bool readyToReadFlap;
	quint8 channel;
	quint16 flapLength;
	icqBuffer *buffer;
	quint16 reqSeq;
	bool keepAlive;
	QTimer *timer;
	bool connectBos;
	QByteArray cookieForBos;

	bool fAutoAwayRunning;
	bool autoAway;
	quint32 awayMin;
    QString m_profile_name;
};

#endif // OSCARPROTOCOL_H
