/*
  Copyright (c) 2008-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef JINGLEVIDEORTP_H__
#define JINGLEVIDEORTP_H__

#include "jingledescription.h"
#include "gloox.h"

#include <string>

namespace gloox
{

  namespace Jingle
  {

    /**
     * @brief An abstraction of the signaling part of Jingle Video via RTP (XEP-0180).
     *
     * XEP Version: 0.11
     * @author Jakob Schroeter <js@camaya.net>
     * @since 1.0
     */
    class VideoRTP : public Description
    {
      public:
        /**
         * Creates a new wrapper for the Video via RTP Application Format defined in XEP-0180.
         * @param payload A list of acceptable Payload Types.
         */
        VideoRTP( const PayloadList& payload )
          : Description( payload )
        {}

        /**
         * Virtual destructor.
         */
        virtual ~VideoRTP() {}

        // reimplemented from Description
        virtual const std::string& xmlns() const { return XMLNS_JINGLE_VIDEO_RTP; }

    };

  }

}

#endif // JINGLEVIDEORTP_H__
