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

#include "VCardRecord.h"

VCardRecord::VCardRecord(bool mode, const QString &type, QWidget *parent) : VCardEntry(parent)
{
	m_mode = mode;
	m_type = type;
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
		editField = new VCardLinedit();
		editField->setVisible(false);
		connect(editField, SIGNAL(readMode()), this, SLOT(setLabelRead()));
		layout->addWidget(editField);
	}
	buttonPos = 2;
	if (m_type == "url")
	{
		setHint(tr("Site:"));
	}
	else if (m_type == "email" || m_type == "phone")
	{
		labelStatus = new VCardRole(m_mode, m_type);
		layout->addWidget(labelStatus);
		QSpacerItem *horisontalSpacer = new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
//		layout->addSpacerItem(horisontalSpacer);
		buttonPos = 3;
	}
	else if (m_type == "orgname")
	{
		setHint(tr("Company:"));
	}
	else if (m_type == "orgunit")
	{
		setHint(tr("Department:"));
	}
	else if (m_type == "title")
	{
		setHint(tr("Title:"));
	}
	else if (m_type == "role")
	{
		setHint(tr("Role:"));
	}
	else if (m_type == "workcountry" || type == "homecountry")
	{
		setHint(tr("Country:"));
	}
	else if (m_type == "workregion" || type == "homeregion")
	{
		setHint(tr("Region:"));
	}
	else if (m_type == "workcity" || type == "homecity")
	{
		setHint(tr("City:"));
	}
	else if (m_type == "workpostcode" || type == "homepostcode")
	{
		setHint(tr("Post code:"));
	}
	else if (m_type == "workstreet" || type == "homestreet")
	{
		setHint(tr("Street:"));
	}
	else if (m_type == "workpostbox" || type == "homepostbox")
	{
		setHint(tr("PO Box:"));
	}
}

void VCardRecord::setHint(const QString &text)
{
	QLabel *label = new QLabel(text);
	label->setFrameShape(QFrame::StyledPanel);
	label->setFrameShadow(QFrame::Plain);
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	label->setSizePolicy(sizePolicy);
	layout->insertWidget(0, label);
	buttonPos = 3;
}

void VCardRecord::setLabelEdit()
{
	m_editable = true;
	emit mouseOut();
	if (labelText->text() == VCardConst::emptyString())
		editField->setText("");
	else
		editField->setText(labelText->text());
	editField->setVisible(true);
	labelText->setVisible(false);
}

void VCardRecord::setLabelRead()
{
	m_editable = false;
	if (editField->text() == "")
		labelText->setText(VCardConst::emptyString());
	else
		labelText->setText(editField->text());
	editField->setVisible(false);
	labelText->setVisible(true);
}

void VCardRecord::addDeleteButton(QToolButton *button)
{
	button->setFixedHeight(labelText->height());
	layout->insertWidget(buttonPos, button);
	button->setVisible(true);
}

void VCardRecord::enterEvent(QEvent *event)
{
	if (m_mode && !m_editable)
		emit mouseOver();
}

void VCardRecord::leaveEvent(QEvent *event)
{
	if (m_mode)
		emit mouseOut();
}

void VCardRecord::setText(const QString &text)
{
	labelText->setText(text);
}

void VCardRecord::setStatus(const QString &text)
{
	labelStatus->setStatus(text);
}

void VCardRecord::setFont(QFont font)
{
	labelText->setFont(font);
}

QString VCardRecord::text()
{
	return labelText->text();
}

QString VCardRecord::status()
{
	if (m_type == "email" || m_type == "phone")
		return labelStatus->status();
	else
		return "not statused entry";
}

QString VCardRecord::type()
{
	return m_type;
}
