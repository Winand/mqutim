/*
    CustomStatusDialog

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>
    						Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/


#include "customstatusdialog.h"
#include "jPluginSystem.h"

CustomStatusDialog::CustomStatusDialog(const QString &u, const QString &profile_name, QWidget *parent)
	: QDialog(parent)
	, mineUin(u)
	, m_profile_name(profile_name)
{
	ui.setupUi(this);
	setFixedSize(size());
	setAttribute(Qt::WA_QuitOnClose, false);
	connect(ui.iconList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
		ui.chooseButton, SIGNAL(clicked()));
	ui.iconList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

CustomStatusDialog::~CustomStatusDialog()
{
	qDeleteAll(itemList);
}

void CustomStatusDialog::setStatuses(const QString &current)
{
	current_mood = current;
	setCaption("");
	setMessage("");

	QStringList list = jPluginSystem::instance().getMoodTr().keys();
	list.removeAll("undefined");
	qSort(list);
	QListWidgetItem *none = new QListWidgetItem(ui.iconList);
	none->setIcon(jPluginSystem::instance().getIcon("icq_xstatus"));
	none->setData(Qt::UserRole+1,"");
	foreach(const QString &mood_name, list)
	{
		QListWidgetItem *tmp= new QListWidgetItem(ui.iconList);
		QIcon icon = getIcon(mood_name);
		if(icon.actualSize(QSize(16,16)).width()<0)
			icon = getIcon("unknown");
		tmp->setIcon(icon);
//		tmp->setToolTip(jPluginSystem::instance().getMoodTr().value(mood_name));
		tmp->setData(Qt::UserRole+1,mood_name);
		if(mood_name==current)
		{
			tmp->setSelected(true);
			QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+mineUin, "accountsettings");
//			setCaption(tmp->toolTip());
			status_message = settings.value("moods/"+current+"/text", "").toString();
			setMessage(status_message);
		}
	}
}

void CustomStatusDialog::on_iconList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * /*previous*/ )
{
//	if ( current->toolTip().isEmpty() )
//	{
//		ui.captionEdit->clear();
//		ui.awayEdit->clear();
//		ui.awayEdit->setEnabled(false);
//	}
//	else
//	{
		ui.awayEdit->setEnabled(true);
		QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+mineUin, "accountsettings");

		setCaption(jPluginSystem::instance().getMoodTr().value(current->data(Qt::UserRole+1).toString()));
		setMessage(settings.value("moods/"+current->data(Qt::UserRole+1).toString()+"/text", "").toString());
//	}
}

void CustomStatusDialog::on_chooseButton_clicked()
{
	int row = ui.iconList->currentRow();
	if(row<0)
	{
		reject();
		return;
	}
	current_mood = ui.iconList->item(row)->data(Qt::UserRole+1).toString();
	status_message = ui.awayEdit->toPlainText();

	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+mineUin, "accountsettings");
	settings.setValue("moods/current", current_mood);
	settings.setValue("moods/"+current_mood+"/text", status_message);
	accept();
}

QIcon CustomStatusDialog::getIcon(const QString &name)
{
	QIcon icon = jPluginSystem::instance().getIcon("moods/"+name);
	if(icon.actualSize(QSize(16,16)).width()<0)
		icon = QIcon(":/icons/moods/"+name);
	return icon;
}
