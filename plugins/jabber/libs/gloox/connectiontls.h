/*
 * Copyright (c) 2007-2009 by Jakob Schroeter <js@camaya.net>
 * This file is part of the gloox library. http://camaya.net/gloox
 *
 * This software is distributed under a license. The full license
 * agreement can be found in the file LICENSE in this distribution.
 * This software may not be copied, modified, sold or distributed
 * other than expressed in the named license agreement.
 *
 * This software is distributed without any warranty.
 */

#ifndef CONNECTIONTLS_H__
#define CONNECTIONTLS_H__

#include <string>
#include "gloox.h"
#include "logsink.h"
#include "connectionbase.h"
#include "tlsdefault.h"
#include "connectiondatahandler.h"

namespace gloox
{

  /**
   * @brief This is an implementation of a TLS/SSL connection.
   *
   * Usage:
   *
   * @code
   * Client *c = new Client( ... );
   * c->setConnectionImpl( new ConnectionTLS( c,
   *                                new ConnectionTCP( c->logInstance(), server, port ),
   *                                c->logInstance()) );
   * @endcode
   *
   * Due to the need for handshaking data to be sent/received before the connection is fully
   * established, be sure not to use the connection until ConnectionDataHandler::handleConnect()
   * of the specified ConnectionDataHandler is called.
   *
   * @author Matthew Wild <mwild1@gmail.com>
   * @since 1.0
   */

  class GLOOX_API ConnectionTLS : public TLSHandler, public ConnectionBase, ConnectionDataHandler
  {
    public:
      /**
       * Constructs a new ConnectionTLS object.
       * @param cdh The ConnectionDataHandler that will be notified of events from this connection
       * @param conn A transport connection. It should be configured to connect to
       * the server and port you wish to make the encrypted connection to.
       * ConnectionTLS will own the transport connection and delete it in its destructor.
       * @param logInstance The log target. Obtain it from ClientBase::logInstance().
       */
      ConnectionTLS( ConnectionDataHandler* cdh, ConnectionBase* conn, const LogSink& log );

      /**
       * Virtual Destructor.
       */
      virtual ~ConnectionTLS();

      // reimplemented from ConnectionBase
      virtual ConnectionError connect();

      // reimplemented from ConnectionBase
      virtual ConnectionError recv( int timeout = -1 );

      // reimplemented from ConnectionBase
      virtual bool send( const std::string& data );

      // reimplemented from ConnectionBase
      virtual ConnectionError receive();

      // reimplemented from ConnectionBase
      virtual void disconnect();

      // reimplemented from ConnectionBase
      virtual void cleanup();

      // reimplemented from ConnectionBase
      virtual void getStatistics( int& totalIn, int& totalOut );

      // reimplemented from ConnectionDataHandler
      virtual void handleReceivedData( const ConnectionBase* connection, const std::string& data );

      // reimplemented from ConnectionDataHandler
      virtual void handleConnect( const ConnectionBase* connection );

      // reimplemented from ConnectionDataHandler
      virtual void handleDisconnect( const ConnectionBase* connection, ConnectionError reason );

      // reimplemented from ConnectionDataHandler
      virtual ConnectionBase* newInstance() const;

      // TLSHandler callbacks
      virtual void handleEncryptedData( const TLSBase*, const std::string& data );
      virtual void handleDecryptedData( const TLSBase*, const std::string& data );
      virtual void handleHandshakeResult( const TLSBase* base, bool success, CertInfo& certinfo );

    private:
      ConnectionBase* m_connection;
      TLSDefault* m_tls;
      const LogSink & m_log;
      bool m_handshaked;

  };

}

#endif
