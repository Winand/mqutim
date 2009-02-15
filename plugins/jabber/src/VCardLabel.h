/*
Copyright (c) 2008 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef VCARDLABEL_H
#define VCARDLABEL_H

#include <QApplication>
#include <QLineEdit>
#include <QLabel>
#include <QKeyEvent>
#include <QTextCursor>

class VCardLabel : public QLabel
{
	Q_OBJECT

	public:
                VCardLabel(bool mode, QWidget *parent = 0);
                ~VCardLabel();

	protected:
		void focusInEvent(QFocusEvent *event);
                void focusOutEvent(QFocusEvent *event);
                //void keyPressEvent(QKeyEvent *event);

        signals:
                void editMode();

	private:
                bool m_mode;

};

#endif // VCARDLABEL_H
