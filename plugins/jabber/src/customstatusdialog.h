/*
    customStatusDialog

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>
    						Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/


#ifndef CUSTOMSTATUSDIALOG_H
#define CUSTOMSTATUSDIALOG_H

#include <QtGui>
#include "ui_customstatusdialog.h"


class CustomStatusDialog : public QDialog
{
    Q_OBJECT

public:
    CustomStatusDialog(const QString &, const QString &profile_name,QWidget *parent = 0);
    ~CustomStatusDialog();
    void setStatuses(const QString &);
    QString current_mood;
    QString status_message;
    void setCaption(const QString &t){ui.captionEdit->setText(t);}
    void setMessage(const QString &t){ui.awayEdit->setPlainText(t);}
private slots:
	void on_iconList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );
    void on_chooseButton_clicked();
private:
    QIcon getIcon(const QString &name);
    Ui::customStatusDialogClass ui;
    QList<QListWidgetItem *> itemList;
    QString mineUin;
    QString m_profile_name;
};

#endif // CUSTOMSTATUSDIALOG_H
