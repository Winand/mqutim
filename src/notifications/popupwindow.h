/*
    PopupWindow
    TrayTextBrowser

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

#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QtGui/QWidget>
#include "ui_popupwindow.h"
#include "../abstractchatlayer.h"

class AbstractNotificationLayer;

class PopupWindow : public QWidget
{
    Q_OBJECT

public:
    PopupWindow(const QString&, int, int, int, int, int, int,
    		bool f = true, QWidget *parent = 0);
    ~PopupWindow();
    bool firstTrayWindow;
    bool showTray;

    void showTrayMessage();
    void setData(const TreeModelItem &item, const QString &, const QString &, const QString &, const QString &);
    void setSystemData(const QString &, const QString &);
    void setTheme(const QString &, const QString&, const QString&, const QString&, const QString&);
private slots:
	void slideVerticallyUp();
	void slideVerticallyDown();
	void slideHorizontallyRight();
	void slideHorizontallyLeft();
	void startChatSlot();

signals:
	void startChat(const QString &);

protected:
	bool eventFilter(QObject *obj, QEvent *event);
	
private:
	AbstractNotificationLayer &m_notification_layer;
	void setDataHtml(const QString &msg, const QString &picturePath);
	void setSystemDataHtml(const QString &msg);
    Ui::PopupWindowClass ui;
    int position;
    int style;
    int time;
    int positionInStack;
    int moveToPointX;
    int moveToPointY;
    int fromX;
    int fromY;
    void slide();
    bool userMessage;
    QString contactUin;

    QString themePath;
    QString themeHeader;
    QString themeHeaderCSS;
    QString themeContent;
    QString themeContentCSS;
    TreeModelItem m_contact_item;
};

#endif // POPUPWINDOW_H
