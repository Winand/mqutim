/*
    buddyPicture

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


#ifndef BUDDYPICTURE_H_
#define BUDDYPICTURE_H_

#include <QObject>
#include <QString>

class QTcpSocket;
class icqBuffer;
/*class QNetworkProxy;*/


class buddyPicture : public QObject
{
	Q_OBJECT
public:
	buddyPicture(const QString &profile_name, const QString &mine_uin,
			QObject *parent = 0);
	~buddyPicture();
	void sendHash(const QString &, const QByteArray &);
	bool connectedToServ;
	bool canSendReqForAvatars;
        void connectToServ(const QString &, const quint16 &, QByteArray/*, const QNetworkProxy &*/);
	void disconnectFromSST();
	void uploadIcon(const QString &);
private slots:
	void readDataFromSocket();
	void socketDisconnected();
	void socketConnected();
	
signals:
	void emptyAvatarList();
	void updateAvatar(const QString &, QByteArray);
private:
	quint16 refNum;
	bool alreadySentCap;
	void incFlapSeq();
	void incSnacSeq();
	void readSnac(quint16);
	QByteArray convertToByteArray(const quint8 &);
	QByteArray convertToByteArray(const quint16 &);
	QByteArray convertToByteArray(const quint32 &);
	quint16 convertToInt16(const QByteArray &);
	quint8 convertToInt8(const QByteArray &);
	quint32 convertToInt32(const QByteArray &);
	QTcpSocket *tcpSocket;
	icqBuffer *buffer;
	bool readyToReadFlap;
	quint8 channel;
	quint16 flapLength;
	quint16 flapSeqNum;
	quint32 snacSeqNum;
	void sendCapab();
	void sendInfoReq();
	void sendRateInfoClientReady();
	void saveAvatar(quint16);
	QString m_profile_name;
	QByteArray SSTcookie;
	QString m_mine_uin;
};

#endif /*BUDDYPICTURE_H_*/
