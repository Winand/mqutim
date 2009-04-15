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

#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QSysInfo>
#include <QProcess>
#include <QTextStream>

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>

#include "systeminfo.h"

SystemInfo::SystemInfo() : QObject(QCoreApplication::instance())
{
	// Initialize
	struct utsname uts;
	uname (&uts);
	timezone_offset_ = 0;
	timezone_str_ = "N/A";
	os_str_.append(uts.sysname);
	os_str_.append(" ");
	os_str_.append(uts.machine);
	os_str_.append(" ");
	os_str_.append(uts.release);
}

SystemInfo* SystemInfo::instance()
{
	if (!instance_)
		instance_ = new SystemInfo();
	return instance_;
}

SystemInfo* SystemInfo::instance_ = NULL;
