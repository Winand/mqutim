/*
    addRenameDialog

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


#ifndef ADDRENAMEDIALOG_H
#define ADDRENAMEDIALOG_H

#include <QtGui>
#include "ui_addrenamedialog.h"

class addRenameDialog : public QDialog
{
    Q_OBJECT

public:
    addRenameDialog(QWidget *parent = 0);
    ~addRenameDialog();
    QString name;

private slots:
	void on_lineEdit_textChanged(const QString &);

private:
    Ui::addRenameDialogClass ui;
};

#endif // ADDRENAMEDIALOG_H
