/*
  Copyright (c) 2008-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#include "jingletransport.h"
#include "tag.h"

namespace gloox
{

  namespace Jingle
  {

    const std::string& Transport::filterString() const
    {
      static const std::string filter = "./transport[@xmlns='" + xmlns() + "']";
      return filter;
    }

    Tag* Transport::tag() const
    {
      Tag* t = new Tag( "transport" );
      t->setXmlns( xmlns() );
      t->addChild( childTag() );
      return t;
    }

  }

}
