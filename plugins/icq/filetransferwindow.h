/*
    fileTransferWindow

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


#ifndef FILETRANSFERWINDOW_H
#define FILETRANSFERWINDOW_H

/*#include <QtGui>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include "ui_filetransferwindow.h"

class fileThread : public QThread
{
public:
//	fileThread();
	bool flag;
	void run()
	{
//		flag = false;
//		for(;;)
//		{
//			if(flag)
//				return;
//		}
		exec();
	}

	void test()
	{
//		for(;;);
	}
};


class fileTransferWindow : public QWidget
{
    Q_OBJECT

public:
    fileTransferWindow(const QString &, const QStringList &,
    		const QString &, QByteArray &,bool, 
    		quint16 listen_port,
    		QWidget *parent = 0);
    ~fileTransferWindow();
    void sendingDeclined(const QString &);
    void sendingAccepted(const QString &);
    void connectToProxy(quint32, quint16, bool);
    void connectToAolProxy(quint32, quint16);
    quint32 fileSize;
    void setVisualContactIp(quint32);
//    void setMainConnectionProxy(const QNetworkProxy &);
private slots:
	void on_cancelButton_clicked();
	void on_openButton_clicked();
	void socketConnected();
	void readFromSocket();
	void sendFileData();
	void checkLocalConnection();
	void sendTransferPacket();
	void bytesWritten();
	void slotNewConnection();
	void updateProgress();
signals:
	void cancelSending(QByteArray &, const QString &);
	void sendingToPeerRequest(const QByteArray &, const QString &,const QStringList &);
	void getRedirectToProxyData(const QByteArray &, const QString &, quint16, quint32);
	void sendAcceptMessage(const QByteArray &, const QString &);
	void sendRedirectToMineServer(const QByteArray&, const QString &, quint16);
protected:
	void closeEvent ( QCloseEvent * event );

private:
    Ui::fileTransferWindowClass ui;
    QPoint desktopCenter();
   	fileThread mainThread;
   	bool sendingFile;
   	QByteArray cookie;
   	QString contactUin;
   	QStringList fileList;
   	QTcpSocket *tcpSocket;
        QByteArray convertToByteArray(const quint8 &) const;
        QByteArray convertToByteArray(const quint16 &) const;
        QByteArray convertToByteArray(const quint32 &) const;
	QString mineUin;
	bool waitingFor18;
        quint16 byteArrayToInt16(const QByteArray &) const;
        quint32 byteArrayToInt32(const QByteArray &) const;

	quint32 prevCheckSum;
        quint32 fileCheckSum(QFile &, quint32) const;
	bool waitingForFileAccept;
	QFile currentFile;
	quint16 currentFileChunkSize;
	bool waitingForGoodTransfer;
	bool connectedToProxy;
	bool sendingFileNow;
	quint32 tmpFileSize;
	quint32 currentFileSize;
	quint32 m_all_file_size;
	quint32 m_total_number_of_files;
	QByteArray transferPacket;

	QTcpServer *tcpServer;
	bool listenSocket;
	quint16 keyProxyPort;

        QString getFileSize(quint32) const;
	bool connectedToAolProxy;
	bool everyThingIsDone;
	void recreateSocket();
	quint32 speed;
	QTime lastTime;

	void setRemainTime();
	QByteArray utf8toUnicode( const QString &);
	quint16 m_listen_port;
};*/

#endif // FILETRANSFERWINDOW_H
