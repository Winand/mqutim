/*
    jLayer.cpp

    Copyright (c) 2008 by Nikita Belov <zodiac.nv@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "jLayer.h"
#include "jClientIdentification.h"
#include "gloox/jid.h"
#include "loginform.h"
#include "jAccountSettings.h"
#include <QtopiaApplication>

bool jLayer::init(PluginSystemInterface *plugin_system)
{
	qRegisterMetaType<TreeModelItem>("TreeModelItem");
	qRegisterMetaType<JID>("JID");
	qRegisterMetaType<BookmarkList>("BookmarkList");
        qRegisterMetaType<ConferenceList>("ConferenceList");
        qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
        qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

	ProtocolInterface::init(plugin_system);
	m_login_widget = 0;
	m_protocol_icon = new QIcon(":/icons/protocol.png");
	m_plugin_system = plugin_system;

	jPluginSystem::instance().setJabberLayer(this);

	return true;
}

PluginSystemInterface *jLayer::getMainPluginSystemPointer()
{
	return m_plugin_system;
}

void jLayer::release()
{
//	x3 =)
}

void jLayer::processEvent(PluginEvent &/*event*/)
{
}

QWidget *jLayer::settingsWidget()
{
		return 0;
}

QString jLayer::name()
{
	return "Jabber";
}

QString jLayer::description()
{
	return "Jabber protocol-plugin for qutIM. Based on gloox-lib";
}

QIcon *jLayer::icon()
{
	return m_protocol_icon;
}

void jLayer::setProfileName(const QString &profile_name)
{
	m_profile_name = profile_name;
	jClientIdentification::instance().init(profile_name);
  jPluginSystem::instance().init();
}

void jLayer::removeAccount(const QString &account_name)
{
	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	QStringList accounts = settings.value("accounts/list").toStringList();
	accounts.removeAll(account_name);
	accounts.sort();
	settings.setValue("accounts/list", accounts);
	killAccount(account_name);
	QSettings dir_settings_path(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+account_name, "profilesettings");
	QDir account_dir(dir_settings_path.fileName());
	account_dir.cdUp();

	if(account_dir.exists())
		removeProfileDir(account_dir.path());
}
void jLayer::editAccount(const QString &account_name)
{
  if(!m_jabber_list.contains(account_name))
      return;
  jAccountSettings *acc_settings = new jAccountSettings(m_profile_name, account_name, m_jabber_list.value(account_name));
  QtopiaApplication::showDialog(acc_settings);

	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	QStringList accounts = settings.value("accounts/list").toStringList();
        if ( accounts.contains(account_name) ){
	}
}

QWidget *jLayer::loginWidget()
{
	if (!m_login_widget)
		m_login_widget = new LoginForm;
	return m_login_widget;
}

void jLayer::removeLoginWidget()
{
	delete m_login_widget;
	m_login_widget = 0;
}

void jLayer::applySettingsPressed()
{
//	if (m_general_icq_settings)
//	{
//		m_general_icq_settings->saveSettings();
//	}
}

QList<QMenu*> jLayer::getAccountStatusMenu()
{
        m_accounts_menu_list.clear();
	foreach(jAccount *account, m_jabber_list)
	{
                m_accounts_menu_list.append(account->getAccountMenu());
	}
        return m_accounts_menu_list;
}

void jLayer::addAccountButtonsToLayout(QHBoxLayout *account_button_layout)
{
	m_account_buttons_layout = account_button_layout;
	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	QStringList accountList = settings.value("accounts/list").toStringList();
	foreach(const QString &account_from_list, accountList)
	{
                addAccount(account_from_list.toLower());
	}
}

void jLayer::saveLoginDataFromLoginWidget()
{
	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	QStringList accounts = settings.value("accounts/list").toStringList();
	QString account_jid = m_login_widget->getJid();
	QString account_password = m_login_widget->getPass();
	if(!accounts.contains(account_jid))
	{
		accounts<<account_jid;
		accounts.sort();
		settings.setValue("accounts/list", accounts);
		QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+account_jid, "accountsettings");
		account_settings.setValue("main/jid", account_jid);
		account_settings.setValue("main/password", account_password);
	}
	addAccount(account_jid);
}

QList<SettingsStructure> jLayer::getSettingsList()
{
	QList<SettingsStructure> settings_list;
//	if ( !m_general_icq_item )
//	{
//		m_general_icq_item = new QTreeWidgetItem;
//		m_general_icq_item->setText(0, QObject::tr("ICQ General"));
//		m_general_icq_item->setIcon(0, m_plugin_system->getIcon("additional"));
//	}
//	if ( !m_general_icq_settings )
//	{
//		m_general_icq_settings = new icqSettings(m_profile_name);
//	}
//	SettingsStructure tmp_struc;
//	tmp_struc.settings_item = m_general_icq_item;
//	tmp_struc.settings_widget = m_general_icq_settings;
//	settings_list.append(tmp_struc);

//	foreach(jAccount *account, m_jabber_list)
//	{
//		QObject::connect(m_general_icq_settings, SIGNAL(settingsSaved()),
//				account, SLOT(generalSettingsChanged()));
//	}
	return settings_list;
}

void jLayer::removeProtocolSettings()
{
//	delete m_general_icq_item;
//	delete m_general_icq_settings;
//	m_general_icq_item = 0;
//	m_general_icq_settings = 0;
	delete m_login_widget;
}

QList<AccountStructure> jLayer::getAccountList()
{
	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
	QList<AccountStructure> accounts_list;
	QStringList accounts = settings.value("accounts/list").toStringList();
	foreach(const QString &account_name, accounts)
	{
		AccountStructure info_account;
		info_account.protocol_icon = *m_protocol_icon;
		info_account.protocol_name = "Jabber";
                info_account.account_name = account_name.toLower();
		accounts_list.append(info_account);
	}
	return accounts_list;
}

QList<AccountStructure> jLayer::getAccountStatuses()
{
	QList<AccountStructure> accounts_list;
	foreach(const QString &account_name, m_jabber_list.keys())
	{
		jAccount *jabber_account = m_jabber_list.value(account_name);
		if (jabber_account)
		{
			AccountStructure info_account;
			info_account.protocol_icon = jabber_account->getStatusIcon();
			info_account.protocol_name = "Jabber";
			info_account.account_name = account_name;
			accounts_list.append(info_account);
		}
	}
	return accounts_list;
}

void jLayer::setAutoAway()
{
	foreach(jAccount *account, m_jabber_list)
	{
	//	account->getProtocol()->setAutoAway();
	}
}

void jLayer::setStatusAfterAutoAway()
{
	foreach(jAccount *account, m_jabber_list)
	{
	//	account->getProtocol()->setStatusAfterAutoAway();
	}
}

void jLayer::itemActivated(const QString &account_name, const QString &contact_name)
{
	if (m_jabber_list.contains(account_name))
	{
	//	m_jabber_list.value(account_name)->getProtocol()->getContactListClass()->itemActivated(contact_name);
	}
}

void jLayer::itemContextMenu(const QList<QAction*> &action_list, const QString &account_name, const QString &contact_name, int item_type, const QPoint &menu_point)
{
	if (m_jabber_list.contains(account_name))
	{
		m_jabber_list.value(account_name)->getJabberRoster()->itemContextMenu(action_list,contact_name,item_type,menu_point);
	//	m_jabber_list.value(account_name)->getProtocol()->getContactListClass()->showItemContextMenu(action_list, contact_name, item_type, menu_point);
	}
}

void jLayer::sendMessageTo(const QString &account_name, const QString &contact_name, int item_type, const QString& message, int message_icon_position)
{
	if (m_jabber_list.contains(account_name))
	{
		m_jabber_list.value(account_name)->getProtocol()->sendMessageTo(contact_name,message,message_icon_position);
	}
}

QStringList jLayer::getAdditionalInfoAboutContact(const QString &account_name, const QString &item_name, int item_type ) const
{
	if ( m_jabber_list.contains(account_name) )
	{
		QStringList list = m_jabber_list.value(account_name)->getProtocol()->getAdditionalInfoAboutContact(item_name);
		if(list.size()==0)
			list.append(item_name);
		if(account_name==item_name)
			list[0] = m_jabber_list.value(account_name)->getProtocol()->getAccountName();
		else if(list[0].isEmpty())
			list[0] = item_name;
		return list;
	}
	QStringList list;
	list.append(item_name);
	return list;
}

void jLayer::showContactInformation(const QString &account_name, const QString &item_name, int item_type )
{
	if ( m_jabber_list.contains(account_name) )
 	{
		//return m_jabber_list.value(account_name)->getProtocol()->getContactListClass()->openInfoWindow(item_name);
		m_jabber_list.value(account_name)->showVCardWidget(item_name);
 	}
}

void jLayer::sendImageTo(const QString &account_name, const QString &item_name, int item_type, const QByteArray &image_raw )
{
	if ( m_jabber_list.contains(account_name) )
	{
//		return m_jabber_list.value(account_name)->getProtocol()->getContactListClass()->sendImageTo(item_name, image_raw);
	}
}

void jLayer::sendFileTo(const QString &account_name, const QString &item_name, int item_type)
{
	if ( m_jabber_list.contains(account_name) )
	{
//		m_jabber_list.value(account_name)->getProtocol()->getContactListClass()->sendFileTo(item_name);
	}
}

void jLayer::sendTypingNotification(const QString &account_name, const QString &item_name, int item_type, int notification_type)
{
	if ( m_jabber_list.contains(account_name) && item_type==0 )
	{
		m_jabber_list.value(account_name)->getProtocol()->sendTypingNotification(item_name, notification_type);
	}
}

void jLayer::moveItemSignalFromCL(const TreeModelItem &old_item, const TreeModelItem &new_item)
{
	if(new_item.m_item_type!=0)
		return;
	if(new_item.m_item_name!=old_item.m_item_name)
		return;
	if ( m_jabber_list.contains(old_item.m_account_name) )
		m_jabber_list.value(old_item.m_account_name)->getProtocol()->moveContact(new_item.m_item_name, new_item.m_parent_name);
}

QString jLayer::getItemToolTip(const QString &account_name, const QString &contact_name)
{
	if ( m_jabber_list.contains(account_name) )
	{
		return m_jabber_list.value(account_name)->getJabberRoster()->getToolTip(contact_name);
	}
	return "";
}

void jLayer::deleteItemSignalFromCL(const QString &account_name, const QString &item_name, int type)
{
	if ( m_jabber_list.contains(account_name) )
	{
//		m_jabber_list.value(account_name)->getProtocol()->getContactListClass()->deleteItemSignalFromCL(item_name, type);
	}
}

void jLayer::chatWindowOpened(const QString &account_name, const QString &item_name)
{
	if ( m_jabber_list.contains(account_name) )
	{
		m_jabber_list.value(account_name)->getJabberRoster()->chatWindowOpened(item_name);
	}
}

void jLayer::chatWindowAboutToBeOpened(const QString &account_name, const QString &item_name)
{
	if ( m_jabber_list.contains(account_name) )
	{
		m_jabber_list.value(account_name)->getJabberRoster()->chatWindowAboutToBeOpened(item_name);
	}
}

void jLayer::chatWindowClosed(const QString &account_name, const QString &item_name)
{
	if ( m_jabber_list.contains(account_name) )
	{
		m_jabber_list.value(account_name)->getJabberRoster()->chatWindowClosed(item_name);
	}
}

void jLayer::sendMessageToConference(const QString &conference_name, const QString &account_name, const QString &message)
{
	if ( m_jabber_list.contains(account_name) )
	{
		m_jabber_list.value(account_name)->getConferenceManagementObject()->sendMessageToConference(conference_name, message);
	}
}

void jLayer::leaveConference(const QString &conference_name, const QString &account_name)
{
	if ( m_jabber_list.contains(account_name) )
	{
		m_jabber_list.value(account_name)->getConferenceManagementObject()->leaveConference(conference_name);
	}
}

void jLayer::addAccount(const QString &account_name)
{
	jAccount *account = new jAccount(account_name, m_profile_name);
	account->createAccountButton(m_account_buttons_layout);
	m_jabber_list.insert(account_name, account);
	QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+account_name, "accountsettings");
	account_settings.beginGroup("main");
	if(account_settings.value("autoconnect",false).toBool()){
		account->setStatusN(Presence::Available);
	    }
}

void jLayer::killAccount(const QString &account_name)
{
	jAccount *delete_account = m_jabber_list.value(account_name);
	m_jabber_list.remove(account_name);
	delete delete_account;
}

void jLayer::removeProfileDir(const QString &path)
{
	QFileInfo fileInfo(path);
	if(fileInfo.isDir())
	{
		QDir dir(path);
		QFileInfoList fileList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
		for (int i = 0; i < fileList.count(); i++)
			removeProfileDir(fileList.at(i).absoluteFilePath());
		dir.rmdir(path);
	}
	else
		QFile::remove(path);
}

void jLayer::conferenceItemActivated(const QString &conference_name, const QString &account_name, const QString &nickname)
{
	TreeModelItem item;
	item.m_protocol_name = "Jabber";
	item.m_account_name = account_name;
	item.m_item_name = conference_name+"/"+nickname;
	item.m_item_type = TreeModelItem::Buddy;
	m_plugin_system->createChat(item);
}

void jLayer::conferenceItemContextMenu(const QList<QAction*> &action_list, const QString &conference_name,
			const QString &account_name, const QString &nickname, const QPoint &menu_point)
{
	if (m_jabber_list.contains(account_name))
	{
		m_jabber_list.value(account_name)->getConferenceManagementObject()->itemContextMenu(action_list,conference_name,nickname,menu_point);
	}
}

QString jLayer::getConferenceItemToolTip(const QString &conference_name, const QString &account_name, const QString &nickname)
{
	if ( m_jabber_list.contains(account_name) )
	{
		return m_jabber_list.value(account_name)->getConferenceManagementObject()->getToolTip(conference_name, nickname);
	}
	return conference_name+"/"+nickname;
}
void jLayer::showConferenceContactInformation(const QString &conference_name, const QString &account_name, const QString &nickname)
{
	showContactInformation(account_name, conference_name+"/"+nickname,0);
}

void jLayer::showConferenceTopicConfig(const QString &conference_name, const QString &account_name)
{
	if ( m_jabber_list.contains(account_name) )
	{
		m_jabber_list.value(account_name)->getConferenceManagementObject()->showTopicConfig(conference_name);
	}
}

void jLayer::getMessageFromPlugins(const QList<void *> &event)
{
	if(event.size()<2)
		return;
	const QStringList &accounts = *((QStringList*)event[1]);
	if(accounts.size()==0)
	{
		foreach(jAccount *account, m_jabber_list.values())
			account->getProtocol()->getMessageFromPlugins(event);
	}
	else
	{
		foreach(const QString &account_name, accounts)
			if(m_jabber_list.contains(account_name))
				m_jabber_list.value(account_name)->getProtocol()->getMessageFromPlugins(event);
	}
}

Q_EXPORT_PLUGIN2(jabberprotocol, jLayer);
