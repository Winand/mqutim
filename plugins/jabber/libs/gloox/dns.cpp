/*
  Copyright (c) 2005-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifdef _WIN32
# include "../config.h.win"
#elif defined( _WIN32_WCE )
# include "../config.h.win"
#else
# include "config.h"
#endif

#include "gloox.h"
#include "dns.h"

#ifndef _WIN32_WCE
# include <sys/types.h>
# include <sstream>
#endif

#include <stdio.h>

#if !defined( _WIN32 ) && !defined( _WIN32_WCE )
# include <netinet/in.h>
# include <arpa/nameser.h>
# include <resolv.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/un.h>
# include <unistd.h>
#endif

#ifdef _WIN32
# include <winsock.h>
#elif defined( _WIN32_WCE )
# include <winsock2.h>
#endif

#ifdef HAVE_WINDNS_H
# include <windns.h>
#endif

#define SRV_COST    (RRFIXEDSZ+0)
#define SRV_WEIGHT  (RRFIXEDSZ+2)
#define SRV_PORT    (RRFIXEDSZ+4)
#define SRV_SERVER  (RRFIXEDSZ+6)
#define SRV_FIXEDSZ (RRFIXEDSZ+6)

#ifndef T_SRV
# define T_SRV 33
#endif

// mingw
#ifndef DNS_TYPE_SRV
# define DNS_TYPE_SRV 33
#endif

#ifndef C_IN
# define C_IN 1
#endif

#ifndef INVALID_SOCKET
# define INVALID_SOCKET -1
#endif

#define XMPP_PORT 5222

namespace gloox
{

#if defined( HAVE_RES_QUERYDOMAIN ) && defined( HAVE_DN_SKIPNAME ) && defined( HAVE_RES_QUERY )
  DNS::HostMap DNS::resolve( const std::string& service, const std::string& proto,
                             const std::string& domain, const LogSink& logInstance )
  {
    buffer srvbuf;
    bool error = false;

    const std::string dname = "_" +  service + "._" + proto;

    if( !domain.empty() )
      srvbuf.len = res_querydomain( dname.c_str(), const_cast<char*>( domain.c_str() ),
                                    C_IN, T_SRV, srvbuf.buf, NS_PACKETSZ );
    else
      srvbuf.len = res_query( dname.c_str(), C_IN, T_SRV, srvbuf.buf, NS_PACKETSZ );

    if( srvbuf.len < 0 )
      return defaultHostMap( domain, logInstance );

    HEADER* hdr = (HEADER*)srvbuf.buf;
    unsigned char* here = srvbuf.buf + NS_HFIXEDSZ;

    if( ( hdr->tc ) || ( srvbuf.len < NS_HFIXEDSZ ) )
      error = true;

    if( hdr->rcode >= 1 && hdr->rcode <= 5 )
      error = true;

    if( ntohs( hdr->ancount ) == 0 )
      error = true;

    if( ntohs( hdr->ancount ) > NS_PACKETSZ )
      error = true;

    int cnt;
    for( cnt = ntohs( hdr->qdcount ); cnt>0; --cnt )
    {
      int strlen = dn_skipname( here, srvbuf.buf + srvbuf.len );
      here += strlen + NS_QFIXEDSZ;
    }

    unsigned char* srv[NS_PACKETSZ];
    int srvnum = 0;
    for( cnt = ntohs( hdr->ancount ); cnt>0; --cnt )
    {
      int strlen = dn_skipname( here, srvbuf.buf + srvbuf.len );
      here += strlen;
      srv[srvnum++] = here;
      here += SRV_FIXEDSZ;
      here += dn_skipname( here, srvbuf.buf + srvbuf.len );
    }

    if( error )
    {
      return defaultHostMap( domain, logInstance );
    }

    // (q)sort here

    HostMap servers;
    for( cnt=0; cnt<srvnum; ++cnt )
    {
      name srvname;

      if( ns_name_ntop( srv[cnt] + SRV_SERVER, (char*)srvname, NS_MAXDNAME ) < 0 )
        printf( "handle this error!\n" );

      servers[(char*)srvname] = ns_get16( srv[cnt] + SRV_PORT );
    }

    return servers;
  }

#elif defined( _WIN32 ) && defined( HAVE_WINDNS_H )
  DNS::HostMap DNS::resolve( const std::string& service, const std::string& proto,
                             const std::string& domain, const LogSink& logInstance )
  {
    const std::string dname = "_" +  service + "._" + proto + "." + domain;
    bool error = false;

    DNS::HostMap servers;
    DNS_RECORD* pRecord;
    if( DnsQuery( dname.c_str(), DNS_TYPE_SRV, DNS_QUERY_STANDARD, NULL, &pRecord, NULL ) == ERROR_SUCCESS )
    {
      DNS_RECORD* pRec = pRecord;
      do
      {
        if( pRec->wType == DNS_TYPE_SRV )
        {
          servers[pRec->Data.SRV.pNameTarget] = pRec->Data.SRV.wPort;
        }
        pRec = pRec->pNext;
      }
      while( pRec != NULL );
      DnsRecordListFree( pRecord, DnsFreeRecordList );
    }
    else
      error = true;

    if( error || !servers.size() )
    {
      servers = defaultHostMap( domain, logInstance );
    }

    return servers;
  }

#else
  DNS::HostMap DNS::resolve( const std::string& service, const std::string& proto,
                             const std::string& domain, const LogSink& logInstance )
  {
    logInstance.warn( LogAreaClassDns, "notice: gloox does not support SRV "
                        "records on this platform. Using A records instead." );
    return defaultHostMap( domain, logInstance );
  }
#endif

  DNS::HostMap DNS::defaultHostMap( const std::string& domain, const LogSink& logInstance )
  {
    HostMap server;

    logInstance.warn( LogAreaClassDns, "notice: no SRV record found for "
                                          + domain + ", using default port." );

    if( !domain.empty() )
      server[domain] = XMPP_PORT;

    return server;
  }

#ifdef HAVE_GETADDRINFO
  void DNS::resolve( struct addrinfo** res, const std::string& service, const std::string& proto,
                     const std::string& domain, const LogSink& logInstance )
  {
    logInstance.dbg( LogAreaClassDns, "Resolving: _" +  service + "._" + proto + "." + domain );
    struct addrinfo hints;
    if( proto == "tcp" )
      hints.ai_socktype = SOCK_STREAM;
    else if( proto == "udp" )
      hints.ai_socktype = SOCK_DGRAM;
    else
    {
      logInstance.err( LogAreaClassDns, "Unknown/Invalid protocol: " + proto );
    }
    memset( &hints, '\0', sizeof( hints ) );
    hints.ai_flags = AI_ADDRCONFIG | AI_CANONNAME;
    hints.ai_socktype = SOCK_STREAM;
    int e = getaddrinfo( domain.c_str(), service.c_str(), &hints, res );
    if( e )
      logInstance.err( LogAreaClassDns, "getaddrinfo() failed" );
  }

  int DNS::connect( const std::string& host, const LogSink& logInstance )
  {
    struct addrinfo* results = 0;

    resolve( &results, host, logInstance );
    if( !results )
    {
      logInstance.err( LogAreaClassDns, "host not found: " + host );
      return -ConnDnsError;
    }

    struct addrinfo* runp = results;
    while( runp )
    {
      int fd = DNS::connect( runp, logInstance );
      if( fd >= 0 )
        return fd;

      runp = runp->ai_next;
    }

    freeaddrinfo( results );

    return -ConnConnectionRefused;
  }

  int DNS::connect( struct addrinfo* res, const LogSink& logInstance )
  {
    if( !res )
      return -1;

    int fd = getSocket( res->ai_family, res->ai_socktype, res->ai_protocol );
    if( fd < 0 )
      return fd;

    if( ::connect( fd, res->ai_addr, res->ai_addrlen ) == 0 )
    {
      char ip[NI_MAXHOST];
      char port[NI_MAXSERV];

      if( getnameinfo( res->ai_addr, sizeof( sockaddr ),
                       ip, sizeof( ip ),
                       port, sizeof( port ),
                       NI_NUMERICHOST | NI_NUMERICSERV ) )
      {
        //FIXME do we need to handle this? How? Can it actually happen at all?
//         printf( "could not get numeric hostname");
      }

      std::ostringstream oss;
      oss << "Connecting to ";
      if( res->ai_canonname )
      {
        oss << res->ai_canonname;
        oss << " (" << ip << "), port " << port;
      }
      else
      {
        oss << ip << ":" << port;
      }
      logInstance.dbg( LogAreaClassDns, oss.str() );
      return fd;
    }

    closeSocket( fd );
    return -ConnConnectionRefused;
  }

#else

  int DNS::connect( const std::string& host, const LogSink& logInstance )
  {
    HostMap hosts = resolve( host, logInstance );
    if( hosts.size() == 0 )
      return -ConnDnsError;

    HostMap::const_iterator it = hosts.begin();
    for( ; it != hosts.end(); ++it )
    {
      int fd = DNS::connect( (*it).first, (*it).second, logInstance );
      if( fd >= 0 )
        return fd;
    }

    return -ConnConnectionRefused;
  }
#endif

  int DNS::getSocket()
  {
#ifdef _WIN32
    WSADATA wsaData;
    if( WSAStartup( MAKEWORD( 1, 1 ), &wsaData ) != 0 )
      return -ConnDnsError;
#endif

    struct protoent* prot;
    if( ( prot = getprotobyname( "tcp" ) ) == 0 )
    {
      cleanup();
      return -ConnDnsError;
    }
    return getSocket( PF_INET, SOCK_STREAM, prot->p_proto );
  }

  int DNS::getSocket( int af, int socktype, int proto )
  {
    int fd;
    if( ( fd = socket( af, socktype, proto ) ) == -1 )
    {
      cleanup();
      return -ConnConnectionRefused;
    }

#ifdef HAVE_SETSOCKOPT
    int timeout = 5000;
    setsockopt( fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof( timeout ) );
    setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, (char*)&timeout, sizeof( timeout ) );
#endif

    return fd;
  }

  int DNS::connect( const std::string& host, unsigned short port, const LogSink& logInstance )
  {
    int fd = getSocket();
    if( fd < 0 )
      return fd;

    struct hostent* h;
    if( ( h = gethostbyname( host.c_str() ) ) == 0 )
    {
      cleanup();
      return -ConnDnsError;
    }

    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons( port );

    if( h->h_length != sizeof( struct in_addr ) )
    {
      cleanup();
      return -ConnDnsError;
    }
    else
    {
      memcpy( &target.sin_addr, h->h_addr, sizeof( struct in_addr ) );
    }

#ifndef _WIN32_WCE
    std::ostringstream oss;
#endif

    memset( target.sin_zero, '\0', 8 );
    if( ::connect( fd, (struct sockaddr *)&target, sizeof( struct sockaddr ) ) == 0 )
    {
#ifndef _WIN32_WCE
      oss << "connecting to " << host.c_str()
          << " (" << inet_ntoa( target.sin_addr ) << ":" << port << ")";
      logInstance.dbg( LogAreaClassDns, oss.str() );
#endif
      return fd;
    }

#ifndef _WIN32_WCE
    oss << "connection to " << host.c_str()
         << " (" << inet_ntoa( target.sin_addr ) << ":" << port << ") failed";
    logInstance.dbg( LogAreaClassDns, oss.str() );
#endif

    closeSocket( fd );
    return -ConnConnectionRefused;
  }

  void DNS::closeSocket( int fd )
  {
#ifndef _WIN32
    close( fd );
#else
    closesocket( fd );
#endif
  }

  void DNS::cleanup()
  {
#ifdef _WIN32
    WSACleanup();
#endif
  }

}
