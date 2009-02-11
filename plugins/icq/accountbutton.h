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


#ifndef ACCOUNTBUTTON_H
#define ACCOUNTBUTTON_H

#include <QToolButton>

class accountButton : public QToolButton
{
    Q_OBJECT

public:
    accountButton(QWidget *parent = 0);
    ~accountButton();

private:

};

#endif // ACCOUNTBUTTON_H
