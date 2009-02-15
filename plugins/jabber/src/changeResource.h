/*
    changeResource.h

    Copyright (c) 2008 by Nikita Belov <zodiac.nv@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef CHANGERESOURCE_H
#define CHANGERESOURCE_H

#include <QtGui>
#include "ui_wChangeResource.h"

class changeResource : public QDialog
{
    Q_OBJECT

public:
	changeResource(QWidget *parent=0);
	virtual ~changeResource();
	
	inline void setResource(QString resource) { ui.resourceEdit->setText(resource); }
	inline void setPriority(int priority) { ui.priorityEdit->setText(QString::number(priority)); }
	
	inline QString getResource() const { return ui.resourceEdit->text(); };
	inline int getPriority() const { return ui.priorityEdit->text().toInt(); };
	
private:
	Ui::changeResourceClass ui;
	QPoint desktopCenter();
};

#endif
