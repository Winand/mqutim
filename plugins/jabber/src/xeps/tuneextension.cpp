/*
    TuneExtenstion, XEP-0118

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

#include "tuneextension.h"

TuneExtension::TuneExtension(const Tag* tag) 
: StanzaExtension( ExtUser+118 )
{
	m_length = -1;
	m_rating = -1;
	if(!tag)
		return;
	Tag* t = tag->findChild( "artist" );
	if( t )
		m_artist = jProtocol::fromStd(t->cdata());
	t = tag->findChild( "length" );
	if( t )
		m_length = jProtocol::fromStd(t->cdata()).toInt();
	t = tag->findChild( "rating" );
	if( t )
		m_rating = jProtocol::fromStd(t->cdata()).toInt();
	t = tag->findChild( "source" );
	if( t )
		m_source = jProtocol::fromStd(t->cdata());
	t = tag->findChild( "title" );
	if( t )
		m_title = jProtocol::fromStd(t->cdata());
	t = tag->findChild( "track" );
	if( t )
		m_track = jProtocol::fromStd(t->cdata());
	t = tag->findChild( "uri" );
	if( t )
		m_uri = jProtocol::fromStd(t->cdata());
}

const std::string &TuneExtension::filterString () const
{
	static const std::string filter = "/message/event/items/item/tune[@xmlns='http://jabber.org/protocol/tune']";
	return filter;
}

StanzaExtension *TuneExtension::newInstance (const Tag *tag) const
{
	return new TuneExtension(tag);
}

Tag* TuneExtension::tag() const
{
	Tag* t = new Tag( "tune" );
	t->setXmlns( "http://jabber.org/protocol/tune");
	if ( !m_artist.isEmpty() )
		new Tag(t, "artist", jProtocol::toStd(m_artist));
	if ( m_length>0 )
		new Tag(t, "length", jProtocol::toStd(QString::number(m_length)));
	if ( m_rating>-1 )
		new Tag(t, "rating", jProtocol::toStd(QString::number(m_rating)));
	if ( !m_source.isEmpty() )
		new Tag(t, "source", jProtocol::toStd(m_source));
	if ( !m_title.isEmpty() )
		new Tag(t, "title", jProtocol::toStd(m_title));
	if ( !m_track.isEmpty() )
		new Tag(t, "track", jProtocol::toStd(m_track));
	if ( !m_uri.isEmpty() )
		new Tag(t, "uri", jProtocol::toStd(m_uri));
	
	/*if ( !m_mood_name.isEmpty() )
	{
		new Tag(t, jProtocol::toStd(m_mood_name), "");
		if ( !m_mood_text.isEmpty() )
			new Tag(t, "text", jProtocol::toStd(m_mood_text));
	}*/
	return t;
}
