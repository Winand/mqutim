/*
    FileTransfer

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


/*#include "filetransfer.h"
#include "icqpluginsystem.h"

FileTransfer::FileTransfer(const QString & uin, QObject *parent)
	: QObject(parent)
	, ownerUin(uin)
{
	sendFileAction = new QAction(IcqPluginSystem::instance().getIcon("save_all"), tr("Send file"), this);
//	connectionProxy = QNetworkProxy::NoProxy;
	m_listen_port = 5191;
//	signalObject = new fileTransferSignals(this);
//	fileTransferSignals obj;
}

FileTransfer::~FileTransfer()
{
	foreach(fileRequestWindow *w, requestList)
		delete w;
	requestList.clear();

	foreach(fileTransferWindow *w, fileSendWindowList)
		delete w;
	fileSendWindowList.clear();
}

QAction *FileTransfer::getSendFileAction()
{
	return sendFileAction;
}

void FileTransfer::sendFileTriggered(const QString &uin, const QStringList &fileName)
{
	sendToUin = uin;

	if ( fileName.count() )
	{

		quint32 msgcookie = QTime::currentTime().hour() * QTime::currentTime().minute()
		* QTime::currentTime().second() * QTime::currentTime().msec();
		quint32 msgcookie2 = qrand();

		QByteArray cookie;
		cookie.append(convertToByteArray((quint32)msgcookie));
		cookie.append(convertToByteArray((quint32)msgcookie2));

		fileTransferWindow *w = new fileTransferWindow(ownerUin, fileName, uin, cookie, true, m_listen_port);
//		w->setMainConnectionProxy(connectionProxy);
		connect( w, SIGNAL(destroyed ( QObject *)),
				this, SLOT(deleteFileWin(QObject *)));

		connect( w, SIGNAL(cancelSending(QByteArray &,const QString &)),
				this, SLOT(cancelSending(QByteArray &, const QString &)));

		connect( w, SIGNAL(sendingToPeerRequest(const QByteArray &, const QString &, const QStringList &)),
				this, SLOT(sendingToPeerRequest(const QByteArray &, const QString &, const QStringList &)));

		connect( w, SIGNAL(getRedirectToProxyData(const QByteArray &, const QString &, quint16, quint32)),
				this, SLOT(getRedirectToProxyData(const QByteArray &, const QString &, quint16, quint32)));

		connect( w, SIGNAL(sendAcceptMessage(const QByteArray &, const QString &)),
				this, SLOT(sendAcceptMessage(const QByteArray &, const QString &)));

		connect( w, SIGNAL(sendRedirectToMineServer(const QByteArray&, const QString &, quint16)),
				this, SLOT(sendRedirectToMineServer(const QByteArray&, const QString &, quint16)));

		fileSendWindowList.insert(cookie, w);
		w->show();

		sendingToPeerRequest(cookie, uin, fileName);



	}
}

QByteArray FileTransfer::convertToByteArray(const quint16 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x100);
	packet[1] = (d % 0x100);
	return packet;
}

QByteArray FileTransfer::convertToByteArray(const quint8 &d)
{
	QByteArray packet;
	packet[0] = d;
	return packet;
}

QByteArray FileTransfer::convertToLEByteArray(const quint16 &d)
{
	QByteArray packet;
	packet[1] = (d / 0x100);
	packet[0] = (d % 0x100);
	return packet;
}

QByteArray FileTransfer::convertToByteArray(const quint32 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x1000000);
	packet[1] = (d / 0x10000);
	packet[2] = (d / 0x100);
	packet[3] = (d % 0x100);
	return packet;
}

quint16 FileTransfer::byteArrayToLEInt16(const QByteArray &array)
{
	bool ok;
	quint16 tmp = array.toHex().toUInt(&ok,16);
	return ((tmp % 0x100) * 0x100 + (tmp)/ 0x100);
}

quint32 FileTransfer::byteArrayToLEInt32(const QByteArray &array)
{
	bool ok;
	quint32 tmp = array.toHex().toUInt(&ok,16);
	return ((tmp % 0x100) * 0x1000000 +
			(tmp % 0x10000 / 0x100) * 0x10000 +
			(tmp % 0x1000000 / 0x10000) * 0x100 +
			(tmp)/0x1000000);
}

quint8 FileTransfer::convertToInt8(const QByteArray &packet)
{
	bool ok;
	return packet.toHex().toUInt(&ok,16);
}

void FileTransfer::deleteFileWin(QObject *obj)
{
	fileTransferWindow *tempWindow = (fileTransferWindow  *)(obj);
	fileSendWindowList.remove(fileSendWindowList.key(tempWindow));
}

void FileTransfer::cancelSending(QByteArray &array, const QString &uin)
{
	QByteArray packet;
	packet.append(array);
	packet.append(convertToByteArray((quint16)0x0002));
	packet[packet.length()] = uin.toUtf8().length();
	packet.append(uin.toUtf8());
	packet.append(convertToByteArray((quint16)0x0005));

	packet.append(convertToByteArray((quint16)26));
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(array);

	packet.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));

	emit emitCancelSending(packet);

}

void FileTransfer::contactCanceled(const QString &uin, const QByteArray &cookie)
{
	if ( fileSendWindowList.contains(cookie))
	{
		fileSendWindowList.value(cookie)->sendingDeclined(uin);
	}
}

void FileTransfer::contactAccept(const QString &uin, const QByteArray &cookie)
{
	if ( fileSendWindowList.contains(cookie))
	{
		fileSendWindowList.value(cookie)->sendingAccepted(uin);
	}
}

void FileTransfer::sendingToPeerRequest(const QByteArray &cookie, const QString &uin, const QStringList &fileName)
{
	QByteArray packet1;
	packet1.append(cookie);
	packet1.append(convertToByteArray((quint16)0x0002));
	packet1[packet1.length()] =  uin.toUtf8().length();
	packet1.append(uin.toUtf8());


	QByteArray rendezvousValue;
	rendezvousValue.append(convertToByteArray((quint16)0));
	rendezvousValue.append(cookie);
	rendezvousValue.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));

	tlv tlv0a;
	tlv0a.setType(0x000a);
	tlv0a.setData((quint16)1);

	rendezvousValue.append(tlv0a.getData());

	rendezvousValue.append(convertToByteArray((quint16)0x000f));
	rendezvousValue.append(convertToByteArray((quint16)0));

	QByteArray rendezvousValue2;


	tlv tlv05;
	tlv05.setType(0x0005);
	tlv05.setData((quint16)5191);

	rendezvousValue2.append(tlv05.getData());

	tlv tlv17;
	tlv17.setType(0x0017);
	tlv17.setData((quint16)(9280 ^ 0xffff));

	rendezvousValue2.append(tlv17.getData());

	if ( fileName.count() == 1 )
	{
		QString realFileName = fileName.at(0).section('/', -1);
	
		
		rendezvousValue2.append(convertToByteArray((quint16)0x2711));
		rendezvousValue2.append(convertToByteArray((quint16)(9 + realFileName.toUtf8().length())));
		rendezvousValue2.append(convertToByteArray((quint16)0x0001));
		rendezvousValue2.append(convertToByteArray((quint16)0x0001));
	
		QFile file(fileName.at(0));
	
		rendezvousValue2.append(convertToByteArray((quint32)file.size()));
	
		rendezvousValue2.append(realFileName.toUtf8());
	
		rendezvousValue2.append(QChar(0x00));
	}
	else
	{
		rendezvousValue2.append(convertToByteArray((quint16)0x2711));
		rendezvousValue2.append(convertToByteArray((quint16)9)); 
		rendezvousValue2.append(convertToByteArray((quint16)2)); 
		rendezvousValue2.append(convertToByteArray((quint16)fileName.count()));
		quint32 sumFileSize = 0;
		foreach(QString file_name, fileName)
		{
			QFile f(file_name);
			sumFileSize += f.size();
		}
		rendezvousValue2.append(convertToByteArray((quint32)sumFileSize));
		rendezvousValue2.append(QChar(0x00));
	}
	tlv tlv2712;
	tlv2712.setType(0x2712);
	tlv2712.setData(QString("utf-8"));

	rendezvousValue2.append(tlv2712.getData());



	emit sendFile(packet1, rendezvousValue, rendezvousValue2);
}

void FileTransfer::requestToRedirect(const QString &uin, const QByteArray &cookie,
		quint16 redirectTo, quint32 ip, quint16 port, const QString & contactName,
		const QString &fileName, quint32 fileSize, quint32 aolProxyIP)
{
	if ( fileSendWindowList.contains(cookie) && redirectTo == 2)
	{
		if ( (aolProxyIP && !ip))
			fileSendWindowList.value(cookie)->connectToProxy(aolProxyIP, port, true);
		else

			fileSendWindowList.value(cookie)->connectToProxy(ip, port, false);
	}

	if ( fileSendWindowList.contains(cookie) && redirectTo == 3)
	{
		fileSendWindowList.value(cookie)->connectToAolProxy(aolProxyIP, port);
	}
	if ( redirectTo == 1)
	{
		fileRequestWindow *w = new fileRequestWindow;
		connect( w, SIGNAL(destroyed ( QObject *)),
				this, SLOT(deleteReqWin(QObject *)));
		connect( w, SIGNAL(cancelSending(QByteArray &,const QString &)),
				this, SLOT(cancelSending(QByteArray &, const QString &)));
		connect( w, SIGNAL(fileAccepted(const QByteArray &, const QString &, const QString &,quint32, quint16, quint32)),
				this, SLOT(fileAccepted(const QByteArray &, const QString &, const QString &,quint32, quint16, quint32)));
		w->setSengingData(contactName, fileName, ip, fileSize, port);
		requestList.insert(cookie, w);
		w->uin = uin;
		w->cookie = cookie;
		w->show();
	}
}

void FileTransfer::getRedirectToProxyData(const QByteArray &cookie, const QString &uin, quint16 port, quint32 ip)
{
	QByteArray packet;
	packet.append(cookie);
	packet.append(convertToByteArray((quint16)0x0002));
	packet[packet.length()] =  uin.toUtf8().length();
	packet.append(uin.toUtf8());


	QByteArray rendezvousValue;
	rendezvousValue.append(convertToByteArray((quint16)0));
	rendezvousValue.append(cookie);
	rendezvousValue.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));

	tlv tlv0a;
	tlv0a.setType(0x000a);
	tlv0a.setData((quint16)3);

	rendezvousValue.append(tlv0a.getData());


	tlv tlv2;
	tlv2.setType(0x0002);
	tlv2.setData((quint32)ip);

	rendezvousValue.append(tlv2.getData());

//	tlv tlv3;
//	tlv3.setType(0x0003);
//	tlv3.setData((quint32)ip);
//
//	rendezvousValue.append(tlv3.getData());


	tlv tlv16;
	tlv16.setType(0x0016);
	tlv16.setData((quint32)(ip ^ 0xffffffff));

	rendezvousValue.append(tlv16.getData());

	tlv tlv5;
	tlv5.setType(0x0005);
	tlv5.setData((quint16)port);

	rendezvousValue.append(tlv5.getData());


	tlv tlv17;
	tlv17.setType(0x0017);
	tlv17.setData((quint16)(port ^ 0xffff));

	rendezvousValue.append(tlv17.getData());

	rendezvousValue.append(convertToByteArray((quint16)0x0010));
	rendezvousValue.append(convertToByteArray((quint16)0));


	packet.append(convertToByteArray((quint16)0x0005));
	packet.append(convertToByteArray((quint16)rendezvousValue.length()));
	packet.append(rendezvousValue);

	emit sendRedirectToProxy(packet);
}

void FileTransfer::sendAcceptMessage(const QByteArray &cookie, const QString &uin)
{
	QByteArray packet;
	packet.append(cookie);
	packet.append(convertToByteArray((quint16)0x0002));
	packet[packet.length()] = uin.toUtf8().length();
	packet.append(uin.toUtf8());
	packet.append(convertToByteArray((quint16)0x0005));

	packet.append(convertToByteArray((quint16)26));
	packet.append(convertToByteArray((quint16)0x0002));
	packet.append(cookie);

	packet.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));

	emit emitAcceptSending(packet);
}

void FileTransfer::deleteReqWin(QObject *obj)
{
	fileRequestWindow *tempWindow = (fileRequestWindow *)(obj);
	requestList.remove(requestList.key(tempWindow));
}

void FileTransfer::fileAccepted(const QByteArray &cookie, const QString &uin,
		const QString &fileName, quint32 ip, quint16 port,quint32 fileSize)
{
	QStringList list;
	list<<fileName;
	QByteArray cook = cookie;
	fileTransferWindow *w = new fileTransferWindow((const QString&)ownerUin,(const QStringList&) list, uin, cook, false, m_listen_port);
//	w->setMainConnectionProxy(connectionProxy);
	w->fileSize = fileSize;
	connect( w, SIGNAL(destroyed ( QObject *)),
			this, SLOT(deleteFileWin(QObject *)));

	connect( w, SIGNAL(cancelSending(QByteArray &,const QString &)),
			this, SLOT(cancelSending(QByteArray &, const QString &)));

	connect( w, SIGNAL(sendingToPeerRequest(const QByteArray &, const QString &, const QStringList &)),
			this, SLOT(sendingToPeerRequest(const QByteArray &, const QString &, const QStringList &)));

	connect( w, SIGNAL(getRedirectToProxyData(const QByteArray &, const QString &, quint16, quint32)),
			this, SLOT(getRedirectToProxyData(const QByteArray &, const QString &, quint16, quint32)));

	connect( w, SIGNAL(sendAcceptMessage(const QByteArray &, const QString &)),
			this, SLOT(sendAcceptMessage(const QByteArray &, const QString &)));
	connect( w, SIGNAL(sendRedirectToMineServer(const QByteArray&, const QString &, quint16)),
			this, SLOT(sendRedirectToMineServer(const QByteArray&, const QString &, quint16)));

	fileSendWindowList.insert(cookie, w);
	w->setVisualContactIp(ip);
	w->show();
	w->connectToProxy(ip, port, false);
}

void FileTransfer::sendRedirectToMineServer(const QByteArray& cookie, const QString &uin, quint16 port)
{
	QByteArray packet1;
	packet1.append(cookie);
	packet1.append(convertToByteArray((quint16)0x0002));
	packet1[packet1.length()] =  uin.toUtf8().length();
	packet1.append(uin.toUtf8());


	QByteArray rendezvousValue;
	rendezvousValue.append(convertToByteArray((quint16)0));
	rendezvousValue.append(cookie);
	rendezvousValue.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));

	tlv tlv0a;
	tlv0a.setType(0x000a);
	tlv0a.setData((quint16)2);

	rendezvousValue.append(tlv0a.getData());


	QByteArray rendezvousValue2;


	tlv tlv05;
	tlv05.setType(0x0005);
	tlv05.setData((quint16)port);

	rendezvousValue2.append(tlv05.getData());

	tlv tlv17;
	tlv17.setType(0x0017);
	tlv17.setData((quint16)(9280 ^ 0xffff));

	rendezvousValue2.append(tlv17.getData());



	emit sendFile(packet1, rendezvousValue, rendezvousValue2);
}
void FileTransfer::disconnectFromAll()
{
	foreach(fileRequestWindow *w, requestList)
		delete w;
	requestList.clear();

	foreach(fileTransferWindow *w, fileSendWindowList)
		delete w;
	fileSendWindowList.clear();
}*/
