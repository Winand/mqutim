/*****************************************************************************
    QIP X-Statuses, UserId+1

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

#ifndef xstatusextension_H
#define xstatusextension_H

#include "gloox/stanzaextension.h"
#include "src/jProtocol.h"
#include <QString>

using namespace gloox;

class XStatusExtension : public StanzaExtension
{
public:
	XStatusExtension(const Tag* tag = 0);
	const std::string &filterString () const;
	StanzaExtension *newInstance (const Tag *tag) const;
	Tag* tag() const;
        StanzaExtension* clone() const
        {
            XStatusExtension *xstatus = new XStatusExtension();
            xstatus->m_title = m_title;
            xstatus->m_text = m_text;
            xstatus->m_id = m_id;
            return xstatus;
        }
	const QString &status_title() const {return m_title;}
	const QString &status_text() const {return m_text;}
	int status_id() const {return m_id;}
private:
	QString m_title;
	QString m_text;
	int m_id;
};

#endif
