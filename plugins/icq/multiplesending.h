/*
    multipleSending

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


#ifndef MULTIPLESENDING_H
#define MULTIPLESENDING_H

#include <QtGui>

#include "ui_multiplesending.h"

class messageFormat;
class treeGroupItem;
class treeBuddyItem;

class multipleSending : public QWidget
{
    Q_OBJECT

public:
    multipleSending(QWidget *parent = 0);
    ~multipleSending();

    void setTreeModel(const QString &, const QHash<quint16, treeGroupItem *> *, const QHash<QString, treeBuddyItem *> *);

private slots:
	void on_contactListWidget_itemChanged(QTreeWidgetItem *, int);
	void on_sendButton_clicked();
	void on_stopButton_clicked();
	void sendMessage();
signals:
	void sendMessageToContact(const messageFormat &);

private:
    Ui::multipleSendingClass ui;
    QStringList sendToList;
    QTreeWidgetItem *rootItem;
    QTimer *sendTimer;
    int barInterval;
};

#endif // MULTIPLESENDING_H
