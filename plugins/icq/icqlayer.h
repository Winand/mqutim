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

#ifndef ICQLAYER_H_
#define ICQLAYER_H_

#include <qutim/protocolinterface.h>
#include "settings/icqsettings.h"
#include "settings/networksettings.h"
#include "settings/statussettings.h"
#include "settings/contactsettings.h"


using namespace qutim_sdk_0_2;
class QString;
class AddAccountForm;
class icqAccount;
class QHBoxLayout;
#include <QAction>
const char crypter[] = {0x10,0x67, 0x56, 0x78, 0x85, 0x14, 0x87, 0x11, 0x45,0x45,0x45,0x45,0x45,0x45 };

class IcqLayer : public QObject, ProtocolInterface
{
	Q_OBJECT
	Q_INTERFACES(qutim_sdk_0_2::PluginInterface)
public:
	virtual bool init(PluginSystemInterface *plugin_system);
	virtual void release();
	virtual void processEvent(PluginEvent & /*event*/) {};
	virtual QWidget *settingsWidget(){return 0;}
	virtual QString name();
	virtual QString description();
	virtual QIcon *icon();
	virtual void setProfileName(const QString &);


	virtual void removeAccount(const QString &account_name);
	virtual QWidget *loginWidget();
	virtual void removeLoginWidget();
	//will be emited if button 'OK' or 'Apply' from settings dialog was pressed.
	virtual void applySettingsPressed();

	//function for getting protocol statuses menu from all accounts
	virtual QList<QMenu*> getAccountStatusMenu();

	//protocol plugin must add account button with status menu and etc to this layout.
	virtual void addAccountButtonsToLayout(QHBoxLayout *);

	//function for saving all login data from login widget.
	virtual void saveLoginDataFromLoginWidget();

	//function return protocol settings widgets and some additional data
	virtual QList<SettingsStructure> getSettingsList();

	//function for deleting protocol settings from memory
	virtual void removeProtocolSettings();

	//function return account list
	virtual QList<AccountStructure> getAccountList();
	
	virtual QList<AccountStructure> getAccountStatuses();
	
	//Function notify about auto-away
	virtual void setAutoAway();
	//Function notify about changing status from auto-away
	virtual void setStatusAfterAutoAway();
	
	virtual void itemActivated(const QString &account_name, const QString &contact_name);
	virtual void itemContextMenu(const QList<QAction*> &action_list,
				const QString &account_name, const QString &contact_name, int item_type, const QPoint &menu_point);
	PluginSystemInterface *getMainPluginSystemPointer() {return m_plugin_system; }
	virtual void sendMessageTo(const QString &account_name, const QString &contact_name, int item_type, const QString& message, int message_icon_position);
	virtual QStringList getAdditionalInfoAboutContact(const QString &account_name, const QString &item_name, int item_type ) const;
	virtual void showContactInformation(const QString &account_name, const QString &item_name, int item_type );
/*	virtual void sendImageTo(const QString &account_name, const QString &item_name, int item_type, const QByteArray &image_raw );*/
/*        virtual void sendFileTo(const QString &account_name, const QString &item_name, int item_type, const QStringList &file_names);*/
	virtual void sendTypingNotification(const QString &account_name, const QString &item_name, int item_type, int notification_type);
	virtual void moveItemSignalFromCL(const TreeModelItem &old_item, const TreeModelItem &new_item);
	
	virtual QString getItemToolTip(const QString &account_name, const QString &contact_name);
	
	virtual void deleteItemSignalFromCL(const QString &account_name, const QString &item_name, int type);
	virtual void chatWindowOpened(const QString &account_name, const QString &item_name);
	virtual void chatWindowAboutToBeOpened(const QString &account_name, const QString &item_name) {}
	virtual void chatWindowClosed(const QString &account_name, const QString &item_name) {}
	virtual void sendMessageToConference(const QString&, const QString&, const QString&) {};
	virtual void leaveConference(const QString&, const QString&) {};
	virtual void conferenceItemActivated(const QString & /*conference_name*/, const QString & /*account_name*/, 
			const QString & /*nickname*/)  {}
	virtual void conferenceItemContextMenu(const QList<QAction*> & /*action_list*/, const QString & /*conference_name*/,
				const QString & /*account_name*/, const QString & /*nickname*/, const QPoint & /*menu_point*/) {}
	virtual QString getConferenceItemToolTip(const QString & /*conference_name*/, const QString & /*account_name*/, const QString & /*nickname*/)
	{
		return "conf";
	}
	virtual void showConferenceContactInformation(const QString & /*conference_name*/, const QString & /*account_name*/, const QString & /*nickname*/) {}
	virtual void showConferenceTopicConfig(const QString & /*conference_name*/, const QString & /*account_name*/) {}
	virtual void showConferenceMenu(const QString & /*conference_name*/, const QString & /*account_name*/, 
			const QPoint & /*menu_point*/) {}
	virtual void getMessageFromPlugins(const QList<void *> &event);
	virtual void editAccount(const QString &){}

private: 
	void removeProfileDir(const QString &);
	void addAccount(const QString &);
	void killAccount(const QString &, bool deleting_account);
	QList<AccountStructure> m_status_list;
	PluginSystemInterface *m_plugin_system;
	QIcon m_protocol_icon;
	AddAccountForm *m_login_widget;
	QString m_profile_name;
	QHBoxLayout *m_account_buttons_layout;
	
	QHash<QString, icqAccount *> m_icq_list;
	icqSettings *m_general_icq_settings;
	QTreeWidgetItem *m_general_icq_item;
	networkSettings *m_network_settings;
	QTreeWidgetItem *m_network_item;
	statusSettings *m_status_settings;
	QTreeWidgetItem *m_status_item;
	ContactSettings *m_contact_settings;
	QTreeWidgetItem *m_contact_item;
};

#endif /*ICQLAYER_H_*/
