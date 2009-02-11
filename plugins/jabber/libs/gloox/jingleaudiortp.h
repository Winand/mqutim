/*
  Copyright (c) 2008-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef JINGLEAUDIORTP_H__
#define JINGLEAUDIORTP_H__

#include "jingledescription.h"

#include <string>

namespace gloox
{

  class Tag;

  namespace Jingle
  {

    /**
     * @brief An abstraction of the signaling part of Jingle Audio via RTP (XEP-0167).
     *
     * XEP Version: 0.17
     * @author Jakob Schroeter <js@camaya.net>
     * @since 1.0
     */
    class AudioRTP : public Description
    {
      public:
        /**
         * Creates a new wrapper for the Audio via RTP Application Format defined in XEP-0167.
         * @param payload A list of acceptable Payload Types.
         */
        AudioRTP( const PayloadList& payload )
          : Description( payload )
        {}

        /**
         * Virtual destructor.
         */
        virtual ~AudioRTP() {}

        // reimplemented from Description
        virtual const std::string& xmlns() const { return XMLNS_JINGLE_AUDIO_RTP; }

    };

  }

}

#endif // JINGLEAUDIORTP_H__
