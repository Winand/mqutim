/*
  Copyright (c) 2007-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/



#ifndef TLSGNUTLSCLIENT_H__
#define TLSGNUTLSCLIENT_H__

#include "tlsgnutlsbase.h"

#ifdef _WIN32
# include "../config.h.win"
#elif defined( _WIN32_WCE )
# include "../config.h.win"
#else
# include "config.h"
#endif

#ifdef HAVE_GNUTLS

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

namespace gloox
{

  /**
   * @brief This class implements a TLS backend using GnuTLS.
   *
   * You should not need to use this class directly.
   *
   * @author Jakob Schröter <js@camaya.net>
   * @since 0.9
   */
  class GnuTLSClient : public GnuTLSBase
  {
    public:
      /**
       * Constructor.
       * @param th The TLSHandler to handle TLS-related events.
       * @param server The server to use in certificate verification.
       */
      GnuTLSClient( TLSHandler* th, const std::string& server );

      /**
       * Virtual destructor.
       */
      virtual ~GnuTLSClient();

      // reimplemented from TLSBase
      virtual bool init();

      // reimplemented from TLSBase
      virtual void setCACerts( const StringList& cacerts );

      // reimplemented from TLSBase
      virtual void setClientCert( const std::string& clientKey, const std::string& clientCerts );

      // reimplemented from TLSBase
      virtual void cleanup();

    private:
      virtual void getCertInfo();

      bool verifyAgainst( gnutls_x509_crt_t cert, gnutls_x509_crt_t issuer );
      bool verifyAgainstCAs( gnutls_x509_crt_t cert, gnutls_x509_crt_t *CAList, int CAListSize );

      gnutls_certificate_credentials m_credentials;

  };

}

#endif // HAVE_GNUTLS

#endif // TLSGNUTLSCLIENT_H__
