/*
    topicConfigDialog.h

    Copyright (c) 2008 by Nikita '∆z' Belov <null@deltaz.ru>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef TOPICCONFIGDIALOG_H
#define TOPICCONFIGDIALOG_H

#include <QtGui>
#include "ui_topicConfig.h"

class topicConfigDialog : public QDialog
{
    Q_OBJECT

public:
	topicConfigDialog(QWidget *parent=0);
	virtual ~topicConfigDialog();
	
	inline void setTopic(QString topic) { ui.topicEdit->setPlainText(topic); }
	
	inline QString getTopic() const { return ui.topicEdit->toPlainText(); };
	
private:
	Ui::topicConfigDialogClass ui;
	QPoint desktopCenter();
};

#endif
