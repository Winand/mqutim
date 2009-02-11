/*
    AbstractStatusLayer

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

#ifndef ABSTRACTSTATUS_H
#define ABSTRACTSTATUS_H

#include <QString>
#include "iconmanager.h"

class AbstractStatusLayer
{
public:
	static AbstractStatusLayer &instance();
	inline void setProfileName(const QString &profile_name)
	{
		m_profile_name = profile_name;
	}
	bool setStatusMessage(QString &status_message, bool &dshow);
private:
	AbstractStatusLayer();
	~AbstractStatusLayer();
	QString m_profile_name;
	IconManager &m_icon_manager;
};

#endif // ABSTRACTSTATUS_H
