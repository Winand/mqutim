/*
    jLayer.h

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

#ifndef JLAYER_H
#define JLAYER_H

#include <qutim/protocolinterface.h>

using namespace qutim_sdk_0_2;

#include "jAccount.h"
#include "loginform.h" //!!!!!!!!!!!!!!!!!!

class jAccount;
class QString;
class QHBoxLayout;

class jLayer : public QObject, ProtocolInterface
{
	Q_OBJECT
	Q_INTERFACES(qutim_sdk_0_2::PluginInterface)
public:
	virtual bool init(PluginSystemInterface *plugin_system);
	PluginSystemInterface *getMainPluginSystemPointer();
	virtual void release();
	virtual void processEvent(PluginEvent &event);
	virtual QWidget *settingsWidget();
	virtual QString name();
	virtual QString description();
	virtual QIcon *icon();
	virtual void setProfileName(const QString &profile_name);
	virtual void removeAccount(const QString &account_name);
	virtual void editAccount(const QString &account_name);
	virtual QWidget *loginWidget();
	virtual void removeLoginWidget();
	virtual void applySettingsPressed();
	virtual QList<QMenu*> getAccountStatusMenu();
	virtual void addAccountButtonsToLayout(QHBoxLayout *account_button_layout);
	virtual void saveLoginDataFromLoginWidget();
	virtual QList<SettingsStructure> getSettingsList();
	virtual void removeProtocolSettings();
	virtual QList<AccountStructure> getAccountList();
	virtual QList<AccountStructure> getAccountStatuses();
	virtual void setAutoAway();
	virtual void setStatusAfterAutoAway();
	virtual void itemActivated(const QString &account_name, const QString &contact_name);
	virtual void itemContextMenu(const QList<QAction*> &action_list, const QString &account_name, const QString &contact_name, int item_type, const QPoint &menu_point);
	virtual void sendMessageTo(const QString &account_name, const QString &contact_name, int item_type, const QString& message, int message_icon_position);
	virtual QStringList getAdditionalInfoAboutContact(const QString &account_name, const QString &item_name, int item_type ) const;
	virtual void showContactInformation(const QString &account_name, const QString &item_name, int item_type );
	virtual void sendImageTo(const QString &account_name, const QString &item_name, int item_type, const QByteArray &image_raw );
	virtual void sendFileTo(const QString &account_name, const QString &item_name, int item_type);
	virtual void sendTypingNotification(const QString &account_name, const QString &item_name, int item_type, int notification_type);
	virtual void moveItemSignalFromCL(const TreeModelItem &old_item, const TreeModelItem &new_item);
	virtual QString getItemToolTip(const QString &account_name, const QString &contact_name);
	virtual void deleteItemSignalFromCL(const QString &account_name, const QString &item_name, int type);
	virtual void chatWindowOpened(const QString &account_name, const QString &item_name);
	virtual void chatWindowAboutToBeOpened(const QString &account_name, const QString &item_name);
	virtual void chatWindowClosed(const QString &account_name, const QString &item_name);
	virtual void sendMessageToConference(const QString &conference_name, const QString &account_name, const QString &message);
	virtual void leaveConference(const QString &conference_name, const QString &account_name);
	virtual void conferenceItemActivated(const QString &conference_name, const QString &account_name, const QString &nickname);
	virtual void conferenceItemContextMenu(const QList<QAction*> &action_list, const QString &conference_name,
				const QString &account_name, const QString &nickname, const QPoint &menu_point);
	virtual QString getConferenceItemToolTip(const QString &conference_name, const QString &account_name, const QString &nickname);
	virtual void showConferenceContactInformation(const QString &conference_name, const QString &account_name, const QString &nickname);
	virtual void showConferenceTopicConfig(const QString &conference_name, const QString &account_name);
	virtual void showConferenceMenu(const QString &conference_name, const QString &account_name,const QPoint &menu_point) {}
	virtual void getMessageFromPlugins(const QList<void *> &event);
private:
	void addAccount(const QString &);
	void killAccount(const QString &);
	void removeProfileDir(const QString &);

	QIcon *m_protocol_icon;
	QHash<QString, jAccount *> m_jabber_list;
        QList<QMenu*> m_accounts_menu_list;
	QHBoxLayout *m_account_buttons_layout;
	QString m_profile_name;
	LoginForm *m_login_widget;
	PluginSystemInterface *m_plugin_system;
};

#endif
