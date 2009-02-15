/*
    AbstractNotificationLayer

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

#include "abstractnotificationlayer.h"
#include "../abstractlayer.h"
#include "../pluginsystem.h"
#include "../abstractsoundlayer.h"
#include <QSettings>
#include <QFile>

AbstractNotificationLayer::AbstractNotificationLayer()
{
	m_first_tray_message_is_shown = false;
	m_position_in_stack = 1;
	m_show_message = true;
}

AbstractNotificationLayer::~AbstractNotificationLayer()
{
}

AbstractNotificationLayer &AbstractNotificationLayer::instance()
{
	static AbstractNotificationLayer anl;
	return anl;
}

void AbstractNotificationLayer::loadProfile(const QString &profile_name)
{
	m_profile_name = profile_name;
	loadSettings();
	loadTheme();
}

void AbstractNotificationLayer::loadSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
	settings.beginGroup("notifications");
	m_show_popup = settings.value("popup", true).toBool();
        m_popup_width = settings.value("pwidth", 240).toInt();
        m_popup_height = settings.value("pheight", 60).toInt();
	m_popup_sec = settings.value("ptime", 6).toInt();
	m_popup_position = settings.value("pposition", 3).toInt();
	m_popup_style = settings.value("pstyle", 1).toInt();
	m_show_balloon = settings.value("balloon", false).toBool();
	m_balloon_sec = settings.value("bsecs", 5).toInt();
	m_show_signon = settings.value("signon", true).toBool();
	m_show_signoff = settings.value("signoff", true).toBool();
	m_show_typing = settings.value("typing", true).toBool();
	m_show_change_status = settings.value("statuschange", true).toBool();
	m_show_message = settings.value("message", true).toBool();
	settings.endGroup();
	loadTheme();
}

void AbstractNotificationLayer::systemMessage(const TreeModelItem &item, const QString &message)
{
	if ( m_show_balloon )
	{
		AbstractLayer::instance().showBalloon(QObject::tr("System message for : %1").arg(item.m_account_name),
				message, m_balloon_sec * 1000);
	}
	
	if ( m_show_popup )
	{
		if ( !m_first_tray_message_is_shown )
		{
			m_first_tray_message_is_shown = true;
			m_position_in_stack = 1;
		}
		PopupWindow *popup_win = new PopupWindow(item.m_account_name, m_popup_width,
			m_popup_height, m_popup_sec, m_popup_position,
			m_popup_style, m_position_in_stack , false);
		popup_win->setTheme(m_tray_theme_header_sys, m_tray_theme_header_sys_css, m_tray_theme_content_sys,
				m_tray_theme_content_sys_css, m_tray_theme_path);
		popup_win->setSystemData(item.m_account_name,message);
		popup_win->firstTrayWindow = m_first_tray_message_is_shown;
		popup_win->showTrayMessage();
		if ( (++m_position_in_stack) > 3)
			m_position_in_stack = 3;
		m_popup_list.append(popup_win);
	}
}

void AbstractNotificationLayer::userMessage(const TreeModelItem &item, const QString &message, NotificationType type)
{
	PluginSystem::instance().userNotification(item, message, type);
	QString trayMessageMsg;
	QString msg;
	//read contact and account info
	PluginSystem &ps = PluginSystem::instance();
	QStringList contact_info = ps.getAdditionalInfoAboutContact(item);
	TreeModelItem account_item = item;
	account_item.m_item_name = item.m_account_name;
	QStringList account_info = ps.getAdditionalInfoAboutContact(account_item);
	QString contact_nick = item.m_item_name,
	contact_avatar, 
	account_nick = item.m_account_name, 
	account_avatar;
	if ( contact_info.count() > 0)
	{
		contact_nick = contact_info.at(0);
	}
	if ( contact_info.count() > 1 )
	{
		contact_avatar= contact_info.at(1);
	}
	
	if ( account_info.count() > 0)
	{
		account_nick = account_info.at(0);
	}
	if ( account_info.count() > 1 )
	{
		account_avatar = account_info.at(1);
	}
	bool show_message = false;
	switch ( type )
	{
                case NotifyStatusChange:
			msg = contact_nick + " " + message;
			trayMessageMsg =  message;
			show_message = m_show_change_status;
			break;
                case NotifyMessageGet:
			msg = QObject::tr("Message from %1:\n%2").arg(contact_nick).arg(message);
			trayMessageMsg = message;
			show_message = m_show_message;
			break;
                case NotifyTyping:
			msg = QObject::tr("%1 is typing").arg(contact_nick);
			trayMessageMsg = QObject::tr("typing");
			show_message = m_show_typing;
			break;
                case NotifyBlockedMessage:
			msg = QObject::tr("Blocked message from %1:\n%2").arg(contact_nick).arg(message);
			trayMessageMsg = QObject::tr("(BLOCKED)\n") + message;
			show_message = true;
			break;
                case NotifyBirthday:
			msg = QObject::tr("%1 has birthday today!!").arg(contact_nick);
			trayMessageMsg = QObject::tr("has birthday today!!");
			show_message = true;
			break;
                case NotifyCustom:
			msg = contact_nick + "(" + item.m_item_name + ") " + message;
			trayMessageMsg =  message;
			show_message = true;
			break;
                case NotifyOnline:
			msg = contact_nick + " " + message;
			trayMessageMsg =  message;
			show_message = m_show_signon;
			break;
                case NotifyOffline:
			msg = contact_nick + " " + message;
			trayMessageMsg =  message;
			show_message = m_show_signoff;
			break;
		default:
			msg = contact_nick + " " + message;
			trayMessageMsg =  message;
			show_message = true;
	}
	if ( m_show_balloon && show_message )
	{
		AbstractLayer::instance().showBalloon(account_nick,
				msg, m_balloon_sec * 1000);
	}
	
	if ( m_show_popup && show_message )
	{
		if ( !m_first_tray_message_is_shown )
		{
				m_first_tray_message_is_shown = true;
				m_position_in_stack = 1;
		}

		PopupWindow *popup_window = new PopupWindow(item.m_item_name,
				m_popup_width,m_popup_height, m_popup_sec, m_popup_position,
							m_popup_style, m_position_in_stack );

		switch(type)
		{
		case 1:
			popup_window->setTheme(m_tray_theme_header_msg, m_tray_theme_header_msg_css, m_tray_theme_content_msg,
					m_tray_theme_content_msg_css, m_tray_theme_path);
			break;
		default:
			popup_window->setTheme(m_tray_theme_header_onl, m_tray_theme_header_onl_css, m_tray_theme_content_onl,
					m_tray_theme_content_onl_css, m_tray_theme_path);
		}
		popup_window->setData(item, account_nick,
				contact_nick, contact_avatar,trayMessageMsg);
		popup_window->firstTrayWindow = m_first_tray_message_is_shown;

		popup_window->showTrayMessage();

		if ( (++m_position_in_stack) > 3)
			m_position_in_stack = 3;

		m_popup_list.append(popup_window);
	}
}

void AbstractNotificationLayer::deletePopupWindow(PopupWindow *window)
{
	if ( window->firstTrayWindow )
	{
		m_first_tray_message_is_shown = false;
	}
	m_popup_list.removeAll(window);
}

void AbstractNotificationLayer::loadTheme()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
	settings.beginGroup("gui");
	m_tray_theme_path = settings.value("popup","").toString();
	settings.endGroup();
	if ( m_tray_theme_path.isEmpty() )
	{
		m_tray_theme_content_msg.clear();
		m_tray_theme_content_msg_css.clear();
		m_tray_theme_content_onl.clear();
		m_tray_theme_content_onl_css.clear();
		m_tray_theme_content_sys.clear();
		m_tray_theme_content_sys_css.clear();
		m_tray_theme_header_msg.clear();
		m_tray_theme_header_msg_css.clear();
		m_tray_theme_header_onl.clear();
		m_tray_theme_header_onl_css.clear();
		m_tray_theme_header_sys.clear();
		m_tray_theme_header_sys_css.clear();
	} else
	{
		QFile fileOnlHead(m_tray_theme_path + "/onlalert/header.html");

		if ( fileOnlHead.open(QIODevice::ReadOnly))
		{
			m_tray_theme_header_onl = fileOnlHead.readAll();
			fileOnlHead.close();
		}

		QFile fileOnlHeadCSS(m_tray_theme_path + "/onlalert/header.css");

		if ( fileOnlHeadCSS.open(QIODevice::ReadOnly))
		{
			m_tray_theme_header_onl_css = fileOnlHeadCSS.readAll();
			fileOnlHeadCSS.close();
		}

		QFile fileOnlCont(m_tray_theme_path + "/onlalert/content.html");

		if ( fileOnlCont.open(QIODevice::ReadOnly))
		{
			m_tray_theme_content_onl = fileOnlCont.readAll();
			fileOnlCont.close();
		}

		QFile fileOnlContCSS(m_tray_theme_path + "/onlalert/content.css");

		if ( fileOnlContCSS.open(QIODevice::ReadOnly))
		{
			m_tray_theme_content_onl_css = fileOnlContCSS.readAll();
			fileOnlContCSS.close();
		}

		QFile fileMsgHead(m_tray_theme_path + "/msg/header.html");

		if ( fileMsgHead.open(QIODevice::ReadOnly))
		{
			m_tray_theme_header_msg = fileMsgHead.readAll();
			fileMsgHead.close();
		}

		QFile fileMsgHeadCSS(m_tray_theme_path + "/msg/header.css");

		if ( fileMsgHeadCSS.open(QIODevice::ReadOnly))
		{
			m_tray_theme_header_msg_css = fileMsgHeadCSS.readAll();
			fileMsgHeadCSS.close();
		}


		QFile fileMsgCont(m_tray_theme_path + "/msg/content.html");

		if ( fileMsgCont.open(QIODevice::ReadOnly))
		{
			m_tray_theme_content_msg = fileMsgCont.readAll();
			fileMsgCont.close();
		}

		QFile fileMsgContCSS(m_tray_theme_path + "/msg/content.css");

		if ( fileMsgContCSS.open(QIODevice::ReadOnly))
		{
			m_tray_theme_content_msg_css = fileMsgContCSS.readAll();
			fileMsgContCSS.close();
		}

		QFile fileSysHead(m_tray_theme_path + "/system/header.html");

		if ( fileSysHead.open(QIODevice::ReadOnly))
		{
			m_tray_theme_header_sys = fileSysHead.readAll();
			fileSysHead.close();
		}

		QFile fileSysHeadCSS(m_tray_theme_path + "/system/header.css");

		if ( fileSysHeadCSS.open(QIODevice::ReadOnly))
		{
			m_tray_theme_header_sys_css = fileSysHeadCSS.readAll();
			fileSysHeadCSS.close();
		}

		QFile fileSysCont(m_tray_theme_path + "/system/content.html");

		if ( fileSysCont.open(QIODevice::ReadOnly))
		{
			m_tray_theme_content_sys = fileSysCont.readAll();
			fileSysCont.close();
		}

		QFile fileSysContCSS(m_tray_theme_path + "/system/content.css");

		if ( fileSysContCSS.open(QIODevice::ReadOnly))
		{
			m_tray_theme_content_sys_css = fileSysContCSS.readAll();
			fileSysContCSS.close();
		}
		}
}
