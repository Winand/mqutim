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

#ifndef VCARDLINEDIT_H
#define VCARDLINEDIT_H

#include <QLineEdit>

class VCardLinedit : public QLineEdit
{
    Q_OBJECT

    public:
        VCardLinedit(QWidget *parent = 0);

    protected:
        void focusOutEvent(QFocusEvent *event);

    signals:
        void readMode();

};

#endif // VCARDLINEDIT_H
