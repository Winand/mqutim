/*
    statusIconClass

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

#include "statusiconsclass.h"
#include "icqpluginsystem.h"

// Statics
QMutex statusIconClass::fInstGuard;
statusIconClass *statusIconClass::fInstance = NULL;

statusIconClass::statusIconClass()

{
	reloadIcons();
}

statusIconClass::~statusIconClass()
{

}

statusIconClass *statusIconClass::getInstance()
{
	if (!fInstance)
	{
		QMutexLocker locker(&fInstGuard);
		if (!fInstance)
		{
			fInstance = new statusIconClass;
			atexit(&statusIconClass::release);
		}
	}

	return fInstance;
}

void statusIconClass::release()
{
	QMutexLocker locker(&fInstGuard);
	if (fInstance)
	{
		delete fInstance;
		fInstance = NULL;
	}
}

bool statusIconClass::reloadIcons()
{
	QMutexLocker locker(&fIconLocker);

	onlineIcon = IcqPluginSystem::instance().getStatusIcon("online", "icq");
	ffcIcon = IcqPluginSystem::instance().getStatusIcon("ffc", "icq");
	awayIcon = IcqPluginSystem::instance().getStatusIcon("away", "icq");
	naIcon = IcqPluginSystem::instance().getStatusIcon("na", "icq");
	occupiedIcon = IcqPluginSystem::instance().getStatusIcon("occupied", "icq");
	dndIcon = IcqPluginSystem::instance().getStatusIcon("dnd", "icq");
	invisibleIcon = IcqPluginSystem::instance().getStatusIcon("invisible", "icq");
	offlineIcon = IcqPluginSystem::instance().getStatusIcon("offline", "icq");
	connectingIcon = IcqPluginSystem::instance().getStatusIcon("connecting", "icq");
	atHomeIcon = IcqPluginSystem::instance().getStatusIcon("athome", "icq");
	atWorkIcon = IcqPluginSystem::instance().getStatusIcon("atwork", "icq");
	lunchIcon = IcqPluginSystem::instance().getStatusIcon("lunch", "icq");
	evilIcon = IcqPluginSystem::instance().getStatusIcon("evil", "icq");
	depressionIcon = IcqPluginSystem::instance().getStatusIcon("depression", "icq");
	contentIcon = IcqPluginSystem::instance().getStatusIcon("content", "icq");

	xstatusList.clear();
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus0"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus1"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus2"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus3"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus4"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus5"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus6"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus7"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus8"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus9"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus10"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus11"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus12"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus13"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus14"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus15"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus16"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus17"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus18"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus19"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus20"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus21"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus22"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus23"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus24"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus25"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus26"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus27"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus28"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus29"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus30"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus31"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus32"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus33"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus34"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus35"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus36"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus37"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus38"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus39"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus40"));
	xstatusList.append(IcqPluginSystem::instance().getIconFileName("icq_xstatus41"));
//	xstatusList.insert(0, ":/icons/xstatus/icq_xstatus0.png");
//	xstatusList.insert(1, ":/icons/xstatus/icq_xstatus1.png");
//	xstatusList.insert(2, ":/icons/xstatus/icq_xstatus2.png");
//	xstatusList.insert(3, ":/icons/xstatus/icq_xstatus3.png");
//	xstatusList.insert(4, ":/icons/xstatus/icq_xstatus4.png");
//	xstatusList.insert(5, ":/icons/xstatus/icq_xstatus5.png");
//	xstatusList.insert(6, ":/icons/xstatus/icq_xstatus6.png");
//	xstatusList.insert(7, ":/icons/xstatus/icq_xstatus7.png");
//	xstatusList.insert(8, ":/icons/xstatus/icq_xstatus8.png");
//	xstatusList.insert(9, ":/icons/xstatus/icq_xstatus9.png");
//	xstatusList.insert(10, ":/icons/xstatus/icq_xstatus10.png");
//	xstatusList.insert(11, ":/icons/xstatus/icq_xstatus11.png");
//	xstatusList.insert(12, ":/icons/xstatus/icq_xstatus12.png");
//	xstatusList.insert(13, ":/icons/xstatus/icq_xstatus13.png");
//	xstatusList.insert(14, ":/icons/xstatus/icq_xstatus14.png");
//	xstatusList.insert(15, ":/icons/xstatus/icq_xstatus15.png");
//	xstatusList.insert(16, ":/icons/xstatus/icq_xstatus16.png");
//	xstatusList.insert(17, ":/icons/xstatus/icq_xstatus17.png");
//	xstatusList.insert(18, ":/icons/xstatus/icq_xstatus18.png");
//	xstatusList.insert(19, ":/icons/xstatus/icq_xstatus19.png");
//	xstatusList.insert(20, ":/icons/xstatus/icq_xstatus20.png");
//	xstatusList.insert(21, ":/icons/xstatus/icq_xstatus21.png");
//	xstatusList.insert(22, ":/icons/xstatus/icq_xstatus22.png");
//	xstatusList.insert(23, ":/icons/xstatus/icq_xstatus23.png");
//	xstatusList.insert(24, ":/icons/xstatus/icq_xstatus24.png");
//	xstatusList.insert(25, ":/icons/xstatus/icq_xstatus25.png");
//	xstatusList.insert(26, ":/icons/xstatus/icq_xstatus26.png");
//	xstatusList.insert(27, ":/icons/xstatus/icq_xstatus27.png");
//	xstatusList.insert(28, ":/icons/xstatus/icq_xstatus28.png");
//	xstatusList.insert(29, ":/icons/xstatus/icq_xstatus29.png");
//	xstatusList.insert(30, ":/icons/xstatus/icq_xstatus30.png");
//	xstatusList.insert(31, ":/icons/xstatus/icq_xstatus31.png");
//	xstatusList.insert(32, ":/icons/xstatus/icq_xstatus32.png");
//	xstatusList.insert(33, ":/icons/xstatus/icq_xstatus33.png");
	return true;
}

bool statusIconClass::reloadIconsFromSettings()
{

	return reloadIcons();
}

const QIcon &statusIconClass::getOnlineIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return onlineIcon;
}

const QIcon	&statusIconClass::getFreeForChatIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return ffcIcon;
}

const QIcon	&statusIconClass::getAwayIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return awayIcon;
}

const QIcon	&statusIconClass::getNotAvailableIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return naIcon;
}

const QIcon	&statusIconClass::getOccupiedIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return occupiedIcon;
}

const QIcon &statusIconClass::getDoNotDisturbIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return dndIcon;
}

const QIcon &statusIconClass::getInvisibleIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return invisibleIcon;
}

const QIcon &statusIconClass::getOfflineIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return offlineIcon;
}

const QIcon &statusIconClass::getConnectingIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return connectingIcon;
}

const QIcon &statusIconClass::getAtHomeIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return atHomeIcon;
}

const QIcon &statusIconClass::getAtWorkIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return atWorkIcon;
}

const QIcon &statusIconClass::getLunchIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return lunchIcon;
}

const QIcon &statusIconClass::getEvilIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return evilIcon;
}

const QIcon &statusIconClass::getDepressionIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return depressionIcon;
}

const QIcon &statusIconClass::getContentIcon() const
{
	QMutexLocker locker(&fIconLocker);
	return contentIcon;
}

const QString statusIconClass::getStatusPath(const QString &status, const QString &path )
{
	return IcqPluginSystem::instance().getStatusIconFileName(status,path);
}
