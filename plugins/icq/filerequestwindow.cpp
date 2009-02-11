/*
    fileRequestWindow

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


#include "filerequestwindow.h"

fileRequestWindow::fileRequestWindow(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	setFixedSize(size());
	move(desktopCenter());
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	gettedFileSize = 0;

}

fileRequestWindow::~fileRequestWindow()
{

}
QPoint fileRequestWindow::desktopCenter() const
{
	QDesktopWidget desktop;
	return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void fileRequestWindow::setSengingData(const QString &name, const QString &fileName, 
		quint32 ip, quint32 fileSize, quint16 port)
{
	peerip = ip;
	peerport = port;
	getFileName = fileName;
	gettedFileSize = fileSize;
	ui.fromLabel->setText(name);
	ui.fileNameLabel->setText(fileName);
	
	QHostAddress addr(ip);
	
	ui.ipLabel->setText(addr.toString());
	ui.fileSizeLabel->setText(getFileSize(fileSize));
	
}

QString fileRequestWindow::getFileSize(quint32 size) const
{
	quint16 bytes = size % 1024;
	quint16 kBytes = size % (1024 * 1024) / 1024;
	quint16 mBytes = size % (1024 * 1024 * 1024) / (1024 * 1024);
	quint16 gBytes = size / (1024 * 1024 * 1024);
	
	QString fileSize;
	
	if ( gBytes )
		fileSize.append(QString::number(gBytes)+",");
	if ( gBytes || mBytes )
		fileSize.append(QString::number(mBytes)+",");
	if ( gBytes || mBytes || kBytes )
		fileSize.append(QString::number(kBytes)+",");
	if ( gBytes || mBytes || kBytes || bytes )
		fileSize.append(QString::number(bytes));
	
	return fileSize;
}

void fileRequestWindow::on_declineButton_clicked()
{
	emit cancelSending(cookie, uin);
	close();
}

void fileRequestWindow::on_acceptButton_clicked()
{
        QString fileName = QString("");/*QFileDialog::getSaveFileName(this, tr("Save File"),
	                            "" + getFileName,
                                    tr("All files (*)"));*/
	if ( !fileName.isEmpty() )
	{
		emit fileAccepted(cookie, uin, fileName, peerip, peerport, gettedFileSize);
		close();
	}
}
