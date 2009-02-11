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

#include "VCardBirthday.h"

VCardBirthday::VCardBirthday(bool mode, QWidget *parent) : VCardEntry(parent)
{
	m_mode = mode;
	m_editable = false;
	layout = new QHBoxLayout();
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);
	labelText = new VCardLabel(m_mode);
	connect(labelText, SIGNAL(editMode()), this, SLOT(setLabelEdit()));
	layout->addWidget(labelText);
	if (m_mode)
	{
		editField = new VCardDatedit();
		editField->setVisible(false);
		connect(editField, SIGNAL(readMode()), this, SLOT(setLabelRead()));
		layout->addWidget(editField);
	}
	QLabel *label = new QLabel(tr("Birthday:"));
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	label->setFrameShape(QFrame::StyledPanel);
	label->setFrameShadow(QFrame::Plain);
	label->setSizePolicy(sizePolicy);
	layout->insertWidget(0, label);
	QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	layout->addItem(horizontalSpacer);
}

void VCardBirthday::setLabelEdit()
{
	m_editable = true;
	emit mouseOut();
	QDate date;
	if (!isISO)
		date = QDate::currentDate();
	else
		date = QDate::fromString(m_date, "yyyy-MM-dd");
	editField->setDate(date);
	editField->setVisible(true);
	labelText->setVisible(false);
}

void VCardBirthday::setLabelRead()
{
	m_editable = false;
	if (editField->date() != QDate::currentDate() || isISO)
	{
		isISO = true;
		m_date = editField->date().toString("yyyy-MM-dd");
		labelText->setText(QDate::fromString(m_date, "yyyy-MM-dd").toString("d MMMM yyyy"));
	}
	editField->setVisible(false);
	labelText->setVisible(true);
}

void VCardBirthday::addDeleteButton(QToolButton *button)
{
	button->setFixedHeight(labelText->height());
	layout->insertWidget(3, button);
	button->setVisible(true);
}

void VCardBirthday::enterEvent(QEvent *event)
{
	if (m_mode && !m_editable)
		emit mouseOver();
}

void VCardBirthday::leaveEvent(QEvent *event)
{
	if (m_mode)
		emit mouseOut();
}

void VCardBirthday::setDate(const QString &text)
{
	m_date = text;
	isISO = QDate::fromString(m_date, "yyyy-MM-dd").isValid();
	if (isISO)
		labelText->setText(QDate::fromString(m_date,"yyyy-MM-dd").toString("d MMMM yyyy"));
	else
		labelText->setText(tr("%1&nbsp;(<font color='#808080'>wrong date format</font>)").arg(m_date));
}

QString VCardBirthday::date()
{
	return m_date;
}

QString VCardBirthday::type()
{
	return "birthday";
}
