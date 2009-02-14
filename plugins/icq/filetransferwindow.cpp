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


/*#include "filetransferwindow.h"
#include "icqpluginsystem.h"




fileTransferWindow::fileTransferWindow(const QString &muin, const QStringList &list,
		                               const QString & uin,QByteArray & c,bool f, 
		                               quint16 listen_port,
		                               QWidget *parent)
    : QWidget(parent)
    , sendingFile(f)
    , cookie(c)
    , contactUin(uin)
    , fileList(list)
    , mineUin(muin)
    , m_listen_port(listen_port)
{
	ui.setupUi(this);
//	ui.label_4->setVisible(false);
//	ui.filesLabel->setVisible(false);
	ui.label_11->setVisible(false);
	ui.senderLabel->setVisible(false);
	setFixedSize(size());
	move(desktopCenter());
	setWindowTitle(tr("File transfer: %1").arg(uin));
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);

	fileSize = 0;
	tmpFileSize = 0;
	listenSocket = false;
	tcpSocket = new QTcpSocket(this);

	lastTime.setHMS(0,0,0,0);


	connect(tcpSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
	connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten()));
	waitingFor18 = false;

	tcpServer = new QTcpServer(this);
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));


	ui.statusLabel->setText(tr("Waiting..."));

	prevCheckSum = 0xffff0000;
	waitingForFileAccept = false;
	waitingForGoodTransfer = false;
	connectedToProxy = true;
	sendingFileNow = false;
	everyThingIsDone = false;
	m_total_number_of_files = fileList.count();;
	m_all_file_size = 0;
//	currentFile.moveToThread(&mainThread);
//	mainThread.start(QThread::LowPriority);
//	mainThread.test();
//	for(;;);

}

fileTransferWindow::~fileTransferWindow()
{
//	mainThread.terminate();
//	mainThread.wait();
}

QPoint fileTransferWindow::desktopCenter()
{
	QDesktopWidget desktop;
	return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void fileTransferWindow::on_cancelButton_clicked()
{
//	emit cancelSending(cookie);
	close();
}

void fileTransferWindow::closeEvent ( QCloseEvent *  )
{
	emit cancelSending(cookie, contactUin);
}

void fileTransferWindow::sendingDeclined(const QString &uin)
{
	setWindowTitle(tr("File transfer: %1").arg(uin));
	if ( !everyThingIsDone)
		ui.statusLabel->setText(tr("Declined by remote user"));
}

void fileTransferWindow::sendingAccepted(const QString &uin)
{
	setWindowTitle(tr("File transfer: %1").arg(uin));
	ui.statusLabel->setText(tr("Accepted"));

	sendTransferPacket();
}

void fileTransferWindow::socketConnected()
{
//	emit sendingToPeerRequest(cookie, contactUin, fileList);

	if ( connectedToProxy )
	{
		if ( sendingFile)
		{
			QByteArray packet;



			if ( connectedToAolProxy )
			{
				packet.append(convertToByteArray((quint16)(mineUin.toUtf8().length() + 41)));
				packet.append(QByteArray::fromHex("044a0004000000000000"));
			}
			else
			{
				packet.append(convertToByteArray((quint16)(mineUin.toUtf8().length() + 39)));
				packet.append(QByteArray::fromHex("044a0002000000000000"));
			}
			packet.append(convertToByteArray((quint8)mineUin.toUtf8().length()));
			packet.append(mineUin.toUtf8());
			if ( connectedToAolProxy)
				packet.append(convertToByteArray((quint16)keyProxyPort));

			packet.append(cookie);
			packet.append(convertToByteArray((quint16)0x0001));
			packet.append(convertToByteArray((quint16)0x0010));
			packet.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));
			waitingFor18 = true;
			tcpSocket->write(packet);
		} else
		{
			QByteArray packet;

			packet.append(convertToByteArray((quint16)(mineUin.toUtf8().length() + 41)));
			packet.append(QByteArray::fromHex("044a0004000000000000"));
			packet.append(convertToByteArray((quint8)mineUin.toUtf8().length()));
			packet.append(mineUin.toUtf8());

			packet.append(convertToByteArray((quint16)keyProxyPort));
			packet.append(cookie);
			packet.append(convertToByteArray((quint16)0x0001));
			packet.append(convertToByteArray((quint16)0x0010));
			packet.append(QByteArray::fromHex("094613434c7f11d18222444553540000"));
			waitingFor18 = true;
			tcpSocket->write(packet);
		}
	}

}

QByteArray fileTransferWindow::convertToByteArray(const quint16 &d) const
{
	QByteArray packet;
	packet[0] = (d / 0x100);
	packet[1] = (d % 0x100);
	return packet;
}

QByteArray fileTransferWindow::convertToByteArray(const quint32 &d) const
{
	QByteArray packet;
	packet[0] = (d / 0x1000000);
	packet[1] = (d / 0x10000);
	packet[2] = (d / 0x100);
	packet[3] = (d % 0x100);
	return packet;
}

QByteArray fileTransferWindow::convertToByteArray(const quint8 &d) const
{
	QByteArray packet;
	packet[0] = d;
	return packet;
}

void fileTransferWindow::connectToProxy(quint32 ip, quint16 port,bool proxy)
{
	if ( sendingFile )
	{
		if ( ip && port )
		{
			if ( proxy)
			{
				connectedToProxy = true;
				connectedToAolProxy = true;
				keyProxyPort = port;
//				tcpSocket->abort();
				recreateSocket();
				tcpSocket->connectToHost(QHostAddress(ip), 5190);
			}
			else
			{
				connectedToProxy = false;
				connectedToAolProxy = false;
//				tcpSocket->abort();
//				tcpSocket->close();
				recreateSocket();
				tcpSocket->connectToHost(QHostAddress(ip), port);
				QTimer::singleShot(1000, this, SLOT(checkLocalConnection()));
			}
		} else
		{
//			tcpSocket->abort();
			connectedToProxy = true;
			connectedToAolProxy = false;
			recreateSocket();
			tcpSocket->connectToHost("64.12.201.185", 5190);
		}
	} else {
		if ( ip && port )
		{


			waitingForFileAccept = true;
			connectedToProxy = false;
//			tcpSocket->abort();
			recreateSocket();
			tcpSocket->connectToHost(QHostAddress(ip), port);
			QTimer::singleShot(1000, this, SLOT(checkLocalConnection()));
		}
	}

}

void fileTransferWindow::connectToAolProxy(quint32 ip, quint16 port)
{
	if ( !sendingFile )
	{
		tcpServer->close();
		connectedToProxy = true;
		connectedToAolProxy = true;
		keyProxyPort = port;
//		tcpSocket->abort();
		recreateSocket();
		tcpSocket->connectToHost(QHostAddress(ip), 5190);
	}
}


void fileTransferWindow::readFromSocket()
{
	if ( sendingFile )
	{
		if ( tcpSocket->bytesAvailable() == 18 && waitingFor18 && !connectedToAolProxy)
		{
			QByteArray answer = tcpSocket->read(18).right(6);
			quint16 port = byteArrayToInt16(answer.left(2));
			quint32 ip = byteArrayToInt32(answer.right(4));

			waitingFor18 = false;
				emit getRedirectToProxyData(cookie, contactUin, port, ip);

		}
		if ( tcpSocket->bytesAvailable() == 12 && waitingFor18 && connectedToAolProxy)
		{
			waitingFor18 = false;
			emit sendAcceptMessage(cookie, contactUin);
			foreach(QString file_name, fileList)
			{
				QFile f(file_name);
				m_all_file_size += f.size();
			}
			m_total_number_of_files = fileList.count();
			sendTransferPacket();
		}

		if ( tcpSocket->bytesAvailable() >= 256 && waitingForFileAccept)
		{
			QByteArray transferData = tcpSocket->read(256);

			if ( transferData.at(6) == 0x02 && transferData.at(7) == 0x02 )
			{
				waitingForFileAccept = false;
	//			currentFile.moveToThread(&mainThread);
				currentFile.close();
				if ( currentFile.open(QIODevice::ReadOnly))
				{
					ui.progressBar->setValue(0);
					currentFile.seek(0);
					sendingFileNow = true;
					currentFileChunkSize = currentFile.size() < 1360 ? currentFile.size() : 1360;
					ui.statusLabel->setText(tr("Sending..."));
//					lastTime.start();
					tmpFileSize = 0;
					currentFileSize = currentFile.size();
					QFileInfo fileInfo(currentFile.fileName());
					ui.currentFileLabel->setText(fileInfo.fileName());
//					ui.currentFileLabel->setText(currentFile.fileName());
					ui.sizeLabel->setText(getFileSize(currentFileSize));


					updateProgress();

					sendFileData();
				}
			}
		}
		if ( tcpSocket->bytesAvailable()>=256 && waitingForGoodTransfer)
		{
			QByteArray transferData = tcpSocket->read(256);
			if ( transferData.at(6) == 0x02 && transferData.at(7) == 0x04 )
			{
				waitingForGoodTransfer = false;

				if ( !fileList.count() )
				{
					tcpSocket->disconnectFromHost();
					ui.statusLabel->setText(tr("Done"));
					everyThingIsDone = true;
					ui.cancelButton->setIcon(IcqPluginSystem::instance().getIcon("apply"));
					ui.cancelButton->setText(tr("Done"));
					ui.speedLabel->clear();
				}
				else
				{
					sendTransferPacket();
				}
			}
		}
	} else {

		if ( tcpSocket->bytesAvailable() == 12 && waitingFor18)
		{


			waitingFor18 = false;
			emit sendAcceptMessage(cookie, contactUin);
		}

		if ( tcpSocket->bytesAvailable()>= 256 && waitingForFileAccept)
		{
			waitingForFileAccept = false;
			currentFile.setFileName(fileList.at(0));
			currentFile.close();
			if ( currentFile.open(QIODevice::WriteOnly))
			{

				QByteArray transferData = tcpSocket->read(256);

				tmpFileSize = 0;

				transferData[6] = 0x02;
				transferData[7] = 0x02;

				transferData.replace(8,8,cookie);

				transferPacket = transferData;
				tcpSocket->write(transferData);
				sendingFileNow = true;


				tmpFileSize = 0;
				currentFileSize = fileSize;
				QFileInfo fileInfo(currentFile.fileName());

				ui.currentFileLabel->setText(fileInfo.fileName());

				ui.sizeLabel->setText(getFileSize(currentFileSize));

				updateProgress();

				ui.statusLabel->setText(tr("Getting..."));
//				lastTime.start();
			}

		}

		else if ( sendingFileNow )
		{

			tmpFileSize += tcpSocket->bytesAvailable();
			speed += tcpSocket->bytesAvailable();
			currentFile.write(tcpSocket->readAll());
			if (tmpFileSize >= fileSize )
			{
				currentFile.close();
 				ui.statusLabel->setText(tr("Done"));
				ui.cancelButton->setIcon(IcqPluginSystem::instance().getIcon("apply"));
				ui.cancelButton->setText(tr("Done"));
				ui.speedLabel->clear();
				transferPacket[7] = 0x04;
				tcpSocket->readAll();
				tcpSocket->write(transferPacket);
				tcpSocket->disconnectFromHost();
				tcpServer->close();
				return;
			}
		}
	}
	tcpSocket->readAll();
}

quint16 fileTransferWindow::byteArrayToInt16(const QByteArray &array) const
{
	bool ok;
	return array.toHex().toUInt(&ok,16);
}

quint32 fileTransferWindow::byteArrayToInt32(const QByteArray &array) const
{
	bool ok;
	return array.toHex().toULong(&ok,16);
}

void fileTransferWindow::sendTransferPacket()
{
	if ( fileList.count() )
	{
		QFile file(fileList.at(0));
	
	
	//	mainThread.start(QThread::LowPriority);
	//	file.moveToThread(&mainThread);
		if (file.open(QIODevice::ReadOnly | QIODevice::Unbuffered))
		{
			currentFile.setFileName(file.fileName());
			QByteArray packet;
			packet.append(QByteArray::fromHex("4f465432"));
			packet.append(convertToByteArray((quint16)0x0100));
			packet.append(convertToByteArray((quint16)0x0101));
			packet.append(cookie);
			packet.append(convertToByteArray((quint16)0));
			packet.append(convertToByteArray((quint16)0));
	
			//total number of files
	
			packet.append(convertToByteArray((quint16)m_total_number_of_files));
	
			//files left
	
			packet.append(convertToByteArray((quint16)fileList.count()));
	
			packet.append(convertToByteArray((quint16)0x0001));
			packet.append(convertToByteArray((quint16)0x0001));
	
	
			//all size
	
			packet.append(convertToByteArray((quint32)m_all_file_size));
	
			//current file size
			packet.append(convertToByteArray((quint32)file.size()));
	
	
			packet.append(convertToByteArray((quint32)0));
	
	
	
	
			packet.append(convertToByteArray((quint32)fileCheckSum(file, file.size())));
			file.seek(0);
			packet.append(convertToByteArray((quint32)0xffff0000));
			packet.append(convertToByteArray((quint32)0));
			packet.append(convertToByteArray((quint32)0));
			packet.append(convertToByteArray((quint32)0xffff0000));
			packet.append(convertToByteArray((quint32)0));
			packet.append(convertToByteArray((quint32)0xffff0000));
	
			packet.append(QByteArray::fromHex("436f6f6c2046696c6558666572"));
	
			for ( int i = 0; i < 19; i++)
				packet.append(QChar(0x00));
	
			packet.append(convertToByteArray((quint8)0x20));
			packet.append(convertToByteArray((quint16)0x1c11));
			for ( int i = 0; i < 85; i++)
				packet.append(QChar(0x00));
			packet.append(convertToByteArray((quint32)0));
	
	
			QFileInfo fileInfo(file.fileName());
	//		QByteArray fileName64(utf8toUnicode(fileInfo.fileName()));
	
			QByteArray fileName64;
			fileName64.append(fileInfo.fileName());
	//		fileName64.append(fileInfo.fileName().toUtf8());
			qint64 fileNameLength = fileName64.length();
	
	
			if ( fileNameLength > 64 )
				fileName64 = fileName64.left(64);
			else
			{
				for ( int i = 0; i < 64 - fileNameLength; i++ )
					fileName64.append(QChar(0x00));
			}
	
			packet.append(fileName64);
	
			waitingForFileAccept = true;
			tcpSocket->write(packet);
			fileList.removeAt(0);
			
			ui.filesLabel->setText(QString("%1/%2").arg(m_total_number_of_files
					- fileList.count())
					.arg(m_total_number_of_files));
		}
	}
}

quint32 fileTransferWindow::fileCheckSum(QFile &file, quint32 size) const
{
	quint32 bytesRead = 0;
	int bufferSize = 10240;

//	while (bytesRead < size) //not sure that any client use this for real
//	{
		QByteArray buffer = file.read(qMin(bufferSize, (int)(size - bytesRead)));
		file.seek(file.pos() + buffer.length());
//		if (buffer.length() == 0)
//			break;
		bytesRead += buffer.length();

		quint32 check = (prevCheckSum >> 16) & 0xffff;
		quint32 oldcheck;
		quint16 val;

		for (int i = 0; i < buffer.length(); i++)
		{
			oldcheck = check;
			quint8 byte = buffer.at(i);

			if (i&1)
				val = byte;
			else
				val = byte << 8;
			check -= val;

			if (check > oldcheck)
				check--;
			file.seek(file.pos() + 1);
		}
		check = ((check & 0x0000ffff) + (check >> 16));
		check = ((check & 0x0000ffff) + (check >> 16));
		return check << 16;


//	}

}

void fileTransferWindow::sendFileData()
{
	qint64 tmpL = currentFile.size() - currentFile.pos();

	if ( tmpL < 1 )
	{
		waitingForGoodTransfer = true;
		currentFile.close();
//		ui.statusLabel->setText(tr("Done"));
//		tcpSocket->disconnectFromHost();
		return;
	}

	tcpSocket->write(currentFile.peek(currentFileChunkSize));

	tmpFileSize = currentFile.pos() + currentFileChunkSize;
//	updateProgress();
	currentFile.seek(currentFile.pos() + currentFileChunkSize);
	speed += currentFileChunkSize;

	if ( connectedToProxy )
		currentFileChunkSize = tmpL < 1360 ? tmpL : 1360;
	else
		currentFileChunkSize = tmpL < 8000 ? tmpL : 8000;


//	QTimer::singleShot(50, this, SLOT(sendFileData()));

}

void fileTransferWindow::checkLocalConnection()
{
	if ( tcpSocket->state() == QAbstractSocket::ConnectedState)
	{
		if ( sendingFile )
		{
			waitingForFileAccept = true;
			emit sendAcceptMessage(cookie, contactUin);
			QTimer::singleShot(500, this, SLOT(sendTransferPacket()));
		} else {
			emit sendAcceptMessage(cookie, contactUin);
		}
	} else {
		if ( sendingFile)
		{
			connectedToProxy = true;
			connectedToAolProxy = false;
//			tcpSocket->abort();
//			tcpSocket->close();
			recreateSocket();
			tcpSocket->connectToHost("64.12.201.185", 5190);
		} else {
			tcpServer->listen(QHostAddress::Any, m_listen_port);
			emit sendRedirectToMineServer(cookie, contactUin, m_listen_port);
		}
	}
}

void fileTransferWindow::bytesWritten()
{
	if ( sendingFileNow && sendingFile)
	{
		sendFileData();
	}
}

void fileTransferWindow::slotNewConnection()
{
	if ( !listenSocket )
	{
		delete tcpSocket;

		tcpSocket = tcpServer->nextPendingConnection();
		connect(tcpSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
		connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
		connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten()));
		listenSocket = true;
	}
}

void fileTransferWindow::updateProgress()
{
	if ( tmpFileSize > currentFileSize )
	{
		waitingForGoodTransfer = true;
		ui.doneLabel->setText(getFileSize(currentFileSize));
		ui.progressBar->setValue(100);
		ui.speedLabel->clear();
		currentFile.close();
//		ui.statusLabel->setText(tr("Done"));
	}
	else
	{
		ui.doneLabel->setText(getFileSize(tmpFileSize));

		ui.progressBar->setValue(100 * ((float)tmpFileSize / currentFileSize));
//		ui.progressBar->setValue(100 * ((float)tmpFileSize / m_all_file_size));
		ui.speedLabel->setText(getFileSize(speed) + tr("/s"));
		setRemainTime();
		speed = 0;
//		tmpTime.addSecs(lastTime.elapsed());
		ui.lastLabel->setText(lastTime.toString());
		lastTime = lastTime.addSecs(1);

		if ( tcpSocket->state() == QAbstractSocket::ConnectedState )
			QTimer::singleShot(1000, this, SLOT(updateProgress()));
	}
}

QString fileTransferWindow::getFileSize(quint32 size) const
{
	quint16 bytes = size % 1024;
	quint16 kBytes = size % (1024 * 1024) / 1024;
	quint16 mBytes = size % (1024 * 1024 * 1024) / (1024 * 1024);
	quint16 gBytes = size / (1024 * 1024 * 1024);

	QString fileSize;

//	if ( gBytes )
//		fileSize.append(QString::number(gBytes)+",");
//	if ( gBytes || mBytes )
//		fileSize.append(QString::number(mBytes)+",");
//	if ( gBytes || mBytes || kBytes )
//		fileSize.append(QString::number(kBytes)+",");
//	if ( gBytes || mBytes || kBytes || bytes )
//		fileSize.append(QString::number(bytes));

	if ( bytes && !kBytes && !mBytes && !gBytes )
		fileSize.append(QString::number(bytes) + tr(" B"));
	else if (kBytes && !mBytes && !gBytes )
		fileSize.append(QString::number(kBytes) + "," + QString::number(bytes) + tr(" KB"));
	else if (mBytes && !gBytes )
		fileSize.append(QString::number(mBytes) + "," + QString::number(kBytes) + tr(" MB"));
	else if (gBytes )
		fileSize.append(QString::number(gBytes) + "," + QString::number(mBytes) + tr(" GB"));

	return fileSize;
}

void fileTransferWindow::recreateSocket()
{
	delete tcpSocket;
	tcpSocket = new QTcpSocket(this);

	connect(tcpSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
	connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten()));
}

void fileTransferWindow::setVisualContactIp(quint32 ip)
{
	ui.label_11->setVisible(true);
	ui.senderLabel->setVisible(true);
	QHostAddress contactIp(ip);
	ui.senderLabel->setText(contactIp.toString());
}

void fileTransferWindow::setRemainTime()
{
	if( speed)
	{
		int secsLeft = (currentFileSize - tmpFileSize) / speed;
		QTime tmpTime(0,0,0);
		ui.remainedLabel->setText(tmpTime.addSecs(secsLeft).toString());
	}
}

void fileTransferWindow::on_openButton_clicked()
{
	QFileInfo fileInfo(currentFile.fileName());
	QDesktopServices::openUrl(QUrl(fileInfo.absoluteDir().path()));
}

QByteArray fileTransferWindow::utf8toUnicode(const QString &message)
{
	QByteArray msg;
	const ushort *array = message.utf16();
	for(;*array;array++)
			msg.append(convertToByteArray((quint16)*array));
	return msg;
}

void fileTransferWindow::setMainConnectionProxy(const QNetworkProxy &p)
{
	tcpSocket->setProxy(p);
	tcpServer->setProxy(p);
}*/
