/*
  Copyright (c) 2007-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/

#ifndef PUBSUBEVENTHANDLER_H__
#define PUBSUBEVENTHANDLER_H__

#include "macros.h"
#include "pubsub.h"

#include <string>

namespace gloox
{

  class JID;
  class Tag;
  class DataForm;

  namespace PubSub
  {

    /**
     * Handler for PubSub event notifications.
     *
     * @author Vincent Thomasset
     */
    class GLOOX_API EventHandler
    {
      public:
        /**
         * Default virtual destructor.
         */
        virtual ~EventHandler() {}

        /**
         * Receives notification of node creations.
         * @param service Service hosting the new node.
         * @param node Node ID of the new node.
         * @param config Optional DataForm of the new configuration (may be null).
         */
        virtual void handleNodeCreation( const JID& service,
                                         const std::string& node,
                                         const DataForm* config ) = 0;

        /**
         * Receives notification of node destructions.
         * @param service Service hosting the new node.
         * @param node Node ID of the new node.
         */
        virtual void handleNodeRemoval( const JID& service,
                                        const std::string& node ) = 0;

        /**
         * Receives notifications of item publications.
         * @param service Service hosting the parent node.
         * @param node Node ID of the parent node.
         * @param item Item ID of the new Item.
         * @param entry Payload of the newly published item (may be null).
         */
        virtual void handleItemPublication( const JID& service,
                                            const std::string& node,
                                            const std::string& item,
                                            const Tag* entry ) = 0;

        /**
         * Receives notifications item removals.
         * @param service Service hosting the parent node.
         * @param node Node ID of the parent node.
         * @param item Item ID of the new Item.
         */
        virtual void handleItemRemoval( const JID& service,
                                        const std::string& node,
                                        const std::string& item) = 0;

        /**
         * Receives notifications of subscription type changes.
         * @param service Service hosting the node.
         * @param node Node ID of the node.
         * @param jid Entity responsible for the change.
         * @param reason Reason for the change.
         * @param type New SubscriptionType.
         */
        virtual void handleSubscriptionChange( const JID& service,
                                               const JID& jid,
                                               const std::string& node,
                                               const std::string& reason,
                                               SubscriptionType type ) = 0;

        /**
         * Receives notifications of node configuration changes.
         * @param service Service hosting the node.
         * @param node Node ID of the node.
         * @param config Optional DataForm of the new configuration (may be null).
         */
        virtual void handleConfigurationChange( const JID& service,
                                                const std::string& node,
                                                const DataForm* config ) = 0;

        /**
         * Receives notifications of node purges.
         * @param service Service hosting the node.
         * @param node Node ID of the purged node.
         */
        virtual void handleNodePurge( const JID& service,
                                      const std::string& node ) = 0;

    };

  }

}

#endif /* PUBSUBEVENTHANDLER_H__ */

