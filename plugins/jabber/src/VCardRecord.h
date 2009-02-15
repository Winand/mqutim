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

#ifndef VCARDRECORD_H
#define VCARDRECORD_H

#include <QHBoxLayout>
#include <QToolButton>

#include "VCardLabel.h"
#include "VCardRole.h"
#include "VCardLinedit.h"
#include "VCardConst.h"
#include "VCardEntry.h"

class VCardRole;

class VCardRecord : public VCardEntry
{
	Q_OBJECT

	public:
		VCardRecord(bool mode, const QString &type, QWidget *parent = 0);
		void setText(const QString &text);
		void setStatus(const QString &text);
		void setFont(QFont text);
		QString text();
		QString status();
		QString type();
		void addDeleteButton(QToolButton *button);

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
		void setHint(const QString &text);
		QString m_type;
		bool m_mode, m_editable;
		int buttonPos;
		QHBoxLayout *layout;
		VCardLabel *labelText;
		VCardRole *labelStatus;
		VCardLinedit *editField;
};

#endif // VCARDRECORD_H
