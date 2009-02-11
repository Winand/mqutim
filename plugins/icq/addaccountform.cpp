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

#include "addaccountform.h"

AddAccountForm::AddAccountForm(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	
	QRegExp rx("[1-9][0-9]{1,9}");
	QValidator *validator = new QRegExpValidator(rx, this);
	ui.uinEdit->setValidator(validator);
}

AddAccountForm::~AddAccountForm()
{

}
