/*
    ActivityDialog

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


#ifndef ACTIVITYDIALOG_H
#define ACTIVITYDIALOG_H

#include <QtGui>
#include "ui_activitydialog.h"


class ActivityDialog : public QDialog
{
    Q_OBJECT

public:
    ActivityDialog(const QString &account_name, const QString &profile_name,QWidget *parent = 0);
    ~ActivityDialog();
    void setActivity(const QString &general, const QString &specific);
    const QString &getGeneral() { return m_general; }
    const QString &getSpecific() { return m_specific; }
    const QString &getText() { return m_text; }
    
private slots:
	void on_generalList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous ) { onGeneralListCurrentItemChanged(current, previous, ""); }
	void onGeneralListCurrentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous, const QString &specific );
	void on_specificList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );
	void on_chooseButton_clicked();
private:
    QIcon getIcon(const QString &general, const QString &specific = "");
    Ui::activityDialogClass ui;
    QString m_account_name;
    QString m_profile_name;
    QString m_general;
    QString m_specific;
    QString m_text;
};

#endif // ACTIVITYDIALOG_H
