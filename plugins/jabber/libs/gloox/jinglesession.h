/*
  Copyright (c) 2007-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef JINGLESESSION_H__
#define JINGLESESSION_H__

#include "stanzaextension.h"
#include "tag.h"
#include "iqhandler.h"

#include <string>

namespace gloox
{

  class ClientBase;

  /**
   * @brief The namespace containing Jingle-related (XEP-0166 et. al.) classes.
   *
   * @author Jakob Schroeter <js@camaya.net>
   * @since 1.0
   */
  namespace Jingle
  {

    class Description;
    class Transport;
    class SessionHandler;
    class Content;

    /**
     * A list of Jingle Content Types.
     */
    typedef std::list<const Content*> ContentList;

    /**
     * Jingle Session actions.
     */
    enum Action
    {
      ContentAccept,                /**< */
      ContentAdd,                   /**< */
      ContentModify,                /**< */
      ContentRemove,                /**< */
      ContentReplace,               /**< */
      SessionAccept,                /**< */
      SessionInfo,                  /**< */
      SessionInitiate,              /**< */
      SessionTerminate,             /**< */
      TransportInfo,                /**< */
      InvalidAction                 /**< */
    };

    /**
     * @brief This is an implementation of a Jingle Session (XEP-0166).
     *
     * Beware! The classes in the Jingle namespace implement the signaling part of Jingle only.
     * At this point, there is no support for actually establishing any connection to a remote entity,
     * nor for transfering any media in any way whatsoever.
     *
     * XEP Version: 0.25
     * @author Jakob Schroeter <js@camaya.net>
     * @since 1.0
     */
    class GLOOX_API Session : public IqHandler
    {

      public:
        /**
         * Session state.
         */
        enum State
        {
          Ended,                    /**< */
          Pending,                  /**< */
          Active                    /**< */
        };

        /**
         * @brief This is an implementation of XEP-0166 (Jingle) as a StanzaExtension.
         *
         * XEP Version: 0.25
         * @author Jakob Schroeter <js@camaya.net>
         * @since 1.1
         */
        class Jingle : public StanzaExtension
        {

          friend class Session;

          public:
            /**
             * Constructs a new object from the given Tag.
             * @param tag The Tag to parse.
             */
            Jingle( const Tag* tag = 0 );

            /**
             * Virtual Destructor.
             */
            virtual ~Jingle();

            /**
             *
             */
            void addContent( const Content* content, Action action );

            /**
             *
             */
            const std::string& sid() const { return m_sid; }

            /**
             *
             */
            void setInitiator( const std::string& initiator ) { m_initiator = initiator; }

            /**
             *
             */
            void setResponder( const std::string& responder ) { m_responder = responder; }

            /**
             *
             */
            Action action() const { return m_action; }

            // reimplemented from StanzaExtension
            virtual const std::string& filterString() const;

            // reimplemented from StanzaExtension
            virtual StanzaExtension* newInstance( const Tag* tag ) const
            {
              return new Jingle( tag );
            }

            // reimplemented from StanzaExtension
            virtual Tag* tag() const;

            // reimplemented from StanzaExtension
            virtual StanzaExtension* clone() const;

#ifdef JINGLE_TEST
          public:
#else
          private:
#endif
            /**
             * Constructs a new object and fills it according to the parameters.
             * @param action The Action to carry out.
             */
            Jingle( Action action, const ContentList& contents, const std::string& sid );

            /**
             *
             */
            Jingle( Action action, const Content* content, const std::string& sid );

            Action m_action;
            std::string m_sid;
            std::string m_initiator;
            std::string m_responder;
            ContentList m_contents;

        };

        /**
         */
        Session( ClientBase* parent, const JID& callee, SessionHandler* jsh );

        /**
         * Virtual Destructor.
         */
        virtual ~Session();

        /**
         *
         */
        bool initiate();

        /**
         *
         */
        bool terminate();

        /**
         *
         */
        State state() const { return m_state; }

        /**
         * Use this function to add a new Jingle Content Type to
         * this Session.
         * A Content Type consists of a Description (Application Format), e.g. AudioRTP,
         * and a Transport, e.g. ICEUDP.
         * This enables the Session to offer that Content Type to the peer.
         * @param content A ContentType to add.
         */
        void addContent( Content* content )
        {
          if( content )
            m_contents.push_back( content );
        }

        // reimplemented from IqHandler
        virtual bool handleIq( const IQ& iq );

        // reimplemented from IqHandler
        virtual void handleIqID( const IQ& iq, int context );

#ifdef JINGLE_TEST
      public:
#else
      private:
#endif
        ClientBase* m_parent;
        State m_state;
        JID m_callee;
        SessionHandler* m_handler;
        ContentList m_contents;
        std::string m_sid;
        bool m_valid;

    };

  }

}

#endif // JINGLESESSION_H__
