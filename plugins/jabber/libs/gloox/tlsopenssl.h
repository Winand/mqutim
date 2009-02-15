/*
  Copyright (c) 2007-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/



#ifndef TLSOPENSSL_H__
#define TLSOPENSSL_H__

#include "tlsbase.h"

#ifdef _WIN32
# include "../config.h.win"
#elif defined( _WIN32_WCE )
# include "../config.h.win"
#else
# include "config.h"
#endif

#ifdef HAVE_OPENSSL

#include <openssl/ssl.h>

namespace gloox
{

  /**
   * This class implements a TLS backend using OpenSSL.
   *
   * @author Jakob Schröter <js@camaya.net>
   * @since 0.9
   */
  class OpenSSL : public TLSBase
  {
    public:
      /**
       * Constructor.
       * @param th The TLSHandler to handle TLS-related events.
       * @param server The server to use in certificate verification.
       */
      OpenSSL( TLSHandler* th, const std::string& server );

      /**
       * Virtual destructor.
       */
      virtual ~OpenSSL();

      // reimplemented from TLSBase
      virtual bool init();

      // reimplemented from TLSBase
      virtual bool encrypt( const std::string& data );

      // reimplemented from TLSBase
      virtual int decrypt( const std::string& data );

      // reimplemented from TLSBase
      virtual void cleanup();

      // reimplemented from TLSBase
      virtual bool handshake();

      // reimplemented from TLSBase
      virtual void setCACerts( const StringList& cacerts );

      // reimplemented from TLSBase
      virtual void setClientCert( const std::string& clientKey, const std::string& clientCerts );

    private:
      void pushFunc();

      enum TLSOperation
      {
        TLSHandshake,
        TLSWrite,
        TLSRead
      };

      void doTLSOperation( TLSOperation op );
      int openSSLTime2UnixTime( const char* time_string );

      SSL* m_ssl;
      SSL_CTX* m_ctx;
      BIO* m_ibio;
      BIO* m_nbio;
      std::string m_recvBuffer;
      std::string m_sendBuffer;
      char* m_buf;
      const int m_bufsize;
  };

}

#endif // HAVE_OPENSSL

#endif // TLSOPENSSL_H__
