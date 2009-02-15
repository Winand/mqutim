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

#ifndef VCARDBIRTHDAY_H
#define VCARDBIRTHDAY_H

#include <QHBoxLayout>
#include <QToolButton>

#include "VCardLabel.h"
#include "VCardDatedit.h"
#include "VCardEntry.h"

class VCardRole;

class VCardBirthday : public VCardEntry
{
	Q_OBJECT

	public:
		VCardBirthday(bool mode, QWidget *parent = 0);
		void setDate(const QString &date);
		QString date();
		virtual QString type();
		virtual void addDeleteButton(QToolButton *button);

	protected:
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

	signals:
		void mouseOver();
		void mouseOut();

	private slots:
		void setLabelEdit();
		void setLabelRead();

	private:
		bool m_mode, m_editable, isISO;
		QString m_date;
		QHBoxLayout *layout;
		VCardLabel *labelText;
		VCardDatedit *editField;
};

#endif // VCARDBIRTHDAY_H
