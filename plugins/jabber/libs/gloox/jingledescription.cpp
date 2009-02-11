/*
  Copyright (c) 2008-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#include "jingledescription.h"
#include "tag.h"
#include "util.h"

namespace gloox
{

  namespace Jingle
  {

    // ---- Jingle::Description::Payload ----
    Tag* Jingle::Description::Payload::tag() const
    {
      if( m_attribs.empty() )
        return 0;

      Tag* t = new Tag( "payload-type" );
      StringMap::const_iterator it = m_attribs.begin();
      for( ; it != m_attribs.end(); ++it )
        t->addAttribute( (*it).first, (*it).second );

      it = m_parameters.begin();
      for( ; it != m_parameters.end(); ++it )
      {
        Tag* p = new Tag( t, "parameter" );
        p->addAttribute( "name", (*it).first );
        p->addAttribute( "value", (*it).second );
      }

      return t;
    }
    // ---- ~Jingle::Description::Payload ----

    // ---- Jingle::Description ----
    Description::~Description()
    {
      util::clearList( m_payload );
    }

    const std::string& Description::filterString() const
    {
      static const std::string filter = "./description[@xmlns='" + xmlns() + "']";
      return filter;
    }

    Tag* Description::tag() const
    {
      Tag* t = new Tag( "description" );
      t->setXmlns( xmlns() );

      PayloadList::const_iterator it = m_payload.begin();
      for( ; it != m_payload.end(); ++it )
      {
        if( (*it) )
          t->addChild( (*it)->tag() );
      }

      return t;
    }
  }

}
