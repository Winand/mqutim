/*
    addBuddyDialog

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


#ifndef ADDBUDDYDIALOG_H
#define ADDBUDDYDIALOG_H

#include <QtGui>
#include "ui_addbuddydialog.h"

class addBuddyDialog : public QDialog
{
    Q_OBJECT

public:
    addBuddyDialog(QWidget *parent = 0);
    ~addBuddyDialog();

    void setTitle(const QString &t) { setWindowTitle(tr("Add %1").arg(t));}
    void setContactData(const QString &, const QStringList &);
    inline QString getNick() const { return ui.nickEdit->text(); };
    inline QString getGroup() const { return ui.groupList->currentText(); };
    void setMoveData(const QStringList &);
private:
    Ui::addBuddyDialogClass ui;
};

#endif // ADDBUDDYDIALOG_H
