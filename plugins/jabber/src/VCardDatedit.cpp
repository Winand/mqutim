/*
Copyright (c) 2008 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#include "VCardDatedit.h"

VCardDatedit::VCardDatedit(QWidget *parent) : QDateEdit(parent)
{
	setMinimumWidth(150);
	setDisplayFormat("d MMMM yyyy");
}

void VCardDatedit::focusOutEvent(QFocusEvent *event)
{
	emit readMode();
}
