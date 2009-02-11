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

#include "joinconferenceform.h"
#include "jConference.h"

#include <QDesktopWidget>

JoinConferenceForm::JoinConferenceForm(jConference *real_parent, QWidget *parent)
    : QWidget(parent)
    , m_real_parent(real_parent)
{
	ui.setupUi(this);
	move(desktopCenter());
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
}

JoinConferenceForm::~JoinConferenceForm()
{

}

QPoint JoinConferenceForm::desktopCenter()
{
	QDesktopWidget desktop;
	return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void JoinConferenceForm::setIcons(const QIcon &apply_icon, const QIcon &cancel_icon)
{
	ui.joinButton->setIcon(apply_icon);
	ui.cancelButton->setIcon(cancel_icon);
}

void JoinConferenceForm::on_joinButton_clicked()
{
	m_real_parent->createConferenceRoom(ui.hostEdit->text(),
			ui.roomEdit->text(),
			ui.nicknameEdit->text(),
			ui.passwordEdit->text());
	close();
}
