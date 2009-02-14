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


#ifndef FILETRANSFER_H
#define FILETRANSFER_H

/*#include <QtGui>
//#include <QNetworkProxy>
#include "tlv.h"
#include "filetransferwindow.h"
#include "filerequestwindow.h"

class FileTransfer : public QObject
{
    Q_OBJECT
public:
	FileTransfer(const QString &, QObject *parent = 0);
	~FileTransfer();
	QAction *getSendFileAction();
	void removeAll();
        void sendFileTriggered(const QString &, const QStringList &);
	void contactCanceled(const QString &, const QByteArray &);
	void contactAccept(const QString &, const QByteArray &);
	void requestToRedirect(const QString &, const QByteArray &, quint16, quint32, quint16, const QString &, const QString &, quint32, quint32);
	void disconnectFromAll();
//	QNetworkProxy connectionProxy;
	void setListenPort(quint16 port)
	{
		m_listen_port = port;
	}
private slots:
	void getRedirectToProxyData(const QByteArray &, const QString &, quint16, quint32);
	void deleteFileWin(QObject *);
	void deleteReqWin(QObject *);
	void cancelSending(QByteArray &, const QString &);
	void sendingToPeerRequest(const QByteArray &, const QString &, const QStringList &);

	void sendAcceptMessage(const QByteArray &, const QString &);
	void fileAccepted(const QByteArray &, const QString &, const QString &,quint32, quint16, quint32);
	void sendRedirectToMineServer(const QByteArray&, const QString &, quint16);
signals:
	void sendFile(QByteArray &, QByteArray &, QByteArray &);
	void emitCancelSending(QByteArray &);
	void sendRedirectToProxy(const QByteArray &);
	void emitAcceptSending(const QByteArray &);
private:
	QTreeWidgetItem *settingsTreeItem;
	QAction *sendFileAction;

	QByteArray convertToByteArray(const quint8 &);
	QByteArray convertToByteArray(const quint16 &);
	QByteArray convertToLEByteArray(const quint16 &);
	QByteArray convertToByteArray(const quint32 &);

	quint16 byteArrayToLEInt16(const QByteArray &);
	quint32 byteArrayToLEInt32(const QByteArray &);
	quint8 convertToInt8(const QByteArray &);

	QHash<QByteArray, fileTransferWindow *> fileSendWindowList;

	QString sendToUin;
	QString ownerUin;

	QHash<QByteArray, fileRequestWindow *> requestList;
	quint16 m_listen_port;
};*/

#endif // FILETRANSFER_H
