/*
    ActivityExtenstion, XEP-0108

    Copyright (c) 2008-2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef activityextension_H
#define activityextension_H

#include "gloox/stanzaextension.h"
#include "src/jProtocol.h"
#include <QString>

class ActivityExtension : public StanzaExtension
{
public:
	ActivityExtension(const Tag* tag = 0);
	ActivityExtension(const QString &general, const QString &specific="", const QString &text="");
	const std::string &filterString () const;
        StanzaExtension *newInstance (const Tag *tag) const;
	Tag* tag() const;
        StanzaExtension* clone() const
        {
            return new ActivityExtension(m_general, m_specific, m_text);
        }
	const QString &general() const {return m_general;}
	const QString &specific() const {return m_specific;}
	const QString &text() const {return m_text;}
private:
	QString m_general;
	QString m_specific;
	QString m_text;
};

#endif
