/*
    MoodsExtenstion

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>
             2008-2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "moodsextenstion.h"

MoodsExtenstion::MoodsExtenstion(const Tag* tag)
: StanzaExtension( ExtUser+107 )
{
	if(!tag)
		return;
	TagList tag_list = tag->children();
	if ( !tag_list.empty() )
		m_mood_name = jProtocol::fromStd(tag_list.front()->name());
	if(m_mood_name=="text")
		m_mood_name="";
	if(!m_mood_name.isEmpty())
	{
		if(!jPluginSystem::instance().getMoodTr().contains(m_mood_name))
			m_mood_name="undefined";
		Tag* t = tag->findChild( "text" );
		if( t )
			m_mood_text = jProtocol::fromStd(t->cdata());
	}
}

MoodsExtenstion::MoodsExtenstion(const QString &name, const QString &text)
: StanzaExtension( ExtUser+107 )
{
	m_mood_name = name;
	m_mood_text = text;
}

const std::string &MoodsExtenstion::filterString () const
{
	static const std::string filter = "/message/event/items/item/mood[@xmlns='http://jabber.org/protocol/mood']";
	return filter;
}

StanzaExtension *MoodsExtenstion::newInstance (const Tag *tag) const
{
	return new MoodsExtenstion(tag);
}

Tag* MoodsExtenstion::tag() const
{
	Tag* t = new Tag( "mood" );
	t->setXmlns( "http://jabber.org/protocol/mood");
	
	if ( !m_mood_name.isEmpty() )
	{
		new Tag(t, jProtocol::toStd(m_mood_name), "");
		if ( !m_mood_text.isEmpty() )
			new Tag(t, "text", jProtocol::toStd(m_mood_text));
	}
	return t;
}

/*int MoodsExtenstion::extensionType() const{
	return ExtUser+107;
}*/
