/*****************************************************************************
**                                                                          **
**    Copyright (C) 2008 Denisss (Denis Novikov). All rights reserved.      **
**                                                                          **
**    This file contains global constaints enums and typedefs.              **
**                                                                          **
**    This  file may be used  under the terms of the GNU General  Public    **
**    License  version 2.0 as published by the Free Software Foundation.    **
**    Alternatively  you may (at your option) use any  later  version of    **
**    the GNU General Public License.                                       **                                                **
**                                                                          **
**        This file is provided AS IS with NO  WARRANTY OF ANY KIND,        **
**          INCLUDING THE  WARRANTY OF DESIGN, MERCHANTABILITY AND          **
**                   FITNESS  FOR A PARTICULAR PURPOSE.                     **
**                                                                          **
*****************************************************************************/

#ifndef QUTICQGLOBALS_H
#define QUTICQGLOBALS_H

enum userMessageType { statusNotyfication, messageNotification, typingNotification,
	readNotification,blockedMessage, xstatusReadNotification , customMessage};

enum TrayPosition { TopLeft, TopRight, BottomLeft, BottomRight };

enum accountStatus { online= 0, ffc, away, na, occupied, dnd, invisible, lunch,
	evil, depression, athome, atwork, offline, connecting
	//!!! STATUS_COUNT must always be at the last position !!!//
	, restoreAccount, STATUS_COUNT };
const char* const StatusNames[] =
	{ "online", "ffc", "away", "na", "occupied", "dnd", "invisible",
	  "lunch", "evil", "depression", "athome", "atwork", "offline",
	  "connecting"
	  , 0 };

enum contactStatus { contactOnline = 0, contactFfc,
	contactAway, contactLunch, contactAtWork, contactAtHome, contactEvil,
	contactDepression, contactNa, contactOccupied, contactDnd,
	contactInvisible, contactOffline };

namespace SoundEvent
{
	enum SoundSystem { None = 0, LibPhonon,  LibSound, UserCommand };

	enum Events { ContactOnline = contactOnline, ContactFfc, ContactAway,
		ContactLunch, ContactAtWork, ContactAtHome, ContactEvil,
		ContactDepression, ContactNa, ContactOccupied, ContactDnd,
		ContactInvisible, ContactOffline
		// Reserve 9 contact statuses
		, ContactBirthday = ContactOffline + 10, Startup, Connected,
		Disconnected, MessageGet, MessageSend, SystemEvent
		//!!! EVENT_COUNT must always be at the last position !!!//
		, EVENT_COUNT }; // more events?
	const char* const XmlEventNames[] = { "c_online", "c_ffc",
		"c_away", "c_lunch", "c_work", "c_home", "c_evil",
		"c_depression", "c_na", "c_occupied", "c_dnd",
		"c_invis", "c_offline", 0, 0, 0, 0, 0, 0, 0, 0, 0,
		"c_birth", "start", "connect", "disconnect",
		"m_get", "m_send", "sys_event", 0 };
}

#endif // QUTICQGLOBALS_H
