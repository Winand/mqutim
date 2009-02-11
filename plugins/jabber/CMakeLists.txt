CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (jabber)

LIST (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include( FindZLIB )
include( FindLibIDN )
if( OpenSSL )
        SET( OPENSSL_REQUIRED )
	include( FindOpenSSL )
endif( OpenSSL )
if( GNUTLS )
        SET( GNUTLS_REQUIRED )
	include( FindGNUTLS )
endif( GNUTLS )
if( WinTLS )
	FIND_LIBRARY( SECUR32_LIBRARIES secur32 )
	FIND_LIBRARY( CRYPT32_LIBRARIES crypt32 )
	if( SECUR32_LIBRARIES )
		message( STATUS "Secur32 found: ${SECUR32_LIBRARIES}" )
	else( )
		message( SEND_ERROR "Secur32 not found" )
	endif( )
        if( CRYPT32_LIBRARIES )
                message( STATUS "Crypt32 found: ${CRYPT32_LIBRARIES}" )
        else( )
                message( SEND_ERROR "Crypt32 not found" )
        endif( )

endif( )

include( CheckFunctionExists )

SET (JABBER_LIBS "")
SET (JABBER_INCLUDE_DIR "")

if( UNIX )
	FIND_LIBRARY( RESOLV_LIBRARIES resolv)
	if( RESOLV_LIBRARIES )
		ADD_DEFINITIONS ( -DHAVE_RES_QUERYDOMAIN )
		ADD_DEFINITIONS ( -DHAVE_DN_SKIPNAME )
		ADD_DEFINITIONS ( -DHAVE_RES_QUERY )
		LIST(APPEND JABBER_LIBS ${RESOLV_LIBRARIES})
		message( STATUS "Found resolv libs: ${RESOLV_LIBRARIES}" )
	else( )
		message( SEND_ERROR "Could NOT find resolv libs" )
	endif( RESOLV_LIBRARIES )
else( WIN32 )
	FIND_LIBRARY( DNSAPI_LIBRARIES dnsapi)
	if( DNSAPI_LIBRARIES )
		ADD_DEFINITIONS ( -DHAVE_WINDNS_H )
		LIST(APPEND JABBER_LIBS ${DNSAPI_LIBRARIES})
	endif( DNSAPI_LIBRARIES )
endif( )

check_function_exists( setsockopt SETSOCKOPT_EXISTS )
if( SETSOCKOPT_EXISTS )
	ADD_DEFINITIONS ( -DHAVE_SETSOCKOPT )
endif( SETSOCKOPT_EXISTS )

if( GNUTLS_FOUND )
        ADD_DEFINITIONS ( -DHAVE_GNUTLS )
	LIST(APPEND JABBER_LIBS ${GNUTLS_LIBRARIES})
	LIST(APPEND JABBER_INCLUDE_DIR ${GNUTLS_INCLUDE_DIR})
else( GNUTLS_FOUND )
	if( OPENSSL_FOUND )
        	ADD_DEFINITIONS ( -DHAVE_OPENSSL )
		LIST(APPEND JABBER_LIBS ${OPENSSL_LIBRARIES} )
		if( WIN32 )
			LIST(APPEND JABBER_LIBS ${OPENSSL_EAY_LIBRARIES} )		
		endif( WIN32 )
		LIST(APPEND JABBER_INCLUDE_DIR ${OPENSSL_INCLUDE_DIR} )
	else( OPENSSL_FOUND )
		if( WIN32 )
			if( SECUR32_LIBRARIES AND CRYPT32_LIBRARIES )
				LIST(APPEND JABBER_LIBS ${SECUR32_LIBRARIES} )
                                LIST(APPEND JABBER_LIBS ${CRYPT32_LIBRARIES} )
				ADD_DEFINITIONS ( -DHAVE_WINTLS )
			endif( SECUR32_LIBRARIES AND CRYPT32_LIBRARIES )
		endif( WIN32 )
	endif( OPENSSL_FOUND )
endif( GNUTLS_FOUND )

if( LIBIDN_FOUND )
	ADD_DEFINITIONS ( -DHAVE_LIBIDN )
	LIST(APPEND JABBER_LIBS ${LIBIDN_LIBRARIES} )
	LIST(APPEND JABBER_INCLUDE_DIR ${LIBIDN_INCLUDE_DIR} )
endif( LIBIDN_FOUND )

if( ZLIB_FOUND )
	ADD_DEFINITIONS ( -DHAVE_ZLIB )
        LIST(APPEND JABBER_LIBS ${ZLIB_LIBRARIES} )
        LIST(APPEND JABBER_INCLUDE_DIR ${ZLIB_INCLUDE_DIR} )
endif( ZLIB_FOUND)

SET (QT_USE_QTNETWORK true)
SET (QT_USE_QTGUI true)
FIND_PACKAGE (Qt4 REQUIRED)

INCLUDE (${CMAKE_ROOT}/Modules/UseQt4.cmake)
FIND_PATH( QUTIM_INCLUDE_DIRS qutim/protocolinterface.h ../../include )
if( QUTIM_INCLUDE_DIRS )
	MESSAGE( STATUS "Found qutIM headers: ${QUTIM_INCLUDE_DIRS}" )
	INCLUDE_DIRECTORIES( ${QUTIM_INCLUDE_DIRS} )
else( QUTIM_INCLUDE_DIRS )
	MESSAGE( SEND_ERROR "Could NOT find qutIM development headers" )
endif( QUTIM_INCLUDE_DIRS )
if( APPLE )
	INCLUDE_DIRECTORIES ( /opt/local/include )
endif( APPLE )
#if( UNIX )
#INCLUDE_DIRECTORIES ( /usr/include )
#endif( UNIX )

INCLUDE_DIRECTORIES (${QT_QTGUI_INCLUDE_DIR}
        ${QT_QTCORE_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
	${JABBER_INCLUDE_DIR}
        .
                src
                libs
        )

SET (QUTIM_QT_HEADERS
src/xmlconsole.h
src/VCardAvatar.h
src/VCardBirthday.h
src/VCardDatedit.h
src/VCardEntry.h
src/VCardLinedit.h
src/VCardConst.h
src/VCardRecord.h
src/VCardLabel.h
src/jAccount.h
src/jBuddy.h
src/jConnection.h
src/jLayer.h
src/jClientIdentification.h
src/conference/joinconferenceform.h
src/conference/jConference.h
src/conference/reasondialog.h
src/conference/topicConfigDialog.h
src/changeResource.h
src/jRoster.h
src/VCardRole.h
src/jSlotSignal.h
src/acceptautdialog.h
src/jVCard.h
src/jJoinChat.h
src/jSearchConference.h
src/jServiceDiscovery.h
src/jAddContact.h
src/xeps/gmailextension.h
src/xeps/tuneextension.h
src/xeps/activityextension.h
src/xeps/versionextension.h
src/xeps/moodsextenstion.h
src/xeps/xstatusextension.h
src/systeminfo.h
src/jPluginSystem.h
src/loginform.h
src/jProtocol.h
src/customstatusdialog.h
src/activitydialog.h
src/jServiceBrowser.h
src/jAccountSettings.h
src/jTransport.h
src/jInvitationHandler.h
src/jDiscoItem.h
src/jDiscoIdentity.h
src/jDataForm.h
src/jAdhoc.h
)
SET(HEADERS
${QUTIM_QT_HEADERS}
libs/config.win.h
libs/gloox/presence.h                                                          
libs/gloox/pubsubevent.h                                                       
libs/gloox/gloox.h                                                             
libs/gloox/resource.h                                                          
libs/gloox/presencehandler.h                                                   
libs/gloox/privacymanager.h                                                    
libs/gloox/parser.h                                                            
libs/gloox/stanza.h                                                            
libs/gloox/rostermanager.h                                                     
libs/gloox/connectionbase.h                                                    
libs/gloox/eventdispatcher.h                                                   
libs/gloox/compressiondatahandler.h                                            
libs/gloox/connectiontcpclient.h                                               
libs/gloox/disco.h                                                             
libs/gloox/jinglecontent.h                                                     
libs/gloox/util.h                                                              
libs/gloox/disconodehandler.h                                                  
libs/gloox/annotations.h                                                       
libs/gloox/nonsaslauth.h                                                       
libs/gloox/siprofilefthandler.h                                                
libs/gloox/simanager.h                                                         
libs/gloox/dns.h                                                               
libs/gloox/sihandler.h                                                         
libs/gloox/gpgsigned.h                                                         
libs/gloox/lastactivityhandler.h                                               
libs/gloox/tag.h                                                               
libs/gloox/chatstate.h                                                         
libs/gloox/md5.h                                                               
libs/gloox/event.h                                                             
libs/gloox/connectionlistener.h                                                
libs/gloox/privatexmlhandler.h                                                 
libs/gloox/jinglevideortp.h                                                    
libs/gloox/bookmarkstorage.h                                                   
libs/gloox/tlsgnutlsclient.h                                                   
libs/gloox/client.h                                                            
libs/gloox/config.h                                                            
libs/gloox/tlsgnutlsbase.h                                                     
libs/gloox/jinglesession.h                                                     
libs/gloox/vcardupdate.h                                                       
libs/gloox/socks5bytestream.h                                                  
libs/gloox/rosteritemdata.h                                                    
libs/gloox/jingleplugin.h                                                      
libs/gloox/shim.h                                                              
libs/gloox/stanzaextension.h                                                   
libs/gloox/capabilities.h                                                      
libs/gloox/instantmucroom.h                                                    
libs/gloox/jingleaudiortp.h                                                    
libs/gloox/featureneg.h                                                        
libs/gloox/connectiontcpserver.h                                               
libs/gloox/registrationhandler.h                                               
libs/gloox/tlsbase.h                                                           
libs/gloox/dataformfieldcontainer.h                                            
libs/gloox/connectionhandler.h                                                 
libs/gloox/sha.h                                                               
libs/gloox/annotationshandler.h                                                
libs/gloox/statisticshandler.h                                                 
libs/gloox/connectionbosh.h                                                    
libs/gloox/bytestream.h                                                        
libs/gloox/searchhandler.h                                                     
libs/gloox/search.h                                                            
libs/gloox/messagesessionhandler.h                                             
libs/gloox/xhtmlim.h                                                           
libs/gloox/adhoc.h                                                             
libs/gloox/jingledescription.h                                                 
libs/gloox/taghandler.h                                                        
libs/gloox/messagehandler.h                                                    
libs/gloox/jid.h                                                               
libs/gloox/messageeventfilter.h                                                
libs/gloox/jinglerawudp.h                                                      
libs/gloox/mutex.h                                                             
libs/gloox/mucinvitationhandler.h                                              
libs/gloox/bookmarkhandler.h                                                   
libs/gloox/jinglesessionhandler.h                                              
libs/gloox/dataformitem.h                                                      
libs/gloox/tlsdefault.h                                                        
libs/gloox/socks5bytestreammanager.h                                           
libs/gloox/connectiontcpbase.h                                                 
libs/gloox/nickname.h                                                          
libs/gloox/jingleiceudp.h                                                      
libs/gloox/subscription.h                                                      
libs/gloox/flexoff.h                                                           
libs/gloox/loghandler.h                                                        
libs/gloox/component.h                                                         
libs/gloox/chatstatefilter.h                                                   
libs/gloox/stanzaextensionfactory.h                                            
libs/gloox/bytestreamdatahandler.h                                             
libs/gloox/connectiondatahandler.h                                             
libs/gloox/gpgencrypted.h                                                      
libs/gloox/privacylisthandler.h                                                
libs/gloox/vcard.h                                                             
libs/gloox/mutexguard.h                                                        
libs/gloox/inbandbytestream.h                                                  
libs/gloox/pubsubmanager.h                                                     
libs/gloox/tlsopenssl.h                                                        
libs/gloox/dataform.h                                                          
libs/gloox/jingletransport.h                                                   
libs/gloox/oob.h                                                               
libs/gloox/pubsubresulthandler.h
libs/gloox/tlsschannel.h
libs/gloox/vcardhandler.h
libs/gloox/siprofileft.h
libs/gloox/flexoffhandler.h
libs/gloox/mucroomconfighandler.h
libs/gloox/logsink.h
libs/gloox/eventhandler.h
libs/gloox/vcardmanager.h
libs/gloox/discohandler.h
libs/gloox/chatstatehandler.h
libs/gloox/tlshandler.h
libs/gloox/privacyitem.h
libs/gloox/iqhandler.h
libs/gloox/mucroomhandler.h
libs/gloox/bytestreamhandler.h
libs/gloox/dataformreported.h
libs/gloox/error.h
libs/gloox/base64.h
libs/gloox/pubsub.h
libs/gloox/macros.h
libs/gloox/messagefilter.h
libs/gloox/connectionhttpproxy.h
libs/gloox/mucroom.h
libs/gloox/messageeventhandler.h
libs/gloox/receipt.h
libs/gloox/compressionbase.h
libs/gloox/privatexml.h
libs/gloox/compressionzlib.h
libs/gloox/siprofilehandler.h
libs/gloox/messageevent.h
libs/gloox/iq.h
libs/gloox/pubsubeventhandler.h
libs/gloox/rosteritem.h
libs/gloox/connectionsocks5proxy.h
libs/gloox/clientbase.h
libs/gloox/pubsubitem.h
libs/gloox/rosterlistener.h
libs/gloox/delayeddelivery.h
libs/gloox/adhoccommandprovider.h
libs/gloox/adhochandler.h
libs/gloox/prep.h
libs/gloox/socks5bytestreamserver.h
libs/gloox/lastactivity.h
libs/gloox/subscriptionhandler.h
libs/gloox/registration.h
libs/gloox/tlsgnutlsserveranon.h
libs/gloox/connectiontls.h
libs/gloox/message.h
libs/gloox/uniquemucroom.h
libs/gloox/tlsgnutlsclientanon.h
libs/gloox/messagesession.h
libs/gloox/mucmessagesession.h
libs/gloox/dataformfield.h
libs/gloox/amp.h
)

SET (SOURCES
src/VCardBirthday.cpp
src/VCardDatedit.cpp
src/VCardEntry.cpp
src/VCardRecord.cpp
src/changeResource.cpp
src/VCardRole.cpp
src/jConnection.cpp
src/jVCard.cpp
src/jLayer.cpp
src/systeminfo.cpp
src/acceptautdialog.cpp
src/jBuddy.cpp
src/jProtocol.cpp
src/conference/jConference.cpp
src/conference/joinconferenceform.cpp
src/conference/topicConfigDialog.cpp
src/conference/reasondialog.cpp
src/jPluginSystem.cpp
src/jAccount.cpp
src/jRoster.cpp
src/VCardAvatar.cpp
src/jAddContact.cpp
src/jClientIdentification.cpp
src/xeps/gmailextension.cpp
src/xeps/versionextension.cpp
src/xeps/tuneextension.cpp
src/xeps/activityextension.cpp
src/xeps/moodsextenstion.cpp
src/xeps/xstatusextension.cpp
src/loginform.cpp
src/VCardLinedit.cpp
src/xmlconsole.cpp
src/VCardLabel.cpp
src/jSlotSignal.cpp
src/jJoinChat.cpp
src/jSearchConference.cpp
src/jServiceDiscovery.cpp
src/customstatusdialog.cpp
src/activitydialog.cpp
src/jServiceBrowser.cpp
src/jAccountSettings.cpp
src/jTransport.cpp
src/jInvitationHandler.cpp
src/jDataForm.cpp
src/jAdhoc.cpp
libs/gloox/iq.cpp                                                                
libs/gloox/connectionbosh.cpp                                                    
libs/gloox/nonsaslauth.cpp                                                       
libs/gloox/connectiontcpserver.cpp                                               
libs/gloox/amp.cpp                                                               
libs/gloox/tlsopenssl.cpp                                                        
libs/gloox/prep.cpp                                                              
libs/gloox/mucmessagesession.cpp                                                 
libs/gloox/dataformreported.cpp                                                  
libs/gloox/pubsubmanager.cpp                                                     
libs/gloox/gpgencrypted.cpp                                                      
libs/gloox/connectionhttpproxy.cpp                                               
libs/gloox/annotations.cpp                                                       
libs/gloox/disco.cpp                                                             
libs/gloox/util.cpp                                                                                                                                                                 
libs/gloox/messagefilter.cpp                                                                                                                                                        
libs/gloox/connectiontcpclient.cpp                                                                                                                                                  
libs/gloox/messageevent.cpp                                                                                                                                                         
libs/gloox/subscription.cpp                                                                                                                                                         
libs/gloox/vcardupdate.cpp                                                                                                                                                          
libs/gloox/tlsgnutlsclient.cpp                                                                                                                                                      
libs/gloox/dataformitem.cpp                                                                                                                                                         
libs/gloox/bookmarkstorage.cpp                                                                                                                                                      
libs/gloox/jinglesession.cpp                                                                                                                                                        
libs/gloox/dns.cpp                                                                                                                                                                  
libs/gloox/connectiontls.cpp                                                                                                                                                        
libs/gloox/tlsgnutlsbase.cpp                                                                                                                                                        
libs/gloox/oob.cpp                                                                                                                                                                  
libs/gloox/gloox.cpp                                                                                                                                                                
libs/gloox/tag.cpp                                                                                                                                                                  
libs/gloox/chatstatefilter.cpp                                                                                                                                                      
libs/gloox/dataform.cpp                                                                                                                                                             
libs/gloox/stanza.cpp                                                                                                                                                               
libs/gloox/message.cpp                                                                                                                                                              
libs/gloox/connectionsocks5proxy.cpp                                                                                                                                                
libs/gloox/messagesession.cpp                                                                                                                                                       
libs/gloox/siprofileft.cpp                                                                                                                                                          
libs/gloox/chatstate.cpp                                                                                                                                                            
libs/gloox/rostermanager.cpp                                                                                                                                                        
libs/gloox/jinglecontent.cpp                                                                                                                                                        
libs/gloox/vcardmanager.cpp
libs/gloox/base64.cpp
libs/gloox/simanager.cpp
libs/gloox/client.cpp
libs/gloox/jid.cpp
libs/gloox/socks5bytestreammanager.cpp
libs/gloox/pubsubevent.cpp
libs/gloox/inbandbytestream.cpp
libs/gloox/socks5bytestream.cpp
libs/gloox/socks5bytestreamserver.cpp
libs/gloox/search.cpp
libs/gloox/xhtmlim.cpp
libs/gloox/shim.cpp
libs/gloox/tlsgnutlsserveranon.cpp
libs/gloox/connectiontcpbase.cpp
libs/gloox/tlsgnutlsclientanon.cpp
libs/gloox/md5.cpp
libs/gloox/parser.cpp
libs/gloox/component.cpp
libs/gloox/stanzaextensionfactory.cpp
libs/gloox/tlsdefault.cpp
libs/gloox/registration.cpp
libs/gloox/presence.cpp
libs/gloox/dataformfield.cpp
libs/gloox/gpgsigned.cpp
libs/gloox/featureneg.cpp
libs/gloox/eventdispatcher.cpp
libs/gloox/vcard.cpp
libs/gloox/receipt.cpp
libs/gloox/dataformfieldcontainer.cpp
libs/gloox/logsink.cpp
libs/gloox/pubsubitem.cpp
libs/gloox/error.cpp
libs/gloox/adhoc.cpp
libs/gloox/jingletransport.cpp
libs/gloox/messageeventfilter.cpp
libs/gloox/instantmucroom.cpp
libs/gloox/privacyitem.cpp
libs/gloox/lastactivity.cpp
libs/gloox/uniquemucroom.cpp
libs/gloox/compressionzlib.cpp
libs/gloox/tlsschannel.cpp
libs/gloox/mutex.cpp
libs/gloox/jingledescription.cpp
libs/gloox/flexoff.cpp
libs/gloox/mucroom.cpp
libs/gloox/privatexml.cpp
libs/gloox/capabilities.cpp
libs/gloox/privacymanager.cpp
libs/gloox/rosteritem.cpp
libs/gloox/sha.cpp
libs/gloox/nickname.cpp
libs/gloox/clientbase.cpp
libs/gloox/delayeddelivery.cpp
)

SET (RESOURCES
./jabber.qrc
)

SET (FORMS
src/loginform.ui
src/xmlconsole.ui
src/wPersonal.ui
src/wChangeResource.ui
src/conference/joinconferenceform.ui
src/conference/topicConfig.ui
src/conference/reasondialog.ui
src/jVCard.ui
src/acceptautdialog.ui
src/jJoinChat.ui
src/jSearchConference.ui
src/jAddContact.ui
src/customstatusdialog.ui
src/activitydialog.ui
src/jServiceBrowser.ui
src/jAccountSettings.ui
)

QT4_WRAP_CPP (MOC_SRCS ${QUTIM_QT_HEADERS})
QT4_WRAP_UI (UIS_H ${FORMS})
QT4_ADD_RESOURCES (RCC ${RESOURCES})

ADD_LIBRARY (jabber SHARED ${SOURCES} ${MOC_SRCS} ${UIS_H} ${RCC})

TARGET_LINK_LIBRARIES (jabber
        ${QT_LIBRARIES}
        ${JABBER_LIBS}
        )
# I do not think, that it is suitable for CMake.
#
# SET_TARGET_PROPERTIES(jabber
#   PROPERTIES
#   PREFIX "../lib"
# )

INSTALL (TARGETS jabber DESTINATION lib/qutim )
