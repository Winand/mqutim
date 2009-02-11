/*
    customStatusDialog

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>

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
#include "icqpluginsystem.h"

customStatusDialog::customStatusDialog(const QString &u, const QString &profile_name, QWidget *parent)
	: QDialog(parent)
	, mineUin(u)
	, m_profile_name(profile_name)
{
	ui.setupUi(this);
	setFixedSize(size());
	setAttribute(Qt::WA_QuitOnClose, false);
	connect(ui.iconList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
		ui.chooseButton, SIGNAL(clicked()));
}

customStatusDialog::~customStatusDialog()
{
	qDeleteAll(itemList);
}

void customStatusDialog::setStatuses(int index, const QList<QString> &list)
{
	QListWidgetItem *none = new QListWidgetItem(ui.iconList);
	none->setIcon(IcqPluginSystem::instance().getIcon("icq_xstatus"));
	foreach(QString path, list)
	{
		QListWidgetItem *tmp= new QListWidgetItem(ui.iconList);
		itemList.append(tmp);
		tmp->setIcon(QIcon(path));
                tmp->setWhatsThis(getToolTip(list.indexOf(path)));

	}
	statusIndex = index;

        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+mineUin, "accountsettings");
	QString cap = settings.value("xstatus"+ QString::number(statusIndex - 1) +"/caption", "").toString();
	if ( cap.isEmpty())
                ui.captionEdit->setText(ui.iconList->item(statusIndex)->whatsThis());
	else
		setCaption(cap);
	setMessage(settings.value("xstatus"+ QString::number(statusIndex - 1) +"/message", "").toString());
	

	if ( !index )
	{
		ui.captionEdit->clear();
		ui.awayEdit->clear();
		ui.captionEdit->setEnabled(false);
		ui.awayEdit->setEnabled(false);
	}
	ui.birthBox->setChecked(settings.value("xstatus/birth", false).toBool());
}

QString customStatusDialog::getToolTip(int index) const
{
	switch(index)
	{
	case 0:
		return tr("Angry");
	case 1:
		return tr("Taking a bath");
	case 2:
		return tr("Tired");
	case 3:
		return tr("Party");
	case 4:
		return tr("Drinking beer");
	case 5:
		return tr("Thinking");
	case 6:
		return tr("Eating");
	case 7:
		return tr("Watching TV");
	case 8:
		return tr("Meeting");
	case 9:
		return tr("Coffee");
	case 10:
		return tr("Listening to music");
	case 11:
		return tr("Business");
	case 12:
		return tr("Shooting");
	case 13:
		return tr("Having fun");
	case 14:
		return tr("On the phone");
	case 15:
		return tr("Gaming");
	case 16:
		return tr("Studying");
	case 17:
		return tr("Shopping");
	case 18:
		return tr("Feeling sick");
	case 19:
		return tr("Sleeping");
	case 20:
		return tr("Surfing");
	case 21:
		return tr("Browsing");
	case 22:
		return tr("Working");
	case 23:
		return tr("Typing");
	case 24:
		return tr("Picnic");
	case 28:
		return tr("On WC");
	case 29:
		return tr("To be or not to be");
	case 30:
		return tr("PRO 7");
	case 31:
		return tr("Love");
	case 34:
		return tr("Sex");	
	case 35:
		return tr("Smoking");
	case 36:
		return tr("Cold");
	case 37:
		return tr("Crying");
	case 38:
		return tr("Fear");
	case 39:
		return tr("Reading");
	case 40:
		return tr("Sport");
	case 41:
		return tr("In tansport");
	default:
		return tr("?");
	}
}

void customStatusDialog::on_iconList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * /*previous*/ )
{

	statusIndex = ui.iconList->row(current);
        if ( current->whatsThis().isEmpty() )
	{
		ui.captionEdit->clear();
		ui.awayEdit->clear();
		ui.captionEdit->setEnabled(false);
		ui.awayEdit->setEnabled(false);
	}
	else
	{
//		ui.captionEdit->setText(current->toolTip());
		ui.captionEdit->setEnabled(true);
		ui.awayEdit->setEnabled(true);
                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+mineUin, "accountsettings");
		QString cap = settings.value("xstatus"+ QString::number(statusIndex -1 ) +"/caption", "").toString();

		if ( cap.isEmpty())
                        ui.captionEdit->setText(current->whatsThis());
		else
			setCaption(cap);


		setMessage(settings.value("xstatus"+ QString::number(statusIndex - 1) +"/message", "").toString());


	}




}

void customStatusDialog::on_chooseButton_clicked()
{
	status = statusIndex;

	statusCaption = ui.captionEdit->text();
	statusMessage = ui.awayEdit->toPlainText();

        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+mineUin, "accountsettings");
	settings.setValue("xstatus/index", status);
	settings.setValue("xstatus"+ QString::number(statusIndex - 1) + "/caption", statusCaption);
	settings.setValue("xstatus"+ QString::number(statusIndex - 1) + "/message", statusMessage);

	settings.setValue("xstatus/caption", statusCaption);
	settings.setValue("xstatus/message", statusMessage);
	settings.setValue("xstatus/birth", ui.birthBox->isChecked());
	accept();
}

void customStatusDialog::on_awayEdit_textChanged()
{
	QString xstat = ui.awayEdit->toPlainText();
	if (xstat.length() > 6500){
		ui.chooseButton->setEnabled(false);
	} else {
		ui.chooseButton->setEnabled(true);
	}
}
