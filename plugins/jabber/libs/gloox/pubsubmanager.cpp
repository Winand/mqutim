/*
  Copyright (c) 2007-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/

#include "pubsubmanager.h"
#include "clientbase.h"
#include "dataform.h"
#include "iq.h"
#include "pubsub.h"
#include "pubsubeventhandler.h"
#include "pubsubresulthandler.h"
#include "pubsubitem.h"
#include "shim.h"
#include "util.h"
#include "error.h"

#include <iostream>

namespace gloox
{

  namespace PubSub
  {

    static const std::string
      XMLNS_PUBSUB_NODE_CONFIG = "http://jabber.org/protocol/pubsub#node_config",
      XMLNS_PUBSUB_SUBSCRIBE_OPTIONS = "http://jabber.org/protocol/pubsub#subscribe_options";

    /**
     * Finds the associated PubSubFeature for a feature tag 'type' attribute,
     * as received from a disco info query on a pubsub service (XEP-0060 sect. 10).
     * @param feat Feature string to search for.
     * @return the associated PubSubFeature.
     */
/*    static PubSubFeature featureType( const std::string& str )
    {
      static const char* values [] = {
        "collections",
        "config-node",
        "create-and-configure",
        "create-nodes",
        "delete-any",
        "delete-nodes",
        "get-pending",
        "instant-nodes",
        "item-ids",
        "leased-subscription",
        "manage-subscriptions",
        "meta-data",
        "modify-affiliations",
        "multi-collection",
        "multi-subscribe",
        "outcast-affiliation",
        "persistent-items",
        "presence-notifications",
        "publish",
        "publisher-affiliation",
        "purge-nodes",
        "retract-items",
        "retrieve-affiliations",
        "retrieve-default",
        "retrieve-items",
        "retrieve-subscriptions",
        "subscribe",
        "subscription-options",
        "subscription-notifications",
        "owner",
        "event",
      };
      return static_cast< PubSubFeature >( util::lookup2( str, values ) );
    }
*/

    static const char* subscriptionValues[] = {
      "none", "subscribed", "pending", "unconfigured"
    };

    static inline SubscriptionType subscriptionType( const std::string& subscription )
    {
      return (SubscriptionType)util::lookup( subscription, subscriptionValues );
    }

    static inline const std::string subscriptionValue( SubscriptionType subscription )
    {
      return util::lookup( subscription, subscriptionValues );
    }

    static const char* affiliationValues[] = {
      "none", "publisher", "owner", "outcast"
    };

    static inline AffiliationType affiliationType( const std::string& affiliation )
    {
      return (AffiliationType)util::lookup( affiliation, affiliationValues );
    }

    static inline const std::string affiliationValue( AffiliationType affiliation )
    {
      return util::lookup( affiliation, affiliationValues );
    }

    
    static EventType eventType( const std::string& event )
    {
    static const char* values[] = {
    "collection",
    "configuration",
    "delete",
    "items",
    "purge",
    "subscription"
  };
    return (EventType)util::lookup( event, values );
  }



    void Manager::handleMessage( const Message& msg, MessageSession* )
    {
    	const PubSub *pubsub = msg.findExtension<PubSub>(ExtPubSub);
    	if(pubsub)
    	{
      		EventHandlerList::iterator ith = m_eventHandlerList.begin();
       		for( ; ith != m_eventHandlerList.end(); ith++ )
       		{
    			if(pubsub->items().size())
    			{
				const ItemList &items = pubsub->items();
				Item *item = items.front();
				(*ith)->handleItemPublication( msg.from(), pubsub->node(), item->id(), item->payload() );
    			}
       		}
    	}
      /*const Tag* event = msg.findChild( "event", XMLNS, XMLNS_PUBSUB_EVENT );
      if( !event || m_eventHandlerList.empty() )
      return;

      const JID& service = msg.from();
      const TagList& events = event->children();
      EventType type;
      EventHandlerList::iterator ith = m_eventHandlerList.begin();

      // in case an event may contain several different notifications
      TagList::const_iterator it = events.begin();
      for( ; it != events.end(); ++it )
      {
        type = eventType( (*it)->name() );
        for( ; ith != m_eventHandlerList.end(); ++it )
        {
          const std::string& node = (*it)->findAttribute( "node" );
          switch( type )
          {
          case EventCollection:
          {
            const Tag* x = (*it)->findChild( "x" );
            const DataForm* df = x ? new DataForm( x ) : 0;
            (*ith)->handleNodeCreation( service, node, df );
            delete df;
            break;
          }
          case EventConfigure:
          {
            const Tag* x = (*it)->findChild( "x" );
            const DataForm* df = x ? new DataForm( x ) : 0;
            (*ith)->handleConfigurationChange( service, node, df );
            delete df;
            break;
          }
          case EventDelete:
          {
            (*ith)->handleNodeRemoval( service, node );
            break;
          }
          case EventItems:
          {
            const Tag* items = (*it)->findChild( "items" );
            const Tag* item = items->findChild( "item" );
              // This is the collection node responsible for the notification,
              // in case of a subscription type of 'items'. Currently unused.
            const Tag* headers = item->findChild( "headers", "xmlns", "shim" );
            const std::string& id= item->findAttribute( "id" );
            (*ith)->handleItemPublication( service, node, id, item );
            break;
          }
          case EventPurge:
          {
            (*ith)->handleNodePurge( service, node );
            break;
          }
          case EventSubscription:
          {
            const std::string& jid  = (*it)->findAttribute( "jid" );
            const std::string& sub  = (*it)->findAttribute( "subscription" );
            const Tag* body = event->findChild( "body" );
            (*ith)->handleSubscriptionChange( service, jid, node,
            body ? body->cdata() : EmptyString,
            subscriptionType( sub ) );
            break;
          }
        }
      }
    }*/
  }


    // ---- Manager::PubSubOwner ----
    Manager::PubSubOwner::PubSubOwner( TrackContext context )
      : StanzaExtension( ExtPubSubOwner ), m_ctx( context ), m_form( 0 )
    {
    }

    Manager::PubSubOwner::PubSubOwner( const Tag* tag )
      : StanzaExtension( ExtPubSubOwner ), m_ctx( InvalidContext ), m_form( 0 )
    {
      const Tag* d = tag->findTag( "pubsub/delete" );
      if( d )
      {
        m_ctx = DeleteNode;
        m_node = d->findAttribute( "node" );
        return;
      }
      const Tag* p = tag->findTag( "pubsub/purge" );
      if( p )
      {
        m_ctx = PurgeNodeItems;
        m_node = p->findAttribute( "node" );
        return;
      }
      const Tag* c = tag->findTag( "pubsub/configure" );
      if( c )
      {
        m_ctx = SetNodeConfig;
        m_node = c->findAttribute( "node" );
        if( c->hasChild( "x", "xmlns", XMLNS_X_DATA ) )
        {
          m_ctx = GetNodeConfig;
          m_form = new DataForm( c->findChild( "x", "xmlns", XMLNS_X_DATA ) );
        }
        return;
      }
      const Tag* de = tag->findTag( "pubsub/default" );
      if( de )
      {
        m_ctx = DefaultNodeConfig;
        return;
      }
      const Tag* s = tag->findTag( "pubsub/subscriptions" );
      if( s )
      {
        m_ctx = GetSubscriberList;
        m_node = s->findAttribute( "node" );
        const TagList& l = s->children();
        TagList::const_iterator it =l.begin();
        for( ; it != l.end(); ++it )
        {
          if( (*it)->name() == "subscription" )
          {
            Subscriber sub( (*it)->findAttribute( "jid" ),
                            subscriptionType( (*it)->findAttribute( "subscription" ) ),
                            (*it)->findAttribute( "subid" ) );
            m_subList.push_back( sub );
          }
        }
        return;
      }
      const Tag* a = tag->findTag( "pubsub/affiliations" );
      if( a )
      {
        m_ctx = GetAffiliateList;
        m_node = a->findAttribute( "node" );
        const TagList& l = a->children();
        TagList::const_iterator it =l.begin();
        for( ; it != l.end(); ++it )
        {
          if( (*it)->name() == "affiliation" )
          {
            Affiliate aff( (*it)->findAttribute( "jid" ),
                            affiliationType( (*it)->findAttribute( "affiliation" ) ) );
            m_affList.push_back( aff );
          }
        }
        return;
      }
    }

    Manager::PubSubOwner::~PubSubOwner()
    {
      delete m_form;
    }

    const std::string& Manager::PubSubOwner::filterString() const
    {
      static const std::string filter = "/iq/pubsub[@xmlns='" + XMLNS_PUBSUB_OWNER + "']";
      return filter;
    }

    Tag* Manager::PubSubOwner::tag() const
    {
      if( m_ctx == InvalidContext )
        return 0;

      Tag* t = new Tag( "pubsub" );
      t->setXmlns( XMLNS_PUBSUB_OWNER );

      switch( m_ctx )
      {
        case DeleteNode:
        {
          new Tag( "delete", "node", m_node );
          break;
        }
        case PurgeNodeItems:
        {
          new Tag( "purge", "node", m_node );
          break;
        }
        case GetNodeConfig:
        case SetNodeConfig:
        {
          Tag* c = new Tag( t, "configure" );
          c->addAttribute( "node", m_node );
          if( m_form )
            c->addChild( m_form->tag() );
          break;
        }
        case GetSubscriberList:
        case SetSubscriberList:

        {
          Tag* sub = new Tag( t, "subscriptions" );
          sub->addAttribute( "node", m_node );
          if( m_subList.size() )
          {
            Tag* s;
            SubscriberList::const_iterator it = m_subList.begin();
            for( ; it != m_subList.end(); ++it )
            {
              s = new Tag( sub, "subscription" );
              s->addAttribute( "jid", (*it).jid.full() );
              s->addAttribute( "subscription", util::lookup( (*it).type, subscriptionValues ) );
              if( !(*it).subid.empty() )
                s->addAttribute( "subid", (*it).subid );
            }
          }
          break;
        }
        case GetAffiliateList:
        case SetAffiliateList:
        {
          Tag* aff = new Tag( t, "affiliations" );
          aff->addAttribute( "node", m_node );
          if( m_affList.size() )
          {
            Tag* a;
            AffiliateList::const_iterator it = m_affList.begin();
            for( ; it != m_affList.end(); ++it )
            {
              a = new Tag( aff, "affiliation", "jid", (*it).jid.full() );
              a->addAttribute( "affiliation", util::lookup( (*it).type, affiliationValues ) );
            }
          }
          break;
        }
        case DefaultNodeConfig:
        {
          new Tag( t, "default" );
          break;
        }
        default:
          break;
      }

      return t;
    }
    // ---- ~Manager::PubSubOwner ----

    // ---- Manager::PubSub ----
    Manager::PubSub::PubSub( TrackContext context )
      : StanzaExtension( ExtPubSub ), m_ctx( context ), m_maxItems( 0 ),
        m_notify( false )
    {
      m_options.df = 0;
    }

    Manager::PubSub::PubSub( const Tag* tag )
      : StanzaExtension( ExtPubSub ), m_ctx( InvalidContext ),
        m_maxItems( 0 ), m_notify( false )
    {
      m_options.df = 0;
      if( !tag )
        return;
      ConstTagList l = tag->findTagList( "pubsub/subscriptions/subscription" );
      if( l.size() )
      {
        m_ctx = GetSubscriptionList;
        ConstTagList::const_iterator it = l.begin();
        for( ; it != l.end(); ++it )
        {
          const std::string& node = (*it)->findAttribute( "node" );
          const std::string& sub  = (*it)->findAttribute( "subscription" );
          SubscriptionInfo si;
          si.jid.setJID( (*it)->findAttribute( "jid" ) );
          si.type = subscriptionType( sub );
          m_subscriptionMap[node] = si;
        }
        return;
      }
      l = tag->findTagList( "pubsub/affiliations/affiliation" );
      if( l.size() )
      {
        m_ctx = GetAffiliationList;
        ConstTagList::const_iterator it = l.begin();
        for( ; it != l.end(); ++it )
        {
          const std::string& node = (*it)->findAttribute( "node" );
          const std::string& aff = (*it)->findAttribute( "affiliation" );
          m_affiliationMap[node] = affiliationType( aff );
        }
        return;
      }
      const Tag* s = tag->findTag( "pubsub/subscribe" );
      if( s )
      {
        m_ctx = Subscription;
        m_node = s->findAttribute( "node" );
        m_jid = s->findAttribute( "jid" );
      }
      const Tag* u = tag->findTag( "pubsub/unsubscribe" );
      if( u )
      {
        m_ctx = Unsubscription;
        m_node = u->findAttribute( "node" );
        m_jid = u->findAttribute( "jid" );
        m_subid = u->findAttribute( "subid" );
      }
      const Tag* o = tag->findTag( "pubsub/options" );
      if( o )
      {
        if( m_ctx == InvalidContext )
          m_ctx = GetSubscriptionOptions;
        m_jid.setJID( o->findAttribute( "jid" ) );
        m_options.node = o->findAttribute( "node" );
        m_options.df = new DataForm( o->findChild( "x", "xmlns", XMLNS_X_DATA ) );
      }
      const Tag* su = tag->findTag( "pubsub/subscription" );
      if( su )
      {
        SubscriptionInfo si;
        si.jid.setJID( su->findAttribute( "jid" ) );
        si.subid = su->findAttribute( "subid" );
        si.type = subscriptionType( su->findAttribute( "type" ) );
        m_subscriptionMap[su->findAttribute( "node" )] = si;
        return;
      }
      const Tag* i = tag->findChild( "items" );
      if( i )
      {
        m_ctx = RequestItems;
        m_node = i->findAttribute( "node" );
        m_subid = i->findAttribute( "subid" );
        m_maxItems = atoi( i->findAttribute( "max_items" ).c_str() );
        const TagList& l = i->children();
        TagList::const_iterator it = l.begin();
        for( ; it != l.end(); ++it )
          m_items.push_back( new Item( (*it) ) );
        return;
      }
      const Tag* p = tag->findTag( "pubsub/publish" );
      if( p )
      {
        m_ctx = PublishItem;
        m_node = p->findAttribute( "node" );
        const TagList& l = p->children();
        TagList::const_iterator it = l.begin();
        for( ; it != l.end(); ++it )
          m_items.push_back( new Item( (*it) ) );
        return;
      }
      const Tag* r = tag->findTag( "pubsub/retract" );
      if( r )
      {
        m_ctx = DeleteItem;
        m_node = r->findAttribute( "node" );
        m_notify = r->hasAttribute( "notify", "1" ) || r->hasAttribute( "notify", "true" );
        const TagList& l = p->children();
        TagList::const_iterator it = l.begin();
        for( ; it != l.end(); ++it )
          m_items.push_back( new Item( (*it) ) );
        return;
      }
      const Tag* c = tag->findTag( "pubsub/create" );
      if( c )
      {
        m_ctx = CreateNode;
        m_node = c->findAttribute( "node" );
        const Tag* config = tag->findTag( "pubsub/configure" );
        if( config && config->hasChild( "x", XMLNS_X_DATA ) )
          m_options.df = new DataForm( config->findChild( "x", XMLNS_X_DATA ) );
      }
    }

    Manager::PubSub::~PubSub()
    {
      delete m_options.df;
      util::clearList( m_items );
    }

    const std::string& Manager::PubSub::filterString() const
    {
      static const std::string filter = "/iq/pubsub[@xmlns='" + XMLNS_PUBSUB + "']"
      "|/message/event[@xmlns='" + XMLNS_PUBSUB_EVENT + "']";
      return filter;
    }

    Tag* Manager::PubSub::tag() const
    {
      if( m_ctx == InvalidContext )
        return 0;

      Tag* t = new Tag( "pubsub" );
      t->setXmlns( XMLNS_PUBSUB );

      if( m_ctx == GetSubscriptionList )
      {
        Tag* sub = new Tag( t, "subscriptions" );
        SubscriptionMap::const_iterator it = m_subscriptionMap.begin();
        for( ; it != m_subscriptionMap.end(); ++it )
        {
          Tag* s = new Tag( sub, "subscription" );
          s->addAttribute( "node", (*it).first );
          s->addAttribute( "jid", (*it).second.jid );
          s->addAttribute( "subscription", subscriptionValue( (*it).second.type ) );
        }
      }
      else if( m_ctx == GetAffiliationList )
      {

        Tag* aff = new Tag( t, "affiliations" );
        AffiliationMap::const_iterator it = m_affiliationMap.begin();
        for( ; it != m_affiliationMap.end(); ++it )
        {
          Tag* a = new Tag( aff, "affiliation" );
          a->addAttribute( "node", (*it).first );
          a->addAttribute( "affiliation", affiliationValue( (*it).second ) );
        }
      }
      else if( m_ctx == Subscription )
      {
        Tag* s = new Tag( t, "subscribe" );
        s->addAttribute( "node", m_node );
        s->addAttribute( "jid", m_jid.full() );
        if( m_options.df )
        {
          Tag* o = new Tag( t, "options" );
          o->addChild( m_options.df->tag() );
        }
      }
      else if( m_ctx == Unsubscription )
      {
        Tag* u = new Tag( t, "unsubscribe" );
        u->addAttribute( "node", m_node );
        u->addAttribute( "jid", m_jid.full() );
        u->addAttribute( "subid", m_subid );
      }
      else if( m_ctx == GetSubscriptionOptions
               || m_ctx == SetSubscriptionOptions
               || ( m_ctx == Subscription && m_options.df ) )
      {
        Tag* o = new Tag( t, "options" );
        o->addAttribute( "node", m_options.node );
        o->addAttribute( "jid", m_jid.full() );
        if( m_options.df )
          o->addChild( m_options.df->tag() );
      }
      else if( m_ctx == RequestItems )
      {
        Tag* i = new Tag( t, "items" );
        i->addAttribute( "node", m_node );
        if( m_maxItems )
          i->addAttribute( "max_items", m_maxItems );
        i->addAttribute( "subid", m_subid );
        ItemList::const_iterator it = m_items.begin();
        for( ; it != m_items.end(); ++it )
          i->addChild( (*it)->tag() );
      }
      else if( m_ctx == PublishItem )
      {
        Tag* p = new Tag( t, "publish" );
        p->addAttribute( "node", m_node );
        ItemList::const_iterator it = m_items.begin();
        for( ; it != m_items.end(); ++it )
          p->addChild( (*it)->tag() );
        if( m_options.df )
        {
          Tag* po = new Tag( "publish-options" );
          po->addChild( m_options.df->tag() );
        }
      }
      else if( m_ctx == DeleteItem )
      {
        Tag* r = new Tag( t, "retract" );
        r->addAttribute( "node", m_node );
        if( m_notify )
          r->addAttribute( "notify", "true" );
        ItemList::const_iterator it = m_items.begin();
        for( ; it != m_items.end(); ++it )
          r->addChild( (*it)->tag() );
      }
      else if( m_ctx == CreateNode )
      {
        Tag* c = new Tag( t, "create" );
        c->addAttribute( "node", m_node );
        Tag* config = new Tag( t, "configure" );
        if( m_options.df )
          config->addChild( m_options.df->tag() );
      }
      return t;
    }

    StanzaExtension* Manager::PubSub::clone() const
    {
      PubSub* p = new PubSub();
      p->m_affiliationMap = m_affiliationMap;
      p->m_subscriptionMap = m_subscriptionMap;
      p->m_ctx = m_ctx;

      p->m_options.node = m_options.node;
      p->m_options.df = m_options.df ? new DataForm( *(m_options.df) ) : 0;

      p->m_jid = m_jid;
      p->m_node = m_node;
      p->m_subid = m_subid;
      ItemList::const_iterator it = m_items.begin();
      for( ; it != m_items.end(); ++it )
        p->m_items.push_back( new Item( *(*it) ) );

      p->m_maxItems = m_maxItems;
      p->m_notify = m_notify;
      return p;
    }
    // ---- ~Manager::PubSub ----

    // ---- Manager ----
    Manager::Manager( ClientBase* parent )
      : m_parent( parent )
    {
      if( m_parent )
      {
        m_parent->registerStanzaExtension( new PubSub() );
        m_parent->registerStanzaExtension( new PubSubOwner() );
        m_parent->registerStanzaExtension( new SHIM() );
        m_parent->registerMessageHandler( this );
      }
    }

    const std::string& Manager::getSubscriptionsOrAffiliations( const JID& service,
                                                                ResultHandler* handler,
                                                                TrackContext context )
    {
      if( !m_parent || !handler || !service || context == InvalidContext )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( IQ::Get, service, id );
      iq.addExtension( new PubSub( context ) );

      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, context );
      return id;
    }

    const std::string& Manager::subscribe( const JID& service,
                                           const std::string& node,
                                           ResultHandler* handler,
                                           const JID& jid,
                                           SubscriptionObject type,
                                           int depth )
    {
      if( !m_parent || !handler || !service || node.empty() )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( IQ::Set, service, id );
      PubSub* ps = new PubSub( Subscription );
      ps->setJID( jid ? jid : m_parent->jid() );
      ps->setNode( node );
      if( type != SubscriptionNodes || depth != 1 )
      {
        DataForm* df = new DataForm( TypeSubmit );
        df->addField( DataFormField::TypeHidden, "FORM_TYPE", XMLNS_PUBSUB_SUBSCRIBE_OPTIONS );

        if( type == SubscriptionItems )
          df->addField( DataFormField::TypeNone, "pubsub#subscription_type", "items" );

        if( depth != 1 )
        {
          DataFormField* field = df->addField( DataFormField::TypeNone, "pubsub#subscription_depth" );
          if( depth == 0 )
            field->setValue( "all" );
          else
           field->setValue( util::int2string( depth ) );
        }
        ps->setJID( jid );
        ps->setOptions( node, df );
      }
      iq.addExtension( ps  );

      m_resultHandlerTrackMap[id] = handler;
      m_nopTrackMap[id] = node;
      m_parent->send( iq, this, Subscription );
      return id;
    }

    const std::string& Manager::unsubscribe( const JID& service,
                                              const std::string& node,
                                              const std::string& subid,
                                              ResultHandler* handler,
                                              const JID& jid )
    {
      if( !m_parent || !handler || !service )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( IQ::Set, service, id );
      PubSub* ps = new PubSub( Unsubscription );
      ps->setNode( node );
      ps->setJID( jid ? jid : m_parent->jid() );
      ps->setSubscriptionID( subid );
      iq.addExtension( ps );

      m_resultHandlerTrackMap[id] = handler;
      // FIXME? need to track info for handler
      m_parent->send( iq, this, Unsubscription );
      return id;
    }

    const std::string& Manager::subscriptionOptions( TrackContext context,
                                       const JID& service,
                                       const JID& jid,
                                       const std::string& node,
                                       ResultHandler* handler,
                                       DataForm* df )
    {
      if( !m_parent || !handler || !service )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( df ? IQ::Set : IQ::Get, service, id );
      PubSub* ps = new PubSub( context );
      ps->setJID( jid ? jid : m_parent->jid() );
      ps->setOptions( node, df );
      iq.addExtension( ps );

      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, context );
      return id;
    }

    const std::string& Manager::requestItems( const JID& service,
                                              const std::string& node,
                                              const std::string& subid,
                                              int maxItems,
                                              ResultHandler* handler )
    {
      if( !m_parent || !service || !handler )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( IQ::Get, service, id );
      PubSub* ps = new PubSub( RequestItems );
      ps->setNode( node );
      ps->setSubscriptionID( subid );
      ps->setMaxItems( maxItems );
      iq.addExtension( ps );

      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, RequestItems );
      return id;
    }

    const std::string& Manager::requestItems( const JID& service,
                                              const std::string& node,
                                              const std::string& subid,
                                              const ItemList& items,
                                              ResultHandler* handler )
    {
      if( !m_parent || !service || !handler )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( IQ::Get, service, id );
      PubSub* ps = new PubSub( RequestItems );
      ps->setNode( node );
      ps->setSubscriptionID( subid );
      ps->setItems( items );
      iq.addExtension( ps );

      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, RequestItems );
      return id;
    }

    const std::string& Manager::publishItem( const JID& service,
                                            const std::string& node,
                                            ItemList& items,
                                            DataForm* options,
                                            ResultHandler* handler )
    {
      if( !m_parent || !handler )
      {
        util::clearList( items );
        return EmptyString;
      }

      const std::string& id = m_parent->getID();
      IQ iq( IQ::Set, service, id );
      PubSub* ps = new PubSub( PublishItem );
      ps->setNode( node );
      ps->setItems( items );
      ps->setOptions( EmptyString, options );
      iq.addExtension( ps );

      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, PublishItem );
      return id;
    }

    const std::string& Manager::deleteItem( const JID& service,
                                            const std::string& node,
                                            const ItemList& items,
                                            bool notify,
                                            ResultHandler* handler )
    {
      if( !m_parent || !handler || !service )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( IQ::Set, service, id );
      PubSub* ps = new PubSub( DeleteItem );
      ps->setNode( node );
      ps->setItems( items );
      ps->setNotify( notify );
      iq.addExtension( ps );

      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, DeleteItem );
      return id;
    }

    const std::string& Manager::createNode( const JID& service,
                                            const std::string& node,
                                            DataForm* config,
                                            ResultHandler* handler )
    {
      if( !m_parent || !handler || !service || node.empty() )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( IQ::Set, service, id );
      PubSub* ps = new PubSub( CreateNode );
      ps->setNode( node );
      ps->setOptions( EmptyString, config );
      iq.addExtension( ps );

      m_nopTrackMap[id] = node;
      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, CreateNode );
      return id;
    }

    const std::string& Manager::deleteNode( const JID& service,
                                            const std::string& node,
                                            ResultHandler* handler )
    {
      if( !m_parent || !handler || !service || node.empty() )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( IQ::Set, service, id );
      PubSubOwner* pso = new PubSubOwner( DeleteNode );
      pso->setNode( node );
      iq.addExtension( pso );

      m_nopTrackMap[id] = node;
      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, DeleteNode );
      return id;
    }

    const std::string& Manager::getDefaultNodeConfig( const JID& service,
                                                      NodeType type,
                                                      ResultHandler* handler )
    {
      if( !m_parent || !handler || !service )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( IQ::Get, service, id );
      PubSubOwner* pso = new PubSubOwner( DefaultNodeConfig );
      if( type == NodeCollection )
      {
        DataForm* df = new DataForm( TypeSubmit );
        df->addField( DataFormField::TypeHidden, "FORM_TYPE", XMLNS_PUBSUB_NODE_CONFIG );
        df->addField( DataFormField::TypeNone, "pubsub#node_type", "collection" );
        pso->setConfig( df );
      }
      iq.addExtension( pso );

      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, DefaultNodeConfig );
      return id;
    }

    const std::string& Manager::nodeConfig( const JID& service,
                                            const std::string& node,
                                            DataForm* config,
                                            ResultHandler* handler )
    {
      if( !m_parent || !handler || !service || node.empty() )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( config ? IQ::Set : IQ::Get, service, id );
      PubSubOwner* pso = new PubSubOwner( config ? SetNodeConfig : GetNodeConfig );
      pso->setNode( node );
      if( config )
        pso->setConfig( config );
      iq.addExtension( pso );

      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, config ? SetNodeConfig : GetNodeConfig );
      return id;
    }

    const std::string& Manager::subscriberList( TrackContext ctx,
                                                const JID& service,
                                                const std::string& node,
                                                const SubscriberList& subList,
                                                ResultHandler* handler )
    {
      if( !m_parent || !handler || !service || node.empty() )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( ctx == SetSubscriberList ? IQ::Set : IQ::Get, service, id );
      PubSubOwner* pso = new PubSubOwner( ctx );
      pso->setNode( node );
      pso->setSubscriberList( subList );
      iq.addExtension( pso );
      m_nopTrackMap[id] = node;

      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, ctx );
      return id;
    }

    const std::string& Manager::affiliateList( TrackContext ctx,
                                               const JID& service,
                                               const std::string& node,
                                               const AffiliateList& affList,
                                               ResultHandler* handler )
    {
      if( !m_parent || !handler || !service || node.empty() )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( ctx == SetAffiliateList ? IQ::Set : IQ::Get, service, id );
      PubSubOwner* pso = new PubSubOwner( ctx );
      pso->setNode( node );
      pso->setAffiliateList( affList );
      m_nopTrackMap[id] = node;
      iq.addExtension( pso );

      m_resultHandlerTrackMap[id] = handler;
      m_parent->send( iq, this, ctx );
      return id;
    }

    const std::string& Manager::purgeNode( const JID& service,
                                           const std::string&  node,
                                           ResultHandler* handler  )
    {
      if( !m_parent || !handler || !service || node.empty() )
        return EmptyString;

      const std::string& id = m_parent->getID();
      IQ iq( IQ::Set, service, id );
      PubSubOwner* pso = new PubSubOwner( PurgeNodeItems );
      pso->setNode( node );
      iq.addExtension( pso );

      m_resultHandlerTrackMap[id] = handler;
      m_nopTrackMap[id] = node;
      m_parent->send( iq, this, PurgeNodeItems );
      return id;
    }

    /**
     * @todo Track context info for Unsubscription result.
     * @todo
     */
    void Manager::handleIqID( const IQ& iq, int context )
    {
      const JID& service = iq.from();
//       const Tag* query = iq.query();
      const std::string& id = iq.id();

      ResultHandlerTrackMap::iterator ith = m_resultHandlerTrackMap.find( id );
      if( ith == m_resultHandlerTrackMap.end() )
        return;

      ResultHandler* rh = (*ith).second;

      switch( iq.subtype() )
      {
        case IQ::Error:
        case IQ::Result:
        {
          const Error* error = iq.error();
          switch( context )
          {
            case Subscription:
            {
              const PubSub* ps = iq.findExtension<PubSub>( ExtPubSub );
              if( ps )
              {
                SubscriptionMap sm = ps->subscriptions();
                SubscriptionMap::const_iterator it = sm.begin();
                rh->handleSubscriptionResult( service, (*it).first,
                                              (*it).second.subid,
                                              (*it).second.jid,
                                              (*it).second.type,
                                              error );
              }
              break;
            }
            case Unsubscription:
            {
/*
              FIXME: info tracking for subscription ID...

              SubscriptionOperationTrackMap::iterator it = m_sopTrackMap.find( id );
              if( it != m_sopTrackMap.end() )
              {
                rh->handleUnsubscriptionResult( service, (*it).second.node,
                                                         (*it).second.sid,
                                                         (*it).second.jid );
                m_sopTrackMap.erase( it );
              }
              break;
*/
            }
            case GetSubscriptionList:
            {
              const PubSub* ps = iq.findExtension<PubSub>( ExtPubSub );
              if( !ps )
                return;

              rh->handleSubscriptions( service,
                                       ps->subscriptions(),
                                       error );
              break;
            }
            case GetAffiliationList:
            {
              const PubSub* ps = iq.findExtension<PubSub>( ExtPubSub );
              if( !ps )
                return;

              rh->handleAffiliations( service,
                                      ps->affiliations(),
                                      error );
              break;
            }
            case RequestItems:
            {
              const PubSub* ps = iq.findExtension<PubSub>( ExtPubSub );
              if( !ps )
                return;

              rh->handleItems( service, ps->node(),
                               ps->items(), error );
              break;
            }
            case PublishItem:
            {
              const PubSub* ps = iq.findExtension<PubSub>( ExtPubSub );
              if( ps && ps->items().size())
              {
                const ItemList& il = ps->items();
                ItemList::const_iterator it = il.begin();
                for( ; it != il.end(); ++it )
                {
                  rh->handleItemPublication( service, ps->node(),
                                             (*it)->id(), error );
                }
              }
              break;
            }
            case DeleteItem:
            {
              const PubSub* ps = iq.findExtension<PubSub>( ExtPubSub );
              if( ps )
              {
                rh->handleItemDeletion( service,
                                        ps->node(),
                                        ps->items(),
                                        error );
              }
              break;
            }
            case DefaultNodeConfig:
            {
              const PubSubOwner* pso = iq.findExtension<PubSubOwner>( ExtPubSubOwner );
              if( pso )
              {
                rh->handleDefaultNodeConfig( service,
                                             pso->config(),
                                             error );
              }
              break;
            }
            case GetSubscriptionOptions:
            case GetSubscriberList:
            case SetSubscriberList:
            case GetAffiliateList:
            case SetAffiliateList:
            case GetNodeConfig:
            case SetNodeConfig:
            case CreateNode:
            case DeleteNode:
            case PurgeNodeItems:
            {
              switch( context )
              {
                case GetSubscriptionOptions:
                {
                  const PubSub* ps = iq.findExtension<PubSub>( ExtPubSub );
                  if( ps )
                  {
                    rh->handleSubscriptionOptions( service,
                                                   ps->jid(),
                                                   ps->node(),
                                                   ps->options(),
                                                   error );
                  }
                  break;
                }
//                 case GetSubscriberList:
//                 {
//                   const PubSub* ps = iq.findExtension<PubSub>( ExtPubSub );
//                   if( ps )
//                   {
//                     rh->handleSubscribers( service, ps->node(), ps->subscriptions() );
//                   }
//                   break;
//                 }
                case SetSubscriptionOptions:
                case SetSubscriberList:
                case SetAffiliateList:
                case SetNodeConfig:
                case CreateNode:
                case DeleteNode:
                case PurgeNodeItems:
                {
                  NodeOperationTrackMap::iterator it = m_nopTrackMap.find( id );
                  if( it != m_nopTrackMap.end() )
                  {
                    const std::string& node = (*it).second;
                    switch( context )
                    {
                      case SetSubscriptionOptions:
                        rh->handleSubscriptionOptionsResult( service, JID( /* FIXME */ ), node, error );
                        break;
                      case SetSubscriberList:
                        rh->handleSubscribersResult( service, node, 0, error );
                        break;
                      case SetAffiliateList:
                        rh->handleAffiliatesResult( service, node, 0, error );
                        break;
                      case SetNodeConfig:
                        rh->handleNodeConfigResult( service, node, error );
                        break;
                      case CreateNode:
                        rh->handleNodeCreation( service, node, error );
                        break;
                      case DeleteNode:
                        rh->handleNodeDeletion( service, node, error );
                        break;
                      case PurgeNodeItems:
                        rh->handleNodePurge( service, node, error );
                        break;
                    }
                    m_nopTrackMap.erase( it );
                  }
                  break;
                }
                case GetAffiliateList:
                {
//                   const PubSub

                 /* const TagList& affiliates = query->children();
                  AffiliateList affList;
                  TagList::const_iterator it = affiliates.begin();
                  for( ; it != affiliates.end(); ++it )
                  {
                    Affiliate aff( (*it)->findAttribute( "jid" ),
                                   affiliationType( (*it)->findAttribute( "affiliation" ) ) );
                    affList.push_back( aff );
                  }
                  rh->handleAffiliates( service, query->findAttribute( "node" ), &affList );
                 */
                  break;
                }
                case GetNodeConfig:
                {
                  const PubSubOwner* pso = iq.findExtension<PubSubOwner>( ExtPubSubOwner );
                  if( pso )
                  {
                    rh->handleNodeConfig( service,
                                          pso->node(),
                                          pso->config(),
                                          error );
                  }
                  break;
                }
                default:
                  break;
              }

              break;
            }
          }
          break;
        }
        default:
          break;
      }

    }

  }

}

