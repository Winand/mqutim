/*
    passwordChangeDialog

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

#ifndef PASSWORDCHANGEDIALOG_H
#define PASSWORDCHANGEDIALOG_H

#include <QtGui>
#include "ui_passwordchangedialog.h"

class passwordChangeDialog : public QDialog
{
    Q_OBJECT

public:
    passwordChangeDialog(const QString &, const QString &profile_name, QWidget *parent = 0);
    ~passwordChangeDialog();
   QString newPass;
private slots:
	void on_changeButton_clicked();

private:
    Ui::passwordChangeDialogClass ui;
    QString ownerUin;
    QString m_profile_name;
};

#endif // PASSWORDCHANGEDIALOG_H
