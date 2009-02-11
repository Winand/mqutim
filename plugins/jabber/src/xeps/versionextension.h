/*
    VersionExtenstion XEP-0092

    Copyright (c) 2008-2009 by Nigmatullin Rustam <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef VERSIONEXTENSION_H_
#define VERSIONEXTENSION_H_
#include <QString>
#include "gloox/gloox.h"
#include "gloox/clientbase.h"
#include "gloox/disconodehandler.h"
#include "gloox/stanzaextension.h"
#include "gloox/tag.h"
#include "src/jProtocol.h"

using namespace gloox;

class VersionExtension : public StanzaExtension
{
public:
	VersionExtension (const Tag* tag = 0);
	const std::string &filterString () const;
	StanzaExtension *newInstance (const Tag *tag) const;
	Tag* tag() const;
        StanzaExtension* clone() const
        {
            VersionExtension *version = new VersionExtension();
            version->m_name = m_name;
            version->m_version = m_version;
            version->m_os = m_os;
            return version;
        }
	const QString &name() const {return m_name;}
	const QString &version() const {return m_version;}
	const QString &os() const {return m_os;}
private:
	QString m_name;
	QString m_version;
	QString m_os;
};

#endif /*VERSIONEXTENSION_H_*/
