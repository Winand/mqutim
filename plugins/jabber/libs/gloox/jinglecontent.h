/*
  Copyright (c) 2008-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef JINGLECONTENT_H__
#define JINGLECONTENT_H__


#include "jingleplugin.h"
#include "tag.h"

#include <string>

namespace gloox
{

  namespace Jingle
  {

    /**
     * @brief An abstraction of a Jingle Content Type.
     *
     * You should not need to use this class directly. See
     * @link gloox::Jingle::Session Jingle::Session @endlink for more info on Jingle.
     *
     * @author Jakob Schroeter <js@camaya.net>
     * @since 1.0
     */
    class Content : public Plugin
    {
      public:
        /**
         * Creates a new Content wrapper.
         */
        Content();

        /**
         * Virtual destructor.
         */
        virtual ~Content();

        // reimplemented from Plugin
        virtual const std::string& filterString() const;

        // reimplemented from Plugin
        virtual Tag* tag() const;

    };

  }

}

#endif // JINGLECONTENT_H__
