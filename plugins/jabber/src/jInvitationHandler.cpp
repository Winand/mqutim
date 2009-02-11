/*
Copyright (c) 2008 by Denis Daschenko <dasschenko@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#include "jProtocol.h"
#include "jInvitationHandler.h"
#include <gloox/clientbase.h>

jInvitationHandler::jInvitationHandler(ClientBase *parent) : MUCInvitationHandler(parent)
{
	//connect(m_jabber_protocol, SIGNAL(conferenceInvite(const JID&, const JID&, const QString&, const QString&)), m_conference_management_object, SLOT(s_conferenceInvite(const JID&, const JID&, const QString&, const QString&)));
}

void jInvitationHandler::handleMUCInvitation(const JID& room, const JID& from, const std::string& reason, const std::string& body, const std::string& password, bool cont, const std::string& thread)
{
	if (!jProtocol::fromStd(from.full()).isEmpty())
		emit conferenceInvite(room, from, jProtocol::fromStd(reason), jProtocol::fromStd(password));
}
