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

#ifndef MOODSEXTENSTION_H_
#define MOODSEXTENSTION_H_

#include "gloox/stanzaextension.h"
#include "src/jProtocol.h"
#include <QString>

using namespace gloox;

class MoodsExtenstion : public StanzaExtension
{
public:
	MoodsExtenstion(const Tag* tag = 0);
        MoodsExtenstion(const QString &name, const QString &text="");
	const std::string &filterString () const;
	StanzaExtension *newInstance (const Tag *tag) const;
	Tag* tag() const;
        StanzaExtension* clone() const
        {
            return new MoodsExtenstion(m_mood_name, m_mood_text);
        }
	const QString &moodName() const {return m_mood_name;}
	const QString &moodText() const {return m_mood_text;}
private:
	QString m_mood_name;
	QString m_mood_text;
};

#endif /*MOODSEXTENSTION_H_*/
