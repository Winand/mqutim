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

#include "VCardAvatar.h"

VCardAvatar::VCardAvatar(bool mode, QWidget *parent) : VCardEntry(parent)
{
	m_mode = mode;
	layout = new QHBoxLayout();
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);
	buttonLayout = new QVBoxLayout();
	setMinimumSize(QSize(limitWidth, limitHeight));
	setMaximumSize(QSize(limitWidth, limitHeight));
	label = new QLabel(this);
	label->setMinimumSize(QSize(limitWidth, limitHeight));
	//label->setMaximumSize(QSize(96, 96));
	//label->setGeometry(0, 0, limitWidth, limitHeight);
	layout->addWidget(label);
	layout->addLayout(buttonLayout);
	QSpacerItem *verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
	buttonLayout->addItem(verticalSpacer);

	isEmpty = true;
}

void VCardAvatar::setPhoto(const QString &file)
{
	if (file == "")
	{
		isEmpty = true;
		label->setText("");
	}
	else
	{
		isEmpty = false;
		QSize picSize = getPictureSize(file);
		label->setText(tr("<img src='%1' width='%2' height='%3'/>").arg(file).arg(picSize.width()).arg(picSize.height()));
	}
}

void VCardAvatar::addDeleteButton(QToolButton *button)
{
	buttonLayout->insertWidget(0, button);
	button->setVisible(true);
}

void VCardAvatar::enterEvent(QEvent *event)
{
	if (m_mode && !isEmpty)
		emit mouseOver();
}

void VCardAvatar::leaveEvent(QEvent *event)
{
	if (m_mode && !isEmpty)
		emit mouseOut();
}

QSize VCardAvatar::getPictureSize(const QString &file)
{
	QPixmap pic;
	QSize size;
	pic.load(file);
	size.setHeight(pic.height());
	size.setWidth(pic.width());
	if ( pic.height() >= pic.width() )
	{
		if (pic.height() > limitHeight)
		{
			size.setWidth(pic.width() / (pic.height() / (float) limitWidth));
			size.setHeight(limitWidth);
		}
	} else {
		if (pic.width() > limitWidth)
		{
			size.setWidth(limitWidth);
			size.setHeight( pic.height() / (pic.width() / (float) limitWidth));
		}
	}
	return size;
}
