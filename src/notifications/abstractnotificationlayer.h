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
#include <QString>

class AbstractNotificationLayer
{
public:
	static AbstractNotificationLayer &instance();
	void loadProfile(const QString &profile_name);
	void loadSettings();
	void systemMessage(const TreeModelItem &item, const QString &message);
  void userMessage(const TreeModelItem &item, const QString &, NotificationType);
private:
	AbstractNotificationLayer();
	virtual ~AbstractNotificationLayer();
	QString m_profile_name;
	bool m_show_popup;

	bool m_show_signon;
	bool m_show_signoff;
	bool m_show_typing;
	bool m_show_change_status;
	bool m_show_message;
};

#endif /*ABSTRACTNOTIFICATIONLAYER_H_*/
