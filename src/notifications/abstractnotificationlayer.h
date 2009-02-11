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

#ifndef ABSTRACTNOTIFICATIONLAYER_H_
#define ABSTRACTNOTIFICATIONLAYER_H_

#include "../pluginsystem.h"
#include "popupwindow.h"
#include <QString>

class AbstractNotificationLayer
{
public:
	static AbstractNotificationLayer &instance();
	void loadProfile(const QString &profile_name);
	void loadSettings();
	void systemMessage(const TreeModelItem &item, const QString &message);
	void userMessage(const TreeModelItem &item, const QString &, int);
	void deletePopupWindow(PopupWindow *window);
	void loadTheme();
private:
	AbstractNotificationLayer();
	virtual ~AbstractNotificationLayer();
	QString m_profile_name;
	bool m_show_popup;
	int m_popup_width;
	int m_popup_height;
	int m_popup_sec;
	int m_popup_position;
	int m_popup_style;
	bool m_show_balloon;
	int m_balloon_sec;
	bool m_show_signon;
	bool m_show_signoff;
	bool m_show_typing;
	bool m_show_change_status;
	int m_position_in_stack;
	bool m_show_message;
	bool m_first_tray_message_is_shown;
	QList<PopupWindow *> m_popup_list;
	QString m_tray_theme_path;
	QString m_tray_theme_header_onl;
	QString m_tray_theme_header_onl_css;
	QString m_tray_theme_content_onl;
	QString m_tray_theme_content_onl_css;
	QString m_tray_theme_header_msg;
	QString m_tray_theme_header_msg_css;
	QString m_tray_theme_content_msg;
	QString m_tray_theme_content_msg_css;
	QString m_tray_theme_header_sys;
	QString m_tray_theme_header_sys_css;
	QString m_tray_theme_content_sys;
	QString m_tray_theme_content_sys_css;
};

#endif /*ABSTRACTNOTIFICATIONLAYER_H_*/
