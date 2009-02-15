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

#include "VCardLabel.h"

VCardLabel::VCardLabel(bool mode, QWidget *parent) : QLabel(parent)
{
	m_mode = mode;
	setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::LinksAccessibleByKeyboard|Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	setSizePolicy(sizePolicy);
	setFrameShape(QFrame::StyledPanel);
	setFrameShadow(QFrame::Plain);
	if (m_mode)
	{
		setFocusPolicy(Qt::StrongFocus);
	}
}

VCardLabel::~VCardLabel()
{
}

void VCardLabel::focusInEvent(QFocusEvent *event)
{
	//setFrameShape(QFrame::StyledPanel);
	setFrameShadow(QFrame::Sunken);
	if (m_mode)
		emit editMode();
}

void VCardLabel::focusOutEvent(QFocusEvent *event)
{
	//setFrameShape(QFrame::NoFrame);
	setFrameShadow(QFrame::Plain);
}
