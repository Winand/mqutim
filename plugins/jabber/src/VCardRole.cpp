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

#include "VCardRole.h"

VCardRole::VCardRole(bool mode, const QString &type, QWidget *parent) : QLabel(parent)
{
	m_mode = mode;
	m_type = type;
	m_status = "";
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setSizePolicy(sizePolicy);
	if (mode)
	{
		menuStatus = new QMenu();
		if (type == "email")
		{
			actionEmailPersonal = new QAction(jPluginSystem::instance().getIcon("mail_home"), VCardConst::personalMailStatus(), this);
			actionEmailPersonal->setCheckable(true);
			connect(actionEmailPersonal, SIGNAL(triggered()), this, SLOT(changeStatus()));
			menuStatus->addAction(actionEmailPersonal);
			actionEmailWork = new QAction(jPluginSystem::instance().getIcon("mail_work"), VCardConst::workMailStatus(), this);
			actionEmailWork->setCheckable(true);
			connect(actionEmailWork, SIGNAL(triggered()), this, SLOT(changeStatus()));
			menuStatus->addAction(actionEmailWork);
			actionEmailEmpty = new QAction(jPluginSystem::instance().getIcon("mail_unknown"), VCardConst::emptyMailStatus(), this);
			actionEmailEmpty->setCheckable(true);
			connect(actionEmailEmpty, SIGNAL(triggered()), this, SLOT(changeStatus()));
			menuStatus->addAction(actionEmailEmpty);
		}
		else if (type == "phone") {
			actionPhoneHome = new QAction(VCardConst::homePhoneStatus(), this);
			actionPhoneHome->setIcon(jPluginSystem::instance().getIcon("phone_home"));
//			actionPhoneHome->setIconVisibleInMenu(true);
			actionPhoneHome->setCheckable(true);
			connect(actionPhoneHome, SIGNAL(triggered()), this, SLOT(changeStatus()));
			menuStatus->addAction(actionPhoneHome);
			actionPhoneWork = new QAction(VCardConst::workPhoneStatus(), this);
			actionPhoneWork->setIcon(jPluginSystem::instance().getIcon("phone_work"));
//			actionPhoneWork->setIconVisibleInMenu(true);
			actionPhoneWork->setCheckable(true);
			connect(actionPhoneWork, SIGNAL(triggered()), this, SLOT(changeStatus()));
			menuStatus->addAction(actionPhoneWork);
			actionPhoneCell = new QAction(VCardConst::celluarPhoneStatus(), this);
			actionPhoneCell->setIcon(jPluginSystem::instance().getIcon("phone_mobile"));
//			actionPhoneCell->setIconVisibleInMenu(true);
			actionPhoneCell->setCheckable(true);
			connect(actionPhoneCell, SIGNAL(triggered()), this, SLOT(changeStatus()));
			menuStatus->addAction(actionPhoneCell);
			actionPhoneEmpty = new QAction(VCardConst::emptyPhoneStatus(), this);
			actionPhoneEmpty->setIcon(jPluginSystem::instance().getIcon("phone_unknown"));
//			actionPhoneEmpty->setIconVisibleInMenu(true);
			actionPhoneEmpty->setCheckable(true);
			connect(actionPhoneEmpty, SIGNAL(triggered()), this, SLOT(changeStatus()));
			menuStatus->addAction(actionPhoneEmpty);
		}
	}
}

VCardRole::~VCardRole()
{
}

void VCardRole::mousePressEvent(QMouseEvent *event)
{
	if (m_mode)
		menuStatus->popup(event->globalPos());
	return QLabel::mousePressEvent(event);
}

void VCardRole::changeStatus()
{
	actionChecked->setChecked(false);
	QString status = ((QAction*) sender())->text();
	setStatus(status);
}

void VCardRole::setStatus(const QString &status)
{
	QString fileName;
	QString icon;
	if (m_type == "email")
	{
		if (status == VCardConst::personalMailStatus())
		{
			icon = "mail_home";
			actionChecked = actionEmailPersonal;
		}
		else if (status == VCardConst::workMailStatus())
		{
			icon = "mail_work";
			actionChecked = actionEmailWork;
		}
		else if (status == VCardConst::emptyMailStatus())
		{
			icon = "mail_unknown";
			actionChecked = actionEmailEmpty;
		}
	}
	else if (m_type == "phone")
	{
		if (status == VCardConst::homePhoneStatus())
		{
			icon = "phone_home";
			actionChecked = actionPhoneHome;
		}
		else if (status == VCardConst::workPhoneStatus())
		{
			icon = "phone_work";
			actionChecked = actionPhoneWork;
		}
		else if (status == VCardConst::celluarPhoneStatus())
		{
			icon = "phone_mobile";
			actionChecked = actionPhoneCell;
		}
		else if (status == VCardConst::emptyPhoneStatus())
		{
			icon = "phone_unknown";
			actionChecked = actionPhoneEmpty;
		}
	}
	if (m_mode)
		actionChecked->setChecked(true);
	fileName = "<img src='"+jPluginSystem::instance().getIconFileName(icon)+"'/>";
	setText(fileName);
	m_status = status;
}

QString VCardRole::status()
{
	return m_status;
}
