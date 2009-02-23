/*
    ActivityDialog

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


#include "activitydialog.h"
#include "jPluginSystem.h"

ActivityDialog::ActivityDialog(const QString &account_name, const QString &profile_name, QWidget *parent)
	: QDialog(parent)
	, m_account_name(account_name)
	, m_profile_name(profile_name)
{
	ui.setupUi(this);
	setFixedSize(size());
	setAttribute(Qt::WA_QuitOnClose, false);
	connect(ui.specificList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
		ui.chooseButton, SIGNAL(clicked()));
	connect(ui.generalList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
		ui.chooseButton, SIGNAL(clicked()));
	ui.specificList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.generalList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

ActivityDialog::~ActivityDialog()
{
	ui.specificList->clear();
	ui.generalList->clear();
}

void ActivityDialog::setActivity(const QString &general, const QString &specific)
{
	ui.textEdit->clear();
	ui.captionEdit->clear();

	QStringList list = jPluginSystem::instance().getGeneralActivityTr().keys();
	list.removeAll("unknown");
	qSort(list);
	QListWidgetItem *none = new QListWidgetItem(ui.generalList);
	none->setIcon(jPluginSystem::instance().getIcon("icq_xstatus"));
	none->setData(Qt::UserRole+1,"");
	foreach(const QString &general_name, list)
	{
		QListWidgetItem *tmp= new QListWidgetItem(ui.generalList);
		QIcon icon = getIcon(general_name);
		/*if(icon.actualSize(QSize(16,16)).width()<0)
			icon = getIcon("unknown");*/
		tmp->setIcon(icon);
//		tmp->setToolTip(jPluginSystem::instance().getGeneralActivityTr().value(general_name));
		tmp->setData(Qt::UserRole+1,general_name);
		if(general_name==general)
		{
			tmp->setSelected(true);
			onGeneralListCurrentItemChanged(tmp, 0, specific);
		}
	}
}

void ActivityDialog::onGeneralListCurrentItemChanged ( QListWidgetItem * current, QListWidgetItem *, const QString &specific )
{
	if(current)
		m_general = current->data(Qt::UserRole+1).toString();
	ui.specificList->clear();

//	if ( !current || current->toolTip().isEmpty() )
//	{
//		ui.captionEdit->clear();
//		ui.textEdit->clear();
//		ui.textEdit->setEnabled(false);
//	}
//	else
//	{
		QString general_name = current->data(Qt::UserRole+1).toString();
		ui.textEdit->setEnabled(true);
		QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");

//		ui.captionEdit->setText(current->toolTip());//jPluginSystem::instance().getGeneralActivityTr().value(current->data(Qt::UserRole+1).toString()));
		ui.textEdit->setText(settings.value("activity/"+current->data(Qt::UserRole+1).toString()+"/text", "").toString());

		QStringList list = jPluginSystem::instance().getSpecificActivityTr(general_name).keys();
		list.removeAll("undefined");
		qSort(list);
		QListWidgetItem *none = new QListWidgetItem(ui.specificList);
		none->setIcon(getIcon(current->data(Qt::UserRole+1).toString()));
//		none->setToolTip(current->toolTip());
		none->setData(Qt::UserRole+1,general_name);
		none->setData(Qt::UserRole+2,"");
		bool selected = false;
		foreach(const QString &specific_name, list)
		{
			QListWidgetItem *tmp= new QListWidgetItem(ui.specificList);
			QIcon icon = getIcon(general_name,specific_name);
			if(icon.actualSize(QSize(16,16)).width()<0)
				icon = getIcon(general_name);
			tmp->setIcon(icon);
//			tmp->setToolTip(current->toolTip()+" ("+jPluginSystem::instance().getSpecificActivityTr(general_name).value(specific_name)+")");
			tmp->setData(Qt::UserRole+1,general_name);
			tmp->setData(Qt::UserRole+2,specific_name);
			if(specific_name==specific)
			{
				tmp->setSelected(true);
				on_specificList_currentItemChanged(tmp, 0);
				/*

				QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
				ui.captionEdit->setText(tmp->toolTip());
				m_text = settings.value("activity/"+general_name+(specific.isEmpty()?"":("/"+specific))+"/text", "").toString();
				ui.textEdit->setText(status_message);*/
				selected = true;
			}
		}
		if(!selected)
		{
			none->setSelected(true);
			on_specificList_currentItemChanged(none, 0);
		}
//	}
}

void ActivityDialog::on_specificList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * )
{
	if(current)
		m_specific = current->data(Qt::UserRole+2).toString();
//	if ( !current || current->toolTip().isEmpty() )
//	{
//		ui.captionEdit->clear();
//		ui.textEdit->clear();
//		ui.textEdit->setEnabled(false);
//	}
//	else
//	{
		QString general_name = current->data(Qt::UserRole+1).toString();
		QString specific_name = current->data(Qt::UserRole+2).toString();
		ui.textEdit->setEnabled(true);
		QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
		m_text = settings.value("activity/"+general_name+(specific_name.isEmpty()?"":("/"+specific_name))+"/text", "").toString();
//		ui.captionEdit->setText(current->toolTip());
		ui.textEdit->setText(m_text);
//	}
}

void ActivityDialog::on_chooseButton_clicked()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
	int row = ui.specificList->currentRow();
	if(row<0)
	{
            m_general = "";
            m_specific = "";
            m_text = "";
	}
        else
        {
            m_general = ui.specificList->item(row)->data(Qt::UserRole+1).toString();
            m_specific = ui.specificList->item(row)->data(Qt::UserRole+2).toString();
            m_text = ui.textEdit->toPlainText();
            settings.setValue("activity/"+m_general+(m_specific.isEmpty()?"":("/"+m_specific))+"/text", m_text);
        }
        settings.setValue("activity/general", m_general);
        settings.setValue("activity/specific", m_specific);
        accept();
}

QIcon ActivityDialog::getIcon(const QString &general, const QString &specific)
{
	QIcon icon = jPluginSystem::instance().getIcon("activity/"+general+"/"+(specific.isEmpty()?"category":specific));
	if(icon.actualSize(QSize(16,16)).width()<0)
		icon = QIcon("activity/"+general+"/"+(specific.isEmpty()?"category":specific));
	return icon;
}
