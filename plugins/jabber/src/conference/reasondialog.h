/*
    acceptAuthDialog

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


#ifndef ACCEPTAUTHDIALOG_H
#define ACCEPTAUTHDIALOG_H

#include <QtGui>
#include "ui_reasondialog.h"

class ReasonDialog : public QDialog
{
    Q_OBJECT

public:
    ReasonDialog(QWidget *parent = 0);
    ~ReasonDialog();
	QString m_reason_text;
private slots:
	void on_okButton_clicked()
	{
		m_reason_text = ui.reasonText->toPlainText();
		accept();
	}
private:
	QPoint desktopCenter();
    Ui::Dialog ui;

};

#endif // ACCEPTAUTHDIALOG_H
