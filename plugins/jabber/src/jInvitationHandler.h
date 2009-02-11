/*
Copyright (c) 2008 by Denis Daschenko <daschenko@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef JINVITATIONHANDLER_H
#define JINVITATIONHANDLER_H

#include <QString>
#include <QObject>
#include <gloox/mucinvitationhandler.h>

using namespace gloox;

class jInvitationHandler : public QObject, public MUCInvitationHandler
{

        Q_OBJECT

public:
        jInvitationHandler(ClientBase *parent);
        void handleMUCInvitation(const JID& room, const JID& from, const std::string& reason, const std::string& body, const std::string& password, bool cont, const std::string& thread);

signals:
        void conferenceInvite(const JID &room, const JID &from, const QString &reason, const QString &password);

};

#endif //JINVITATIONHANDLER_H
