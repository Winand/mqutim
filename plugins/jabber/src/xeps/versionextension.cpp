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

#include "versionextension.h"
#include "../jProtocol.h"
#include <QDebug>

VersionExtension::VersionExtension (const Tag* tag)
	: StanzaExtension( ExtVersion )
{
	if(!tag)
		return;
	Tag* t = tag->findChild( "name" );
	if( t )
		m_name = jProtocol::fromStd(t->cdata());
	t = tag->findChild( "version" );
	if( t )
		m_version = jProtocol::fromStd(t->cdata());
	t = tag->findChild( "os" );
	if( t )
		m_os = jProtocol::fromStd(t->cdata());
}

const std::string &VersionExtension::filterString () const
{
	static const std::string filter = "/iq/query[@xmlns='" + XMLNS_VERSION + "']";
	return filter;
}

StanzaExtension *VersionExtension::newInstance (const Tag *tag) const
{
	return new VersionExtension(tag);
}

Tag* VersionExtension::tag() const
{
	Tag* t = new Tag( "query" );
	t->setXmlns( XMLNS_VERSION );

	if( !m_name.isEmpty() )
		new Tag( t, "name", jProtocol::toStd(m_name) );

	if( !m_version.isEmpty() )
		new Tag( t, "version", jProtocol::toStd(m_version) );

	if ( !m_os.isEmpty() )
		new Tag( t, "os", jProtocol::toStd(m_os) );

	return t;
}
