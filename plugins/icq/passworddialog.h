/*
    passwordDialog

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


#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QtGui/QDialog>
#include <QDebug>
#include <QEvent>
#include "ui_passworddialog.h"

class passwordDialog : public QDialog
{
    Q_OBJECT

public:
    passwordDialog(QWidget *parent = 0);
    ~passwordDialog();

    QString getPass() { return password; }
    bool getSavePass() { return savePassword; }
    void setTitle(const QString&);
    
private slots:
	void okEnable(const QString &);
	void savePass(int flag) { savePassword = flag; }

private:
    Ui::passwordDialogClass ui;
    void resetSettings();
    QString password;
    bool savePassword;
};

#endif // PASSWORDDIALOG_H
