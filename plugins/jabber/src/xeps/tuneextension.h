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

#ifndef tuneextension_H
#define tuneextension_H

#include "gloox/stanzaextension.h"
#include "src/jProtocol.h"
#include <QString>

class TuneExtension : public StanzaExtension
{
public:
	TuneExtension(const Tag* tag = 0);
	const std::string &filterString () const;
	StanzaExtension *newInstance (const Tag *tag) const;
	Tag* tag() const;
        StanzaExtension* clone() const
        {
            TuneExtension *tune = new TuneExtension();
            tune->m_artist = m_artist;
            tune->m_length = m_length;
            tune->m_rating = m_rating;
            tune->m_source = m_source;
            tune->m_title = m_title;
            tune->m_track = m_track;
            tune->m_uri = m_uri;
            return tune;
        }
	void setArtist(const QString &artist) { m_artist = artist; }
	void setLength(int length) { m_length = length; }
	void setRating(int rating) { m_rating = rating; }
	void setSource(const QString &source) { m_source = source; }
	void setTitle(const QString &title) { m_title = title; }
	void setTrack(const QString &track) { m_track = track; }
	void setUri(const QString &uri) { m_uri = uri; }
	const QString &getArtist() { return m_artist ; }
	int getLength() { return m_length; }
	int getRating() { return m_rating; }
	const QString &getSource() { return m_source; }
	const QString &getTitle() { return m_title; }
	const QString &getTrack() { return m_track; }
	const QString &getUri() { return m_uri; }
private:
QString m_artist;
int m_length;
int m_rating;
QString m_source;
QString m_title;
QString m_track;
QString m_uri;
};

#endif
