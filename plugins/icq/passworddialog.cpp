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

#include "passworddialog.h"

passwordDialog::passwordDialog(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    resetSettings();
    setFixedSize(size());
    connect(ui.passwordEdit, SIGNAL(textChanged ( const QString &) ),
            this, SLOT(okEnable(const QString &)));
    connect(ui.saveBox, SIGNAL(stateChanged(int)),
            this, SLOT(savePass(int)));
}

passwordDialog::~passwordDialog(){
    //
}

void passwordDialog::okEnable(const QString &text)
{
    ui.saveButton->setEnabled(text != "");
    password = text;
}

void passwordDialog::resetSettings()
{
    ui.passwordEdit->clear();
    ui.saveBox->setChecked(false);
    savePassword = false;
}

void passwordDialog::setTitle(const QString &account) {
    setWindowTitle(tr("Enter %1 password").arg(account));
}

