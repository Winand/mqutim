/*
    privacyListWindow

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


#ifndef PRIVACYLISTWINDOW_H
#define PRIVACYLISTWINDOW_H

#include <QtGui>
#include "ui_privacylistwindow.h"

class privacyListWindow : public QWidget
{
    Q_OBJECT

public:
    privacyListWindow(const QString &, const QString &profile_name, QWidget *parent = 0);
    ~privacyListWindow();

    void createLists();
    void setOnline(bool);

private slots:

	void on_visibleTreeWidget_itemClicked(QTreeWidgetItem * , int );
	void on_invisibleTreeWidget_itemClicked(QTreeWidgetItem * , int );
	void on_ignoreTreeWidget_itemClicked(QTreeWidgetItem * , int );

signals:
	void openInfo(const QString &, const QString &, const QString &, const QString &);
	void deleteFromPrivacyList(const QString &, int);
private:
    Ui::privacyListWindowClass ui;
    QString accountUin;
    QString m_profile_name;
};

#endif // PRIVACYLISTWINDOW_H
