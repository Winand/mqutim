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


#ifndef FILEREQUESTWINDOW_H
#define FILEREQUESTWINDOW_H

#include <QtGui>
#include "ui_filerequestwindow.h"
#include <QHostAddress>

class fileRequestWindow : public QWidget
{
    Q_OBJECT

public:
    fileRequestWindow(QWidget *parent = 0);
    ~fileRequestWindow();
    void setSengingData(const QString &, const QString &, quint32, quint32, quint16);
    QString uin;
    QByteArray cookie;
    
private slots:
	void on_declineButton_clicked();
	void on_acceptButton_clicked();
signals:
	void cancelSending(QByteArray &,const QString &);
	void fileAccepted(const QByteArray &, const QString &, const QString &,quint32, quint16,quint32);
private:
    Ui::fileRequestWindowClass ui;
    QPoint desktopCenter() const;
    QString getFileSize(quint32) const;
    QString getFileName;
    quint32 peerip;
    quint16 peerport;
    quint32 gettedFileSize;


};

#endif // FILEREQUESTWINDOW_H
