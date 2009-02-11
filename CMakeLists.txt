CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
IF (COMMAND cmake_policy)
	cmake_policy (SET CMP0003 NEW)
ENDIF (COMMAND cmake_policy)
PROJECT (qutim)
SET(QT_MIN_VERSION "4.4.0") 

if( UNIX )
	if( BSD )
		ADD_DEFINITIONS ( -DBSD )
		set( CMAKE_THREAD_LIBS:STRING "-pthread" )
		set( CMAKE_USE_PTHREADS:BOOL ON )
		set( CMAKE_EXE_LINKER_FLAGS:STRING "-pthread" )
	endif( BSD )
endif( UNIX )



SET (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET (CMAKE_BUILD_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
LIST (APPEND CMAKE_MODULE_PATH "cmake")
LIST (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
SET (QT_USE_QTNETWORK true)
SET (QT_USE_QTXML true)
SET (QT_USE_QTWEBKIT true)
SET (QUTIM_LIBS "")

IF( Phonon )
	SET (PHONON_REQUIRED)
	include( FindPhonon )
	IF(PHONON_FOUND)
		ADD_DEFINITIONS ( -DHAVE_PHONON )
	        LIST(APPEND QUTIM_LIBS ${PHONON_LIBS})
	ENDIF(PHONON_FOUND)
ENDIF( Phonon )

SET (QT_USE_QTUITOOLS true)
SET (QUTIM_PLUGINS_DEST "lib/qutim/")
FIND_PACKAGE (Qt4 REQUIRED)

INCLUDE (${CMAKE_ROOT}/Modules/UseQt4.cmake)
INCLUDE_DIRECTORIES (${QT_QTGUI_INCLUDE_DIR}
	${QT_QTCORE_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	.
		src
		src/plugins
		src/idle
		include
	)


SET (HEADERS
    include/qutim/plugininterface.h
    include/qutim/protocolinterface.h
    src/globalsettings/abstractglobalsettings.h
    src/globalsettings/globalproxysettings.h
    src/antispam/abstractantispamlayer.h
    src/antispam/antispamlayersettings.h
    src/chatwindow/chatwindowsettings.h
    src/chatwindow/chatwindowstyle.h
    src/chatwindow/chatwindowstyleoutput.h
    src/chatwindow/conferencecontactlist.h
    src/chatwindow/conferenceitem.h
    src/chatwindow/conferenceitemmodel.h
    src/chatwindow/chatemoticonmenu.h
        src/chatwindow/conferencetabcompletion.h
        src/chatwindow/movielabel.h
	src/chatwindow/separatechatwindow.h
	src/chatwindow/separateconference.h
		src/chatwindow/tabbedconference.h
	src/chatwindow/tabbedchatwindow.h
	src/contactlist/contactlistitemdelegate.h
	src/contactlist/contactlisteventeater.h
	src/contactlist/contactlistproxymodel.h
	src/contactlist/contactlistsettings.h
	src/contactlist/proxymodelitem.h
    src/contactlist/treecontactlistmodel.h
    src/contactlist/treeitem.h
    src/notifications/abstractnotificationlayer.h
    src/notifications/notificationslayersettings.h
    src/notifications/popuptextbrowser.h
	src/notifications/popupwindow.h
    src/history/historyengine.h
    src/history/historywindow.h
    src/history/historysettings.h
    src/sound/soundlayersettings.h
    src/themeengine/abstractthemeengine.h
    src/themeengine/qskinobject.h
    src/themeengine/qskinwidgetresizehandler.h
	src/abstractcontactlist.h
	src/abstractcontextlayer.h
	src/abstracthistorylayer.h
	src/abstractstatus.h
	src/abstractchatlayer.h
    src/aboutinfo.h
    src/abstractlayer.h
    src/abstractsoundlayer.h
    src/accountmanagement.h
    src/addaccountwizard.h
    src/contactseparator.h
    src/console.h
    src/guisettingswindow.h
    src/iconmanager.h
    src/logindialog.h
    src/mainsettings.h
    src/pluginsettings.h
    src/pluginsystem.h
    src/profilelogindialog.h
    src/qutim.h
    src/qutimsettings.h
    src/statusdialog.h
    src/statuspresetcaption.h
    src/idle/idle.h
	)

SET (SOURCES
	main.cpp
	src/globalsettings/abstractglobalsettings.cpp
    src/globalsettings/globalproxysettings.cpp
	src/antispam/abstractantispamlayer.cpp
    src/antispam/antispamlayersettings.cpp
	src/chatwindow/chatwindowsettings.cpp
	src/chatwindow/chatwindowstyle.cpp
    src/chatwindow/chatwindowstyleoutput.cpp
    src/chatwindow/conferencecontactlist.cpp
    src/chatwindow/conferenceitem.cpp
    src/chatwindow/conferenceitemmodel.cpp
    src/chatwindow/chatemoticonmenu.cpp
            src/chatwindow/conferencetabcompletion.cpp
    src/chatwindow/movielabel.cpp
	src/chatwindow/separatechatwindow.cpp
	src/chatwindow/separateconference.cpp
			src/chatwindow/tabbedconference.cpp
	src/chatwindow/tabbedchatwindow.cpp
	src/contactlist/contactlistitemdelegate.cpp
	src/contactlist/contactlisteventeater.cpp
	src/contactlist/contactlistproxymodel.cpp
	src/contactlist/contactlistsettings.cpp
	src/contactlist/proxymodelitem.cpp
    src/contactlist/treecontactlistmodel.cpp
    src/contactlist/treeitem.cpp
    src/notifications/abstractnotificationlayer.cpp
    src/notifications/notificationslayersettings.cpp
    src/notifications/popuptextbrowser.cpp
    	src/notifications/popupwindow.cpp
    src/history/historyengine.cpp
        src/history/historywindow.cpp
        src/history/historysettings.cpp
    src/sound/soundlayersettings.cpp
    src/themeengine/abstractthemeengine.cpp
        src/themeengine/qskinobject.cpp
    src/themeengine/qskinwidgetresizehandler.cpp
	src/abstractcontactlist.cpp
		src/abstractcontextlayer.cpp
	src/abstracthistorylayer.cpp
	src/abstractstatus.cpp
	src/abstractchatlayer.cpp
    src/aboutinfo.cpp
    src/abstractlayer.cpp
    src/abstractsoundlayer.cpp
    src/accountmanagement.cpp
    src/addaccountwizard.cpp
    src/contactseparator.cpp
    src/console.cpp
    src/guisettingswindow.cpp
    src/iconmanager.cpp
    src/logindialog.cpp
    src/mainsettings.cpp
    src/pluginsettings.cpp
    src/pluginsystem.cpp
    src/profilelogindialog.cpp
    src/qutim.cpp
    src/qutimsettings.cpp
    src/statusdialog.cpp
    src/statuspresetcaption.cpp
    src/idle/idle.cpp
	)

SET (RESOURCES
	qutim.qrc
	)

SET (FORMS
    src/globalsettings/globalproxysettings.ui
    src/antispam/antispamlayersettings.ui
    src/chatwindow/chatwindowsettings.ui
    src/chatwindow/chatwindowform.ui
    src/chatwindow/conferenceform.ui
    src/contactlist/contactlistsettings.ui
        src/history/historywindow.ui
        src/history/historysettings.ui
    src/notifications/notificationslayersettings.ui
	src/notifications/popupwindow.ui
    src/sound/soundlayersettings.ui
	src/aboutinfo.ui
	src/accountmanagement.ui
	src/guisettingswindow.ui
    src/logindialog.ui
    src/mainsettings.ui
    src/pluginsettings.ui
    src/profilelogindialog.ui
    src/qutim.ui
    src/qutimsettings.ui
    src/statusdialogvisual.ui
    src/statuspresetcaption.ui
    src/console.ui
	)

IF (WIN32)
	LIST (APPEND SOURCES
		src/ex/exsystrayicon.cpp
        src/idle/idle_win.cpp
		)
	LIST (APPEND HEADERS
		src/ex/exsystrayicon.h
		)
		
if (MINGW)
exec_program(windres
ARGS "-i qutim.rc -o qutim_res.o --include-dir=${CMAKE_CURRENT_SOURCE_DIR}")
LIST (APPEND SOURCES qutim_res.o)
else(MINGW)
LIST (APPEND SOURCES qutim.rc)
endif(MINGW)
 
ENDIF (WIN32)

IF (APPLE)
    LIST (APPEND SOURCES
		src/ex/exsystrayicon.cpp
        src/idle/idle_mac.cpp
		)
    LIST (APPEND HEADERS
		src/ex/exsystrayicon.h
		)
	SET (MACOSX_BUNDLE_ICON_FILE icons/qutim.icns)

ELSEIF (UNIX)
	LIST (APPEND SOURCES
		src/idle/idle_x11.cpp
		)
ENDIF ()

QT4_WRAP_CPP (MOC_SRCS ${HEADERS})
QT4_WRAP_UI (UIS_H ${FORMS})
QT4_ADD_RESOURCES (RCC ${RESOURCES})

ADD_EXECUTABLE (qutim WIN32 ${SOURCES} ${MOC_SRCS} ${UIS_H} ${RCC})
TARGET_LINK_LIBRARIES (qutim
	${QT_LIBRARIES}
	${QUTIM_LIBS}
	)
INSTALL (TARGETS qutim DESTINATION bin)
