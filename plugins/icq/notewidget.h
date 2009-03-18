/*
    noteWidget

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

#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QtGui>
#include "ui_notewidget.h"

class noteWidget : public QWidget
{
    Q_OBJECT

public:
    noteWidget(const QString &, const QString &, const QString &, const QString &profile_name, QWidget *parent = 0);
    ~noteWidget();

private slots:
	void on_okButton_clicked();
    
private:
    Ui::noteWidgetClass ui;
    QString contactUin;
    QString mineUin;
    QString m_profile_name;
};

#endif // NOTEWIDGET_H
