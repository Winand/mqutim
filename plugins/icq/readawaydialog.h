/*
    readAwayDialog

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


#ifndef READAWAYDIALOG_H
#define READAWAYDIALOG_H

#include <QDialog>
#include "ui_readawaydialog.h"

class readAwayDialog : public QDialog
{
    Q_OBJECT

public:
    readAwayDialog(QWidget *parent = 0);
    ~readAwayDialog();
    void addMessage(QString &);
    void addXstatus(QString &t){ui.awayMessage->setHtml(t.replace("\n", "<br>"));}

private:
    Ui::readAwayDialogClass ui;
};

#endif // READAWAYDIALOG_H
