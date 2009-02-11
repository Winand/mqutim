/*
    accountButton

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


#include "accountbutton.h"

accountButton::accountButton(QWidget *parent): QToolButton(parent)
{
//	setText("");
	setMinimumSize(QSize(22, 22));
	setMaximumSize(QSize(22, 22));
//	setFlat(true);
	setAutoRaise(true);
}

accountButton::~accountButton()
{

}
