/*
  Copyright (c) 2008-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef JINGLEDESCRIPTION_H__
#define JINGLEDESCRIPTION_H__


#include "jingleplugin.h"
#include "gloox.h"

namespace gloox
{

  class Tag;

  namespace Jingle
  {

    /**
     * @brief An abstraction of a Jingle Description (Application Format).
     *
     * You should not need to use this class directly. Use derived classes instead
     * (AudioRTP, VideoRTP). See
     * @link gloox::Jingle::Session Jingle::Session @endlink for more info on Jingle.
     *
     * @author Jakob Schroeter <js@camaya.net>
     * @since 1.0
     */
    class Description : public Plugin
    {
      public:
        /**
         * @brief An abstraction of the &let;payload-type&gt; element used as child of
         * the &lt;description&gt; element.
         *
         * @author Jakob Schroeter <js@camaya.net>
         * @since 1.0
         */
        class Payload
        {
          public:
            /**
             * Creates a new Payload object from the given attributes and parameters.
             * @param attribs A map of name/value pairs added as XML attributes to the
             * &lt;payload-type&gt; element. See the respective XEP for valid values.
             * @param parameters A map of name/value pairs added as &lt;parameter&gt; child
             * to the &lt;payload-type&gt; element. See the respective XEP and codec specs
             * for valid values.
             */
            Payload( const StringMap& attribs, const StringMap& parameters )
              : m_attribs( attribs ), m_parameters( parameters )
            {}

            /**
             * Destructor.
             */
            ~Payload() {}

            /**
             * Creates a Tag representation of the object.
             * @return A Tag representation of the object. The caller is responsible for
             * deleting the Tag.
             */
            Tag* tag() const;

          private:
            StringMap m_attribs;
            StringMap m_parameters;

        };

        /**
         * A list of payloads.
         */
        typedef std::list<const Description::Payload*> PayloadList;

        /**
         * Constructs a new Description (Application Format), adding the given
         * payloads.
         * @param payload A list payloads to include.
         */
        Description( const PayloadList& payload )
          : m_payload( payload )
        {}

        /**
         * Virtual destructor.
         */
        virtual ~Description();

        // reimplemented from Plugin
        virtual const std::string& filterString() const;

        // reimplemented from Plugin
        virtual Tag* tag() const;

      protected:
        /**
         * Returns the Application Format's namespace.
         * @return The namespace being handled by the Application Format.
         */
        virtual const std::string& xmlns() const = 0;

      private:
        PayloadList m_payload;

    };

  }

}

#endif // JINGLEDESCRIPTION_H__
