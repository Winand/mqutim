/*
    IcqLayer

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

#include "icqlayer.h"
#include "addaccountform.h"
#include "icqaccount.h"
#include "contactlist.h"
#include "plugineventeater.h"
#include <QString>
#include <QIcon>
#include <QSettings>
#include <QDir>
#include <QFileInfo>

bool IcqLayer::init(PluginSystemInterface *plugin_system)
{
	ProtocolInterface::init(plugin_system);
	m_plugin_system = plugin_system;
	m_login_widget = 0;
	m_general_icq_item = 0;
	m_general_icq_settings = 0;
	m_network_item = 0;
	m_network_settings = 0;
	m_status_item = 0;
	m_status_settings = 0;
	m_contact_settings = 0;
	m_contact_item = 0;
	m_protocol_icon = QIcon(":/icons/icq_protocol.png");
	IcqPluginSystem::instance().setIcqLayer(this);
        qsrand(QDateTime::currentDateTime().toTime_t());
	return true;
}

void IcqLayer::release()
{
	delete m_login_widget;
	removeProtocolSettings();
	foreach(icqAccount *account, m_icq_list)
	{
		account->getProtocol()->getContactListClass()->appExiting();
		account->saveAccountSettings();
		killAccount(account->getIcquin(), false);
	}
}
QString IcqLayer::name()
{
	return "ICQ";
}

QString IcqLayer::description()
{
	return "ICQ protocol realization by Rustam Chakin";
}

QIcon *IcqLayer::icon()
{
	return &m_protocol_icon;
}

void IcqLayer::setProfileName(const QString &profile_name)
{
	m_profile_name = profile_name;
}

//IcqLayer &IcqLayer::instance()
//{
//	static IcqLayer il;
//	return il;
//}

void IcqLayer::removeAccount(const QString &account_name)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	QStringList accounts = settings.value("accounts/list").toStringList();
	accounts.removeAll(account_name);
	accounts.sort();
	settings.setValue("accounts/list", accounts);
	killAccount(account_name, true);
        QSettings dir_settings_path(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
	QDir account_dir(dir_settings_path.fileName());
	account_dir.cdUp();

	//delete profile directory
	if( account_dir.exists() )
		removeProfileDir(account_dir.path());
}

QWidget *IcqLayer::loginWidget()
{
	if ( !m_login_widget )
		m_login_widget = new AddAccountForm;
	return m_login_widget;
}

void IcqLayer::removeLoginWidget()
{
	delete m_login_widget;
	m_login_widget = 0;
}

void IcqLayer::applySettingsPressed()
{
	if ( m_general_icq_settings )
	{
		m_general_icq_settings->saveSettings();
	}
	if ( m_network_settings )
	{
		m_network_settings->saveSettings();
	}
	if ( m_status_settings )
	{
		m_status_settings->saveSettings();
	}
	if ( m_contact_settings )
	{
		m_contact_settings->saveSettings();
	}
}

QList<AccountStructure> IcqLayer::getAccountList()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	QList<AccountStructure> accounts_list;
	QStringList accounts = settings.value("accounts/list").toStringList();
	foreach(const QString &account_name, accounts)
	{
		AccountStructure info_account;
		info_account.protocol_icon = QIcon("icons/icq_protocol.png");
		info_account.protocol_name = "ICQ";
		info_account.account_name = account_name;
		accounts_list.append(info_account);
	}
	return accounts_list;
}

QList<QMenu*> IcqLayer::getAccountStatusMenu()
{
	QList<QMenu*> accounts_menu_list;
	foreach(icqAccount *account, m_icq_list)
	{
		accounts_menu_list.append(account->getAccountMenu());
	}
	return accounts_menu_list;
}

void IcqLayer::addAccountButtonsToLayout(QHBoxLayout *account_button_layout)
{
	m_account_buttons_layout = account_button_layout;
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	QStringList accountList = settings.value("accounts/list").toStringList();
	foreach(const QString &account_from_list, accountList)
	{
		addAccount(account_from_list);
	}
}

void IcqLayer::addAccount(const QString &account_name)
{
	icqAccount *account = new icqAccount(account_name, m_profile_name);
	account->createAccountButton(m_account_buttons_layout);
	m_icq_list.insert(account_name, account);
	account->autoconnecting();
	PluginEventEater::instance().setAccountList(m_icq_list);
}

void IcqLayer::saveLoginDataFromLoginWidget()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	QStringList accounts = settings.value("accounts/list").toStringList();
	QString account_name = m_login_widget->getName();
	QString account_password = m_login_widget->getPass();
	if( !accounts.contains(account_name) )
	{
		accounts<<account_name;
		accounts.sort();
		settings.setValue("accounts/list", accounts);
                QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
		account_settings.setValue("main/name",account_name);

		//simple password encryption

		account_password.truncate(8);
		QByteArray roasted_pass;
		for ( int i = 0; i < account_password.length(); i++ )
			roasted_pass[i] = account_password.at(i).unicode() ^ crypter[i];

		account_settings.setValue("main/password",roasted_pass);
		account_settings.setValue("main/savepass", m_login_widget->getSavePass());
		addAccount(account_name);
	}
}

QList<SettingsStructure> IcqLayer::getSettingsList()
{
	QList<SettingsStructure> settings_list;
	if ( !m_general_icq_item )
	{
		m_general_icq_item = new QTreeWidgetItem;
		m_general_icq_item->setText(0, QObject::tr("ICQ General"));
		m_general_icq_item->setIcon(0, m_plugin_system->getIcon("additional"));
	}
	if ( !m_general_icq_settings )
	{
		m_general_icq_settings = new icqSettings(m_profile_name);
	}
	SettingsStructure tmp_struc;
	tmp_struc.settings_item = m_general_icq_item;
	tmp_struc.settings_widget = m_general_icq_settings;
	settings_list.append(tmp_struc);

	if ( !m_network_item )
	{
		m_network_item = new QTreeWidgetItem;
		m_network_item->setText(0, QObject::tr("Network"));
		m_network_item->setIcon(0, m_plugin_system->getIcon("network"));
	}
	if ( !m_network_settings )
	{
		m_network_settings = new networkSettings(m_profile_name);
	}
	tmp_struc.settings_item = m_network_item;
	tmp_struc.settings_widget = m_network_settings;
	settings_list.append(tmp_struc);

	if ( !m_status_item )
	{
		m_status_item = new QTreeWidgetItem;
		m_status_item->setText(0, QObject::tr("Statuses"));
		m_status_item->setIcon(0, m_plugin_system->getIcon("statuses"));
	}
	if ( !m_status_settings )
	{
		m_status_settings = new statusSettings(m_profile_name);
	}
	tmp_struc.settings_item = m_status_item;
	tmp_struc.settings_widget = m_status_settings;
	settings_list.append(tmp_struc);

	if ( !m_contact_item )
	{
		m_contact_item = new QTreeWidgetItem;
		m_contact_item->setText(0, QObject::tr("Contacts"));
		m_contact_item->setIcon(0, m_plugin_system->getIcon("contact_sett"));
	}
	if ( !m_contact_settings )
	{
		m_contact_settings = new ContactSettings(m_profile_name);
	}
	tmp_struc.settings_item = m_contact_item;
	tmp_struc.settings_widget = m_contact_settings;
	settings_list.append(tmp_struc);

	foreach( icqAccount *account, m_icq_list)
	{
		QObject::connect(m_general_icq_settings, SIGNAL(settingsSaved()),
				account, SLOT(generalSettingsChanged()));
		QObject::connect(m_general_icq_settings, SIGNAL(settingsSaved()),
				account->getProtocol()->getContactListClass(), SLOT(msgSettingsChanged()));
		QObject::connect(m_network_settings, SIGNAL(settingsSaved()),
				account, SLOT(networkSettingsChanged()));
		QObject::connect(m_status_settings, SIGNAL(settingsSaved()),
				account->getProtocol()->getContactListClass(), SLOT(statusSettingsChanged()));
		QObject::connect(m_contact_settings, SIGNAL(settingsSaved()),
				account->getProtocol()->getContactListClass(), SLOT(contactSettingsChanged()));
	}
	return settings_list;
}

void IcqLayer::removeProtocolSettings()
{
	delete m_general_icq_item;
	delete m_general_icq_settings;
	m_general_icq_item = 0;
	m_general_icq_settings = 0;
	delete m_network_item;
	delete m_network_settings;
	m_network_item = 0;
	m_network_settings = 0;
	delete m_status_item;
	delete m_status_settings;
	m_status_item = 0;
	m_status_settings = 0;
	delete m_contact_item;
	delete m_contact_settings;
	m_contact_item = 0;
	m_contact_settings = 0;
}

void IcqLayer::removeProfileDir(const QString &path)
{
	//recursively delete all files in directory
	QFileInfo fileInfo(path);
	if( fileInfo.isDir() )
	{
		QDir dir( path );
		QFileInfoList fileList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
		for (int i = 0; i < fileList.count(); i++)
			removeProfileDir(fileList.at(i).absoluteFilePath());
		dir.rmdir(path);
	} else {
		QFile::remove(path);
	}
}

void IcqLayer::killAccount(const QString &account_name, bool deleting_account)
{

	icqAccount *delete_account = m_icq_list.value(account_name);
	if ( deleting_account )
	{
		delete_account->deleteingAccount = deleting_account;
		delete_account->removeContactList();
		m_icq_list.remove(account_name);
		delete delete_account;
	}

}

QList<AccountStructure> IcqLayer::getAccountStatuses()
{
	//QList<AccountStructure> accounts_list;
	m_status_list.clear();
	foreach(icqAccount *account, m_icq_list)
	{
		if ( account )
		{
			AccountStructure info_account;
			info_account.protocol_icon = account->getCurrentIcon();
			info_account.protocol_name = "ICQ";
			info_account.account_name = account->getIcquin();
			//accounts_list.append(info_account);
			m_status_list.append(info_account);
		}

	}
	return m_status_list;
	//return accounts_list;
}

void IcqLayer::setAutoAway()
{
	foreach( icqAccount *account, m_icq_list)
	{
		account->getProtocol()->setAutoAway();
	}
}

void IcqLayer::setStatusAfterAutoAway()
{
	foreach( icqAccount *account, m_icq_list)
	{
		account->getProtocol()->setStatusAfterAutoAway();
	}
}

void IcqLayer::itemActivated(const QString &account_name, const QString &contact_name)
{
	if ( m_icq_list.contains(account_name) )
	{
		m_icq_list.value(account_name)->getProtocol()->getContactListClass()->itemActivated(contact_name);
	}
}

void IcqLayer::itemContextMenu(const QList<QAction*> &action_list, const QString &account_name, const QString &contact_name, int item_type,
		const QPoint &menu_point)
{
	if ( m_icq_list.contains(account_name) )
	{
		m_icq_list.value(account_name)->getProtocol()->getContactListClass()->showItemContextMenu(action_list, contact_name, item_type, menu_point);
	}
}

void IcqLayer::sendMessageTo(const QString &account_name, const QString &contact_name, int item_type, const QString& message, int message_icon_position)
{
	if ( m_icq_list.contains(account_name) )
	{
		m_icq_list.value(account_name)->getProtocol()->getContactListClass()->sendMessageTo(contact_name, message, message_icon_position);
	}
}

QStringList IcqLayer::getAdditionalInfoAboutContact(const QString &account_name, const QString &item_name, int item_type ) const
{
	if ( m_icq_list.contains(account_name) )
	{
		return m_icq_list.value(account_name)->getProtocol()->getContactListClass()->getAdditionalInfoAboutContact(item_name, item_type);
	}
	return QStringList();
}

void IcqLayer::showContactInformation(const QString &account_name, const QString &item_name, int item_type )
{
	if ( m_icq_list.contains(account_name) )
	{
		return m_icq_list.value(account_name)->getProtocol()->getContactListClass()->openInfoWindow(item_name);
	}
}

/*void IcqLayer::sendImageTo(const QString &account_name, const QString &item_name, int item_type, const QByteArray &image_raw )
{
	if ( m_icq_list.contains(account_name) )
	{
		return m_icq_list.value(account_name)->getProtocol()->getContactListClass()->sendImageTo(item_name, image_raw);
	}
}*/

/*void IcqLayer::sendFileTo(const QString &account_name, const QString &item_name, int item_type, const QStringList &file_names)
{
	if ( m_icq_list.contains(account_name) )
	{
                m_icq_list.value(account_name)->getProtocol()->getContactListClass()->sendFileTo(item_name, file_names);
	}
}*/

void IcqLayer::sendTypingNotification(const QString &account_name, const QString &item_name, int item_type, int notification_type)
{
	if ( m_icq_list.contains(account_name) )
	{
		m_icq_list.value(account_name)->getProtocol()->getContactListClass()->sendTypingNotifications(item_name, notification_type);
	}
}

void IcqLayer::moveItemSignalFromCL(const TreeModelItem &old_item, const TreeModelItem &new_item)
{
	if ( m_icq_list.contains(old_item.m_account_name) )
	{
		m_icq_list.value(old_item.m_account_name)->getProtocol()
		->getContactListClass()->moveItemSignalFromCL(old_item, new_item);
	}
}

void IcqLayer::deleteItemSignalFromCL(const QString &account_name, const QString &item_name, int type)
{
	if ( m_icq_list.contains(account_name) )
	{
		m_icq_list.value(account_name)->getProtocol()->getContactListClass()->deleteItemSignalFromCL(item_name, type);
	}
}

QString IcqLayer::getItemToolTip(const QString &account_name, const QString &contact_name)
{
	if ( m_icq_list.contains(account_name) )
	{
		return m_icq_list.value(account_name)->getProtocol()->getContactListClass()->getItemToolTip(contact_name);
	}
	return contact_name;
}

void IcqLayer::chatWindowOpened(const QString &account_name, const QString &item_name)
{
	if ( m_icq_list.contains(account_name) )
	{
		m_icq_list.value(account_name)->getProtocol()->getContactListClass()->chatWindowOpened(item_name, true);
	}
}

void IcqLayer::getMessageFromPlugins(const QList<void *> &event)
{
	PluginEventEater::instance().getEvent(event);
}



Q_EXPORT_PLUGIN2(icqlayer, IcqLayer);
