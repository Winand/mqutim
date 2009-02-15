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

#include "VCardLinedit.h"

VCardLinedit::VCardLinedit(QWidget *parent) : QLineEdit(parent)
{
    setMinimumWidth(150);
}

void VCardLinedit::focusOutEvent(QFocusEvent *event)
{
    emit readMode();
}
