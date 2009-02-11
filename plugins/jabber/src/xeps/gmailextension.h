/*****************************************************************************
    Gmail Notifications, UserId+2

    Copyright (c) 2008-2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef gmailextension_H
#define gmailextension_H

#include "gloox/stanzaextension.h"
#include "src/jProtocol.h"
#include <QString>
#include <QStringList>
#include <QDateTime>

using namespace gloox;

class GMailExtension : public StanzaExtension
{
public:
	struct Sender
	{
		QString name;
		QString address;
		bool originator;
		bool unread;
	};
	struct MailThread
	{
		QString url;
		QDateTime time;
		QString tid;
		int messages;
		QStringList labels;
		QString subject;
		QString snippet;
		QList<Sender> senders;
	};
        GMailExtension(const Tag* tag=0);
	GMailExtension(quint64 date);
	const std::string &filterString () const;
	StanzaExtension *newInstance (const Tag *tag) const;
	Tag* tag() const;
        StanzaExtension* clone() const
        {
            GMailExtension *gmail = new GMailExtension();
            gmail->m_mail_threads = m_mail_threads;
            gmail->m_notification = m_notification;
            gmail->m_date = m_date;
            gmail->m_total_matched = m_total_matched;
            gmail->m_query = m_query;
            return gmail;
        }
	const QList<MailThread> &getMailThreads() const { return m_mail_threads; }
	bool isNotification() const { return m_notification; }
	quint64 getResultTime() const { return m_date; }
	int getTotalMatched() const { return m_total_matched; }
private:
	QList<MailThread> m_mail_threads;
	bool m_notification;
	quint64 m_date;
	int m_total_matched;
	bool m_query;
};

#endif
