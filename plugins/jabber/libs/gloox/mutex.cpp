/*
  Copyright (c) 2007-2009 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#include "mutex.h"

#if !defined( _WIN32 ) && !defined( _WIN32_WCE )
# include "config.h"
#endif

#ifdef _WIN32
# include <windows.h>
#endif

#ifdef _WIN32_WCE
# include <winbase.h>
#endif

#ifdef HAVE_PTHREAD
# include <pthread.h>
#endif

namespace gloox
{

  namespace util
  {

    class Mutex::MutexImpl
    {
      public:
        MutexImpl();
        ~MutexImpl();
        void lock();
        void unlock();
      private:
        MutexImpl( const MutexImpl& );
        MutexImpl& operator=( const MutexImpl& );

  #ifdef _WIN32
        CRITICAL_SECTION m_cs;
  #elif defined( HAVE_PTHREAD )
        pthread_mutex_t m_mutex;
  #endif

    };

    Mutex::MutexImpl::MutexImpl()
    {
  #ifdef _WIN32
      InitializeCriticalSection( &m_cs );
  #elif defined( HAVE_PTHREAD )
      pthread_mutex_init( &m_mutex, 0 );
  #endif
    }

    Mutex::MutexImpl::~MutexImpl()
    {
  #ifdef _WIN32
      DeleteCriticalSection( &m_cs );
  #elif defined( HAVE_PTHREAD )
      pthread_mutex_destroy( &m_mutex );
  #endif
    }

    void Mutex::MutexImpl::lock()
    {
  #ifdef _WIN32
      EnterCriticalSection( &m_cs );
  #elif defined( HAVE_PTHREAD )
      pthread_mutex_lock( &m_mutex );
  #endif
    }

    void Mutex::MutexImpl::unlock()
    {
  #ifdef _WIN32
      LeaveCriticalSection( &m_cs );
  #elif defined( HAVE_PTHREAD )
      pthread_mutex_unlock( &m_mutex );
  #endif
    }

    Mutex::Mutex()
      : m_mutex( new MutexImpl() )
    {
    }

    Mutex::~Mutex()
    {
      delete m_mutex;
    }

    void Mutex::lock()
    {
      m_mutex->lock();
    }

    void Mutex::unlock()
    {
      m_mutex->unlock();
    }

  }

}
