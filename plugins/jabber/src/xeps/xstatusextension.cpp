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

#include "xstatusextension.h"

XStatusExtension::XStatusExtension(const Tag* tag) 
: StanzaExtension( ExtUser+1 )
{
	/*
	 * <x xmlns='http://qip.ru/x-status' id='29'>
	 * 	<title>Тут могла быть</title>
	 * 	<text> ваша реклама</text>
	 * </x>
	 */
	if(!tag)
		return;
	Tag* t = tag->findChild( "title" );
	if( t )
		m_title = jProtocol::fromStd(t->cdata());
	else
		m_title = "";
	t = tag->findChild( "text" );
	if( t )
		m_text = jProtocol::fromStd(t->cdata());
	else
		m_text = "";
	m_id = jProtocol::fromStd(tag->findAttribute("id")).toInt()-1;
	if(m_id==33)
		m_id=31;
	else if(m_id==34)
		m_id=33;
	if(m_id>33 || m_id<0)
		m_id=-1;
}

const std::string &XStatusExtension::filterString () const
{
	static const std::string filter = "/presence/x[@xmlns='http://qip.ru/x-status']";
	return filter;
}

StanzaExtension *XStatusExtension::newInstance (const Tag *tag) const
{
	return new XStatusExtension(tag);
}

Tag* XStatusExtension::tag() const
{
	Tag* t = new Tag( "x" );
	t->setXmlns( "http://qip.ru/x-status");
	t->addAttribute("id",m_id+1);
	if ( !m_title.isEmpty() )
		new Tag(t, "title", jProtocol::toStd(m_title));
	if ( !m_text.isEmpty() )
		new Tag(t, "text", jProtocol::toStd(m_text));
	return t;
}
