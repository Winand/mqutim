/*
    emoticonMenu

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


#ifndef CHATEMOTICONMENU_H
#define CHATEMOTICONMENU_H

#include <QtGui>
#include <cmath>
#include "movielabel.h"

class ChatEmoticonMenu : public QWidget
{
    Q_OBJECT

public:
	ChatEmoticonMenu(QWidget *parent = 0);
    ~ChatEmoticonMenu();
    void setEmoticons(const QHash<QString, QString> &, const QString &);

signals:
	void insertSmile(const QString &);
	
protected:

	void hideEvent ( QHideEvent * );
	void showEvent ( QShowEvent * );
    
private:
    QList<movieLabel *> labelList;
    QList<QMovie *> movieList;
    QHash<QString, QString>  emotList;
    QString emotPath;
    QGridLayout *m_grid_layout;
    void clearList();
};

#endif // CHATEMOTICONMENU_H
