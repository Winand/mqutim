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


#ifndef STATUSICONSCLASS_H_
#define STATUSICONSCLASS_H_

#include <QtCore>
#include <QIcon>


#ifdef _MSC_VER
#pragma warning (disable:4100)
#endif

class treeBuddyItem;

class statusIconClass
{
public:
	typedef const QIcon &(statusIconClass::*getIconFunction)() const;
	//const QIcon &(__thiscall statusIconClass::* )(void) const;


	QList<QString> xstatusList;

	static statusIconClass		*getInstance();
	bool						reloadIcons();
	bool						reloadIconsFromSettings();
	const QString getStatusPath(const QString &status, const QString &path );
	const QIcon					&getOnlineIcon() const;
	const QIcon					&getFreeForChatIcon() const;
	const QIcon					&getAwayIcon() const;
	const QIcon					&getNotAvailableIcon() const;
	const QIcon					&getOccupiedIcon() const;
	const QIcon					&getDoNotDisturbIcon() const;
	const QIcon					&getInvisibleIcon() const;
	const QIcon					&getOfflineIcon() const;
	const QIcon					&getConnectingIcon() const;
	const QIcon					&getAtHomeIcon() const;
	const QIcon					&getAtWorkIcon() const;
	const QIcon					&getLunchIcon() const;
	const QIcon					&getEvilIcon() const;
	const QIcon					&getDepressionIcon() const;
	const QIcon					&getContentIcon() const;

private:
	static QMutex				fInstGuard;
	static	statusIconClass		*fInstance;

	mutable QMutex				fIconLocker;

	QIcon						onlineIcon;
	QIcon						ffcIcon;
	QIcon						awayIcon;
	QIcon						naIcon;
	QIcon						occupiedIcon;
	QIcon						dndIcon;
	QIcon						invisibleIcon;
	QIcon						offlineIcon;
	QIcon						connectingIcon;
	QIcon						atHomeIcon;
	QIcon						atWorkIcon;
	QIcon						lunchIcon;
	QIcon						evilIcon;
	QIcon						depressionIcon;
	QIcon						contentIcon;
	
	statusIconClass();
	statusIconClass(const statusIconClass & /* iconClass */) {  }
	~statusIconClass();

	statusIconClass operator = (const statusIconClass & /* iconClass */) { return *this; };

	static void					release();
};


#endif /*STATUSICONSCLASS_H_*/
