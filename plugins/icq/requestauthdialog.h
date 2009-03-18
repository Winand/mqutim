/*
    requestAuthDialog

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


#ifndef REQUESTAUTHDIALOG_H
#define REQUESTAUTHDIALOG_H

#include <QtGui>
#include "ui_requestauthdialog.h"

class requestAuthDialog : public QDialog
{
    Q_OBJECT

public:
    requestAuthDialog(QWidget *parent = 0);
    ~requestAuthDialog();
    QString getMessage(){return ui.textEdit->toPlainText();}

private:
    Ui::requestAuthDialogClass ui;
};

#endif // REQUESTAUTHDIALOG_H
