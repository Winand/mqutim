/*
  Copyright (c) 2007-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#include "jinglesession.h"

#include "clientbase.h"
#include "jinglecontent.h"
#include "jingledescription.h"
#include "jingletransport.h"
#include "jinglesessionhandler.h"
#include "tag.h"
#include "util.h"

namespace gloox
{

  namespace Jingle
  {

    static const char* actionValues [] = {
      "content-accept",
      "content-add",
      "content-modify",
      "content-remove",
      "content-replace",
      "session-accept",
      "session-info",
      "session-initiate",
      "session-terminate",
      "transport-info"
    };

    static inline Action actionType( const std::string& type )
    {
      return (Action)util::lookup( type, actionValues );
    }

    // ---- Session::Jingle ----
    Session::Jingle::Jingle( Action action, const ContentList& contents, const std::string& sid )
      : StanzaExtension( ExtJingle ), m_action( action ), m_sid( sid ), m_contents( contents )
    {
    }

    Session::Jingle::Jingle( Action action, const Content* content, const std::string& sid )
      : StanzaExtension( ExtJingle ), m_action( action ), m_sid( sid )
    {
      if( content )
        m_contents.push_back( content );
    }

    Session::Jingle::Jingle( const Tag* tag )
      : StanzaExtension( ExtJingle ), m_action( InvalidAction )
    {
    }

      Session::Jingle::~Jingle()
    {
    }

    const std::string& Session::Jingle::filterString() const
    {
      static const std::string filter = "/iq/jingle[@xmlns='" + XMLNS_JINGLE + "']";
      return filter;
    }

    Tag* Session::Jingle::tag() const
    {
      if( m_action == InvalidAction || m_sid.empty() || m_initiator.empty() )
        return 0;

      Tag* t = new Tag( "jingle" );
      t->setXmlns( XMLNS_JINGLE );
      t->addAttribute( "action", util::lookup( m_action, actionValues ) );
      t->addAttribute( "initiator", m_initiator );
      if( !m_responder.empty() )
        t->addAttribute( "responder", m_responder );
      t->addAttribute( "sid", m_sid );

      return t;
    }

    StanzaExtension* Session::Jingle::clone() const
    {
      Jingle* j = new Jingle();
      j->m_action = m_action;
      j->m_sid = m_sid;
      j->m_initiator = m_initiator;
      j->m_responder = m_responder;

      ContentList::const_iterator it = m_contents.begin();
      for( ; it != m_contents.end(); ++it )
        j->m_contents.push_back( new Content( *(*it) ) );

      return j;
    }
    // ---- ~Session::Jingle ----

    // ---- Session ----
    Session::Session( ClientBase* parent, const JID& callee, SessionHandler* jsh )
      : m_parent( parent ), m_state( Ended ), m_handler( jsh ), m_valid( false )
    {
      if( !m_parent || !m_handler )
        return;

      m_parent->registerStanzaExtension( new Jingle() );

      m_valid = true;
    }

    Session::~Session()
    {
      if( m_parent )
        m_parent->removeIDHandler( this );

      util::clearList( m_contents );
    }

    bool Session::initiate()
    {
      if( !m_valid || !m_parent || !m_contents.empty() )
        return false;

      m_state = Pending;
      IQ init( IQ::Set, m_callee, m_parent->getID() );
      init.addExtension( new Jingle( SessionInitiate, m_contents, m_sid ) );
      m_parent->send( init, this, SessionInitiate );

      ContentList::const_iterator it = m_contents.begin();
      for( ; it != m_contents.end(); ++it )
      {
        IQ t( IQ::Set, m_callee, m_parent->getID() );
        t.addExtension( new Jingle( TransportInfo, (*it), m_sid ) );
        m_parent->send( t, this, TransportInfo );
      }

      return true;
    }

    bool Session::terminate()
    {
      if( !m_valid )
        return false;

      m_state = Ended;

      return true;
    }

    bool Session::handleIq( const IQ& iq )
    {
      const Jingle* j = iq.findExtension<Jingle>( ExtJingle );
      if( !j || j->sid() != m_sid )
        return false;

      switch( j->action() )
      {
        default:
          break;
      }

      return true;
    }

    void Session::handleIqID( const IQ& iq, int context )
    {
      if( iq.subtype() == IQ::Result )
      {
        switch( context )
        {
          default:
            break;
        }
      }
      else if( iq.subtype() == IQ::Error )
      {
      }
    }

  }

}
