/*
    JoinConferenceForm

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

#ifndef JOINCONFERENCEFORM_H
#define JOINCONFERENCEFORM_H

#include <QtGui/QWidget>
#include <QPoint>

#include "ui_joinconferenceform.h"

class jConference;

class JoinConferenceForm : public QWidget
{
    Q_OBJECT

public:
    JoinConferenceForm(jConference *real_parent,
    		QWidget *parent = 0);
    ~JoinConferenceForm();
    void setIcons(const QIcon &apply_icon, const QIcon &cancel_icon);

private slots:
	void on_joinButton_clicked();
    
private:
    Ui::JoinConferenceFormClass ui;
	QPoint desktopCenter();
	jConference *m_real_parent;
};

#endif // JOINCONFERENCEFORM_H
