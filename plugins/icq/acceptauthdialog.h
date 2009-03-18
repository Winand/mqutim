/*
    acceptAuthDialog

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


#ifndef ACCEPTAUTHDIALOG_H
#define ACCEPTAUTHDIALOG_H

#include <QtGui>
#include "ui_acceptauthdialog.h"

class acceptAuthDialog : public QDialog
{
    Q_OBJECT

public:
    acceptAuthDialog(const QString &, QWidget *parent = 0);
    ~acceptAuthDialog();
    void setMessage(const QString &t){ui.textBrowser->setPlainText(t);}
    bool acceptAuth;
private slots:
	void on_acceptButton_clicked();
	void on_declineButton_clicked();
signals:
	void sendAuthReqAnswer(bool, const QString &);
private:
    Ui::acceptAuthDialogClass ui;
    QString uin;
};

#endif // ACCEPTAUTHDIALOG_H
