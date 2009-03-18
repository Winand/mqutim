/*
    searchUser

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


#ifndef SEARCHUSER_H
#define SEARCHUSER_H

#include <QtGui>
#include "ui_searchuser.h"


class searchUser : public QWidget
{
    Q_OBJECT

public:
    searchUser(const QString &profile_name, QWidget *parent = 0);
    ~searchUser();

    QString getUin();
    QString getEmail();
    void addFoundedContact(bool, bool, const QString &, const QString &,
			const QString &, const QString &, const QString &,
			const quint8 &, const quint16 &, const quint8&, const quint16 &);



    quint8 gender;
    quint16 minAge;
    quint16 maxAge;
    quint16 countryCode;
    quint16 interestsCode;
    quint16 languageCode;
    quint16 occupationCode;
    bool onlineOnly();
    QString getKeyWords(){ return ui.keyWordEdit->text();}
    QString getCity(){ return ui.cityEdit->text();}
    QString getNick(){ return ui.nickEdit->text();}
    QString getFirst(){ return ui.firstEdit->text();}
    QString getLast(){ return ui.lastEdit->text();}

signals:
	void findAskedUsers(int);
	void openChatWithFounded(const QString &, const QString &);
	void openInfoWindow(const QString &, const QString &, const QString &, const QString &);
	void checkStatusFor(const QString &);
	void addUserToContactList(const QString &, const QString &, bool);

private slots:
 	void on_moreButton_toggled(bool);
 	void on_clearButton_clicked();
 	void on_searchButton_clicked();
 	void on_resultTreeWidget_itemClicked(QTreeWidgetItem *,int );
	void on_resultTreeWidget_customContextMenuRequested ( const QPoint & );

	void addUserActionActivated();
	void checkStatusActionActivated();
	void userInformationActionActivated();
	void sendMessageActionActivated();
	void on_resultTreeWidget_itemDoubleClicked( QTreeWidgetItem *, int );
private:
	void createContextMenu();
	QMenu *contextMenu;
	QAction *addUser;
	QAction *checkStatus;
	QAction *userInformationAction;
	QAction *sendMessageAction;
    Ui::searchUserClass ui;
    QTreeWidgetItem *clickedItemForContext;
    QString m_profile_name;
};

#endif // SEARCHUSER_H
