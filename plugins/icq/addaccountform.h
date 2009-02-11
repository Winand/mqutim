/*
   AddAccountForm

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

#ifndef ADDACCOUNTFORM_H
#define ADDACCOUNTFORM_H

#include <QWidget>
#include <QRegExp>
#include <QValidator>
#include "ui_addaccountform.h"

class AddAccountForm : public QWidget
{
    Q_OBJECT

public:
    AddAccountForm(QWidget *parent = 0);
    ~AddAccountForm();
	QString getName() const {return ui.uinEdit->text();}
	QString getPass() const {return ui.passwordEdit->text();}
	bool getSavePass() const {return ui.passwordBox->isChecked(); }

private:
    Ui::AddAccountFormClass ui;
};

#endif // ADDACCOUNTFORM_H
