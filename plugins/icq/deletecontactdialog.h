/*
    deleteContactDialog

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

#ifndef DELETECONTACTDIALOG_H
#define DELETECONTACTDIALOG_H

#include <QtGui>
#include "ui_deletecontactdialog.h"

class deleteContactDialog : public QDialog
{
    Q_OBJECT

public:
    deleteContactDialog(QWidget *parent = 0);
    ~deleteContactDialog();
    bool deleteHistory(){return ui.checkBox->isChecked();}
private:
    Ui::deleteContactDialogClass ui;
};

#endif // DELETECONTACTDIALOG_H
