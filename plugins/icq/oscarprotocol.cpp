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

//#include <QtGui/QWidget>
#include <QTcpSocket>
#include <QtopiaApplication>

#include "flap.h"
#include "connection.h"
#include "passworddialog.h"
#include "closeconnection.h"
#include "snacchannel.h"
#include "buffer.h"
#include "contactlist.h"

#include "oscarprotocol.h"

oscarProtocol::oscarProtocol(const QString &account, const QString &profile_name,
		QObject *parent)
	: QObject(parent)
	, icqUin(account)
	, m_profile_name(profile_name)
{
	readyToReadFlap = true;
	flapLength = 0;
	channel = 0;
	currentStatus = offline;
	tempStatus = offline;
	md5Connection = true;
	connectingAccount = false;
	reconnectOnDisc = true;
	userDisconnected = false;
	rateLimit = false;

	connectionSocket = new QTcpSocket(this);
	buffer = new icqBuffer(this);
        buffer->open(QIODevice::ReadWrite);
       // flapSeqNum = secnumGenerator(); //sequences[rand() % sequences_num];
	flapSeqNum = 0x0000;
        reqSeq = 0x0000;
	keepAlive = true;
	connectBos = false;
	autoAway = false;
	fAutoAwayRunning = false;

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(sendAlivePacket()));

	connectClass = new connection(connectionSocket, buffer, icqUin, m_profile_name, this);

	connect ( connectionSocket, SIGNAL(disconnected()),
			this, SLOT(disconnected()));
	connect ( connectionSocket, SIGNAL(readyRead()),
			this, SLOT(readDataFromSocket()));


	connect( connectionSocket, SIGNAL(error(QAbstractSocket::SocketError)),
	        this, SLOT(displayError(QAbstractSocket::SocketError)));


	connect ( connectClass, SIGNAL(sendLogin()),
				this, SLOT(sendIdentif()) );
	connect ( connectClass, SIGNAL(connectingToBos()),
					this, SLOT(connectingToBos()) );

	closeConnectionClass = new closeConnection(this);
	connect ( closeConnectionClass , SIGNAL(systemMessage(const QString &)),
			this, SLOT(sendSystemMessage(const QString &)));
	connect ( closeConnectionClass , SIGNAL(sendBosServer(const QHostAddress &)),
				this, SLOT(getBosIp(const QHostAddress &)));
	connect ( closeConnectionClass , SIGNAL(sendBosPort(const quint16 &)),
					this, SLOT(getBosPort(const quint16 &)));
	connect ( closeConnectionClass , SIGNAL(sendCookie(const QByteArray)),
						this, SLOT(reconnectToBos(const QByteArray)));

	snacChannelClass = new snacChannel(connectionSocket, buffer, flapSeqNum, icqUin, m_profile_name, this);

	contactListClass = new contactListTree(connectionSocket, buffer, icqUin, m_profile_name,
			flapSeqNum, snacChannelClass->snacReqId,snacChannelClass->reqSeq, this);
	connect ( contactListClass , SIGNAL(userMessage(const QString &, const QString &, const QString &, userMessageType, bool)),
			this, SLOT(sendUserMessage(const QString &, const QString &, const QString &, userMessageType, bool)));

	connect(this, SIGNAL(updateTranslation()),
		contactListClass, SLOT(onUpdateTranslation()));

	connect ( contactListClass , SIGNAL(sendSystemMessage(const QString &)),
			this, SLOT(sendSystemMessage(const QString &)));

	connect ( contactListClass , SIGNAL(getNewMessage()),
                this, SIGNAL(getNewMessage()));

	connect ( contactListClass , SIGNAL(readAllMessages()),
			this, SIGNAL(readAllMessages()));
	connect ( contactListClass , SIGNAL(updateStatus()),
				this, SLOT(updateChangedStatus()));
	connect ( contactListClass , SIGNAL(restartAutoAway(bool, quint32)),
					this, SLOT(restartAutoAway(bool, quint32)));

        connect(this, SIGNAL(statusChanged(accountStatus)),
                contactListClass, SLOT(onStatusChanged(accountStatus)));
        connect(this, SIGNAL(systemMessage(const QString &)),
                contactListClass, SLOT(onSystemMessage()));
//      connect(this, SIGNAL(accountConnected(bool)),
//              contactListClass, SLOT(onConnected(bool)));

	connect ( snacChannelClass, SIGNAL(incFlapSeq()),
			this, SLOT(incFlapSeqNum()));
	connect ( snacChannelClass, SIGNAL(incReqSeq()),
				this, SLOT(incReqSeq()));
	connect ( snacChannelClass, SIGNAL(rereadSocket()),
				this, SLOT(rereadSocket()));
	connect ( snacChannelClass, SIGNAL(sendAuthKey(const QByteArray &)),
					this, SLOT(getAuthKey(const QByteArray &)));
	connect ( snacChannelClass , SIGNAL(systemMessage(const QString &)),
				this, SLOT(sendSystemMessage(const QString &)));

	connect ( snacChannelClass , SIGNAL(sendBosServer(const QHostAddress &)),
				this, SLOT(getBosIp(const QHostAddress &)));
	connect ( snacChannelClass , SIGNAL(sendBosPort(const quint16 &)),
				this, SLOT(getBosPort(const quint16 &)));
	connect ( snacChannelClass , SIGNAL(sendCookie(const QByteArray)),
					this, SLOT(reconnectToBos(const QByteArray)));
	connect ( snacChannelClass, SIGNAL(connected()),
				this, SLOT(connected()) );

	connect ( snacChannelClass, SIGNAL(blockRateLimit()),
				this, SLOT(blockRateLimit()));
	connect(closeConnectionClass, SIGNAL(blockRateLimit()),
				this, SLOT(blockRateLimit()));
	connect(snacChannelClass, SIGNAL(oncomingBuddy(const QString &, quint16)),
			contactListClass, SLOT(oncomingBuddy(const QString &, quint16)) );

	connect(snacChannelClass, SIGNAL(offlineBuddy(const QString &, quint16)),
			contactListClass, SLOT(offlineBuddy(const QString &, quint16)) );

	connect(snacChannelClass, SIGNAL(getList(bool)),
					contactListClass, SLOT(createContact(bool)));
	connect(snacChannelClass, SIGNAL(clearPrivacyLists()),
					contactListClass, SLOT(clearPrivacyLists()));
	connect ( snacChannelClass , SIGNAL(getMessage(quint16)),
				contactListClass, SLOT(getMessage(quint16)));
	connect ( snacChannelClass , SIGNAL(getOfflineMessage()),
					contactListClass, SLOT(getOfflineMessage()));
	connect ( contactListClass , SIGNAL(incSnacSeq()),
			snacChannelClass, SLOT(returnSnacReqId()));
	connect ( contactListClass , SIGNAL(incFlapSeq()),
				this, SLOT(incFlapSeqNum()));
	connect ( contactListClass , SIGNAL(incMetaSeq()),
				snacChannelClass, SLOT(incReq()));
	connect ( snacChannelClass , SIGNAL(readMetaData(quint16, bool)),
			contactListClass, SLOT(readMetaData(quint16,bool)));
	connect ( snacChannelClass , SIGNAL(getTypingNotif(quint16)),
				contactListClass, SLOT(getTypingNotification(quint16)));

	connect ( snacChannelClass , SIGNAL(readSSTserver(quint16)),
					contactListClass, SLOT(readSSTserver(quint16)));

	connect ( snacChannelClass , SIGNAL(getStatusCheck(quint16)),
						contactListClass, SLOT(getStatusCheck(quint16)));

	connect ( snacChannelClass , SIGNAL(getModifyItemFromServer(quint16)),
							contactListClass, SLOT(getModifyItemFromServer(quint16)));

	connect ( snacChannelClass , SIGNAL(getStatusCheck(quint16)),
						contactListClass, SLOT(getStatusCheck(quint16)));

	connect ( snacChannelClass , SIGNAL(getMessageAck(quint16)),
						contactListClass, SLOT(getMessageAck(quint16)));

	connect ( this, SIGNAL(addToEventList(bool)),
									parent, SLOT(addToEvent(bool)));

	connect (snacChannelClass, SIGNAL(youWereAdded(quint16)),
			contactListClass, SLOT(youWereAdded(quint16)));

	connect (snacChannelClass, SIGNAL(getUploadIconData(quint16)),
			contactListClass, SLOT(getUploadIconData(quint16)));

	connect (snacChannelClass, SIGNAL(getAwayMessage(quint16)),
			contactListClass, SLOT(getAwayMessage(quint16)));

	connect (snacChannelClass, SIGNAL(getAuthorizationRequest(quint16)),
			contactListClass, SLOT(getAuthorizationRequest(quint16)));


	connect (snacChannelClass, SIGNAL(authorizationAcceptedAnswer(quint16)),
			contactListClass, SLOT(authorizationAcceptedAnswer(quint16)));

        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	autoAway = settings.value("statuses/autoaway", true).toBool();
	awayMin = settings.value("statuses/awaymin", 10).toUInt();
	if ( autoAway )
		emit addToEventList(true);


}

oscarProtocol::~oscarProtocol()
{

}

quint16 oscarProtocol::secnumGenerator()
{
    quint32 n = qrand(), s = 0, i;
    for (i = n; i >>= 3; s += i);
    return ((((0 - s) ^ (quint8)n) & 7 ^ n) + 2) & 0x7fff;
}

void oscarProtocol::setStatus(accountStatus status)
{
	// Disable autoaway
	fAutoAwayRunning = false;

	if ( status != currentStatus)
	{

		bool changestatus = true;
		if ( status == offline && currentStatus != offline )
		{
			emit statusChanged(offline);
			clearSocket();
			connectingAccount = false;
                        connectionSocket->disconnectFromHost();
		}

		if ( (currentStatus == offline && status !=offline))
		{
			if ( status != connecting )
			{
			if ( checkPassword() )
			{
				connectingAccount = true;
				userDisconnected = false;
				emit statusChanged(connecting);
				connectClass->connectToServer(connectionSocket);
				currentStatus = status;
//                              contactListClass->currentStatus = status;
				changestatus = true;
			} else {
				changestatus = false;
			}
			}

		}
		if ( changestatus && !connectingAccount )
		{
			currentStatus = status;
//                      contactListClass->currentStatus = status;
			snacChannelClass->changeStatus(currentStatus);
			emit statusChanged(status);


		}
	}
}

void oscarProtocol::disconnected()
{
	clearSocket();
	tempStatus = currentStatus;
	if ( !connectBos)
	{
		currentStatus = offline;
                reservedForFutureAOLHacks();
	} else
		connectBos = false;
        snacChannelClass->setConnectBOS(connectBos);

	emit statusChanged(offline);


//      contactListClass->currentStatus = offline;

	snacChannelClass->setStatus(tempStatus);
	timer->stop();
	contactListClass->goingOnline(false);

	if ( !userDisconnected && reconnectOnDisc && !rateLimit )
	{
		setStatus(tempStatus);
	}

//	if ( connectBos )
//	{
//		connectClass->connectToBos(bosIp, bosPort, cookieForBos, flapSeqNum);
//		incFlapSeqNum();
//		connectBos = false;
//	}

}

void oscarProtocol::connected()
{
	connectingAccount = false;
	rateLimit = false;
	currentStatus = tempStatus;
//      contactListClass->currentStatus = tempStatus;
	emit accountConnected(true);
	emit statusChanged(tempStatus);

	if ( keepAlive )
		timer->start(60000);
	contactListClass->goingOnline(true);
}

void oscarProtocol::displayError(QAbstractSocket::SocketError error)
{
	setStatus(offline);

	switch(error)
	{
	case QAbstractSocket::ConnectionRefusedError:
		systemMessage(tr("The connection was refused by the peer (or timed out)."));
		break;
	case QAbstractSocket::RemoteHostClosedError:
		systemMessage(tr("The remote host closed the connection."));
		break;
	case QAbstractSocket::HostNotFoundError:
		systemMessage(tr("The host address was not found."));
		break;
	case QAbstractSocket::SocketAccessError:
		systemMessage(tr("The socket operation failed because "
				"the application lacked the required privileges."));
		break;
	case QAbstractSocket::SocketResourceError:
		systemMessage(tr("The local system ran out of resources (e.g., too many sockets)."));
		break;
	case QAbstractSocket::SocketTimeoutError:
		systemMessage(tr("The socket operation timed out."));
		break;
	case QAbstractSocket::NetworkError:
		systemMessage(tr("An error occurred with the network "
				"(e.g., the network cable was accidentally plugged out)."));
		break;
	case QAbstractSocket::UnsupportedSocketOperationError:
		systemMessage(tr("The requested socket operation is not supported "
				"by the local operating system (e.g., lack of IPv6 support)."));
		break;
	case QAbstractSocket::ProxyAuthenticationRequiredError:
		systemMessage(tr("The socket is using a proxy, and the proxy requires authentication."));
		break;
	default:
		systemMessage(tr("An unidentified network error occurred."));
	}
}

void oscarProtocol::readDataFromSocket()
{

//	if ( connectionSocket->bytesAvailable()  < 6 )
//		return;
//	quint64 tmpLength = connectionSocket->bytesAvailable();
	buffer->write(connectionSocket->readAll());
	if ( readyToReadFlap )
	{

		flapPacket flap;
		if ( !flap.readFromSocket( buffer ) )
		{
//			qDebug()<<connectionSocket->readAll().toHex();
			return;
		}


		channel = flap.getChannel();
		flapLength = flap.getLength();

	}

	if ( buffer->bytesAvailable() < flapLength )
	{
		readyToReadFlap = false;
		return;
	}
	readyToReadFlap = true;
		if ( channel == 0x01)
			connectClass->readData(flapLength);

		if ( channel == 0x02 )
		{
			snacChannelClass->readData(flapLength);

		}

		if ( channel == 0x03 )
			buffer->read(flapLength);

		if ( channel == 0x04)
			closeConnectionClass->readData(connectionSocket, buffer, icqUin);

		if ( channel >= 0x05 )
			buffer->read(flapLength);

	if (buffer->bytesAvailable())
		readDataFromSocket();
//	if ( connectionSocket->bytesAvailable() )
//		clearSocket();

}

void oscarProtocol::clearSocket()
{
	connectionSocket->readAll();
	buffer->readAll();
}

bool oscarProtocol::checkPassword()
{	
  QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin, "accountsettings");
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
  static const char crypter[] = {0x10,0x67, 0x56, 0x78, 0x85, 0x14, 0x87, 0x11, 0x45,0x45,0x45,0x45,0x45,0x45 };
  
	md5Connection = settings.value("connection/md5", true).toBool();
	if ( account_settings.value("main/savepass", false).toBool() )
	{
		QByteArray tmpPass = account_settings.value("main/password").toByteArray();
		QByteArray roastedPass;
		for ( int i = 0; i < tmpPass.length(); i++ )
			roastedPass[i] = tmpPass.at(i) ^ crypter[i];
		password = contactListClass->convertPassToCodePage(roastedPass);
//		password = settings.value("main/password").toString();
		return true;
	} else {

		passwordDialog dialog;
		dialog.setTitle(icqUin);
		if ( QtopiaApplication::execDialog(&dialog) )
		{
			password = contactListClass->convertPassToCodePage(dialog.getPass());
			if ( dialog.getSavePass() )
			{
				QByteArray roastedPass;
				for ( int i = 0; i < password.length(); i++ )
					roastedPass[i] = password.at(i) ^ crypter[i];
//				settings.setValue("main/password", password);
				account_settings.setValue("main/password",roastedPass);
			}
				account_settings.setValue("main/savepass", dialog.getSavePass() );
			return true;
		}
	}
	return false;
}

void oscarProtocol::sendIdentif()
{
	if ( md5Connection )
		snacChannelClass->sendIdent(flapSeqNum);
	else
	{
		connectClass->sendIdent(password);
		password.clear();
	}
}

void oscarProtocol::incFlapSeqNum()
{
	if ( flapSeqNum != 0x8000 )
		flapSeqNum++;
	else
		flapSeqNum = 0x0000;
	snacChannelClass->incFlap();
}


void oscarProtocol::getAuthKey(const QByteArray &authKey)
{
	snacChannelClass->md5Login(password, authKey, flapSeqNum);
	password.clear();
}

void oscarProtocol::reconnectToBos(const QByteArray cookie)
{
	connectBos = true;
	cookieForBos = cookie;
	connectionSocket->disconnectFromHost();
	connectionSocket->close();
//	if ( connectionSocket->proxy().type() != QNetworkProxy::NoProxy)
//	{
//	delete connectionSocket;
//	connectionSocket = new QTcpSocket(this);
//
//	connect ( connectionSocket, SIGNAL(disconnected()),
//				this, SLOT(disconnected()));
//	connect ( connectionSocket, SIGNAL(readyRead()),
//				this, SLOT(readDataFromSocket()));
//
//	connect( connectionSocket, SIGNAL(error(QAbstractSocket::SocketError)),
//		        this, SLOT(displayError(QAbstractSocket::SocketError)));
//	}

	connectClass->connectToBos(bosIp, bosPort, cookie, flapSeqNum);
	incFlapSeqNum();
}

void oscarProtocol::connectingToBos()
{
	emit statusChanged(connecting);
	currentStatus = connecting;
//      contactListClass->currentStatus = connecting;

}

void oscarProtocol::removeContactList()
{
	contactListClass->removeContactList();
}

void oscarProtocol::rereadSocket()
{
	readyToReadFlap = true;
	readDataFromSocket();
}

void oscarProtocol::readreadMessageStack()
{
	contactListClass->readMessageStack();
}

void oscarProtocol::incReqSeq()
{
	reqSeq++;

}

void oscarProtocol::sendKeepAlive(bool flag)
{


	if ( keepAlive != flag )
	{
		if ( !connectingAccount && (currentStatus != offline))
		{
			if ( flag )
			{
				timer->start(60000);
			} else {
				timer->stop();
			}
		}
	}

	keepAlive = flag;
}

void oscarProtocol::sendAlivePacket()
{
	if ( !connectionSocket->bytesAvailable() && !connectionSocket->bytesToWrite())
	{
		QByteArray packet;
		packet[0] = 0x2A;
		packet[1] = 0x05;
		packet[2] = flapSeqNum / 0x100;
		packet[3] = flapSeqNum % 0x100;
		packet[4] = 0x00;
		packet[5] = 0x00;
		incFlapSeqNum();
		qint64 aliveSize = connectionSocket->write((const char*)packet, 6);
		if (!connectionSocket->waitForBytesWritten(1000))
			connectionSocket->abort();
		if ( aliveSize == 0 || aliveSize == -1 )
			connectionSocket->disconnectFromHost();
		buffer->readAll();

	}

}


void oscarProtocol::updateChangedStatus()
{
	if ( currentStatus != connecting && currentStatus != offline )
	{
		snacChannelClass->changeStatus(currentStatus);
	}
}

void oscarProtocol::restartAutoAway(bool f, quint32 min)
{
	emit addToEventList(f);
	autoAway = f;
	awayMin = min;
}

void oscarProtocol::resendCapabilities()
{
	if ( currentStatus != offline || currentStatus != connecting )
		snacChannelClass->resendCapabilities();
}

void oscarProtocol::sendOnlyCapabilities()
{
	if ( currentStatus != offline || currentStatus != connecting )
		snacChannelClass->sendOnlyCapabilities();
}

void oscarProtocol::onReloadGeneralSettings()
{
	contactListClass->onReloadGeneralSettings();
}

void oscarProtocol::proxyDeleteTimer()
{
	connectClass->connectToBos(bosIp, bosPort, cookieForBos,
		flapSeqNum);
}

void oscarProtocol::onSecondIdle(int seconds)
{
	// Autoaway is disabled, do not touch anything
	if (!autoAway)
		return ;

	// if activity is detected
	if (seconds == 0)
	{
		if (currentStatus == away && fAutoAwayRunning)
			setStatus(online);
	}

	// If idle time is bigger than autoaway time
	if (seconds > static_cast<int>(awayMin) * 60)
	{
		if (currentStatus == online)
		{
			setStatus(away);
			fAutoAwayRunning = true;
		}
	}
}

void oscarProtocol::setAutoAway()
{
	if (currentStatus == online)
	{
		setStatus(away);
		fAutoAwayRunning = true;
	}
}

void oscarProtocol::setStatusAfterAutoAway()
{
	if (currentStatus == away && fAutoAwayRunning)
		setStatus(online);
}

contactListTree* oscarProtocol::getContactListClass() const
{
	return contactListClass;
}

void oscarProtocol::reservedForFutureAOLHacks()
{
   // flapSeqNum = sequences[rand() % sequences_num];
   // flapSeqNum = secnumGenerator();
    flapSeqNum = 0x0000;
    snacChannelClass->flapSequence = flapSeqNum;

}
