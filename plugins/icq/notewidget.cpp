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

#include "notewidget.h"

noteWidget::noteWidget(const QString &mu, const QString &cu, const QString &name, const QString &profile_name, QWidget *parent)
    : QWidget(parent)
    , contactUin(cu)
    , mineUin(mu)
    , m_profile_name(profile_name)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowTitle(name);
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+mineUin, "contactlist");
	ui.noteEdit->setPlainText(settings.value(contactUin + "/note", "").toString());
}

noteWidget::~noteWidget() {
    //
}

void noteWidget::on_okButton_clicked()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+mineUin, "contactlist");
	settings.setValue(contactUin + "/note", ui.noteEdit->toPlainText());
	close();
}
