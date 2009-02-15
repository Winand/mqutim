/*!
 * Jabber Plugin SDK
 *
 * @author Nigmatullin Ruslan
 * Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>
 * encoding: UTF-8 
 *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
 */

#ifndef QUTIM_JABBER_SDK_H
#define QUTIM_JABBER_SDK_H

namespace qutim_sdk_jabber {

enum EventToJabber
{
	SetStatus = 0,
	RestoreStatus,
	SetMood,
	SetActivity,
	SetTune
};

enum Status
{
	Simple = -1, /*!< do not change */
	Online = 0,
	FreeForChat = 1,
	Away = 2,
	NA = 3,
	DND = 5,
	Invisible = 6,
	Offline = 12,
	Connecting = 13
};	

} //end namespace qutim_sdk_jabber

#endif //QUTIM_JABBER_SDK_H
