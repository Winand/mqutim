/*****************************************************************************
    System Info

    Copyright (c) 2007-2008 by Remko Tronçon
    					2008 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <QObject>
#include <QString>

class SystemInfo : public QObject
{
public:
	static SystemInfo* instance();
	const QString& os() const { return os_str_; }
	int timezoneOffset() const { return timezone_offset_; }
	const QString& timezoneString() const { return timezone_str_; }

private:
	SystemInfo();

	static SystemInfo* instance_;
	int timezone_offset_;
	QString timezone_str_;
	QString os_str_;
};

#endif
