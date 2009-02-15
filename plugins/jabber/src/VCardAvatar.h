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

#ifndef VCARDAVATAR_H
#define VCARDAVATAR_H

#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

#include "VCardEntry.h"

class VCardAvatar : public VCardEntry
{
	Q_OBJECT

	public:
		VCardAvatar(bool mode, QWidget *parent = 0);
		void setPhoto(const QString &file);
		virtual QString type() {return "avatar";};
		virtual void addDeleteButton(QToolButton *button);

	protected:
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

	signals:
		void mouseOver();
		void mouseOut();

	private:
		QSize getPictureSize(const QString &file);
		bool m_mode, isEmpty;
		QHBoxLayout *layout;
		QVBoxLayout *buttonLayout;
		QLabel *label;
		QToolButton *button;
		static const int limitWidth = 96, limitHeight = 96;

};

#endif // VCARDAVATAR_H
