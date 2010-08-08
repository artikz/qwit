project(qwit)

cmake_minimum_required(VERSION 2.6.2 FATAL_ERROR)

set(QT_MIN_VERSION 4.5.0)
find_package(Qt4 REQUIRED)

find_package(Automoc4 REQUIRED)
include (CheckCXXCompilerFlag)

add_definitions( ${QT_DEFINITIONS} )
add_definitions(-DVERSION=\"1.1\")
remove_definitions(-DQT3_SUPPORT_WARNINGS -DQT3_SUPPORT)

include_directories( 
	${QT_INCLUDES} 
	/usr/include/QtOAuth
	/usr/include/qca2/QtCrypto
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

set( LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )

set( qwit_UI
	src/MainWindow.ui
	src/OptionsDialog.ui
	src/AboutDialog.ui
	src/OAuthDialog.ui
	src/AccountConfigurationDialog.ui
	src/DirectMessageDialog.ui
	src/FriendsMgmtDialog.ui
	)

set( qwit_SRCS
    src/qwit.cpp
    src/Configuration.cpp
    src/MainWindow.cpp
    src/MessageTextEdit.cpp
    src/OptionsDialog.cpp
    src/AboutDialog.cpp
    src/OAuthDialog.cpp
    src/DirectMessageDialog.cpp
    src/TwitPicDialog.cpp
    src/AccountConfigurationDialog.cpp
    src/Account.cpp
    src/AbstractPage.cpp
    src/HomePage.cpp
    src/PublicPage.cpp
    src/RepliesPage.cpp
    src/FavoritesPage.cpp
    src/InboxPage.cpp
    src/OutboxPage.cpp
    src/SearchPage.cpp
    src/TwitterWidget.cpp
    src/Services.cpp
    src/Twitter.cpp
    src/Message.cpp
    src/QwitTools.cpp
    src/UserpicsDownloader.cpp
    src/UrlShortener.cpp
    src/TwitterWidgetItem.cpp
    src/FriendsMgmtDialog.cpp
    src/UserMgmtWidget.cpp
    src/UserMgmtWidgetItem.cpp
    src/TwitterWidgetItemMessage.cpp
	src/Translator.cpp
    src/AbstractUserMgmtPage.cpp
    src/FriendshipsMgmtPage.cpp
    src/FollowersMgmtPage.cpp
    src/BlocksMgmtPage.cpp
	)

file( GLOB qwit_TS translations/*.ts )

set( qwit_RCC
	qwit.qrc 
	)

add_definitions( -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_XML_LIB -DQT_SHARED )

qt4_wrap_ui( qwit_UI_SRCS ${qwit_UI} )
qt4_add_resources( qwit_RCC_SRCS ${qwit_RCC} )
qt4_add_translation( qwit_QM ${qwit_TS} )

automoc4_add_executable( qwit
	${qwit_SRCS}
	${qwit_UI_SRCS}
	${qwit_RCC_SRCS}
	)

add_custom_target (translations_target DEPENDS ${qwit_QM} )
add_dependencies( qwit translations_target )

target_link_libraries( qwit
	${QT_QTCORE_LIBRARY}
	${QT_QTNETWORK_LIBRARY}
	${QT_QTGUI_LIBRARY}
	${QT_QTXML_LIBRARY}
	qoauth
	)

install( TARGETS qwit DESTINATION ${QT_BINARY_DIR} )
install( FILES qwit.desktop DESTINATION share/applications )
install( FILES images/qwit.png DESTINATION share/icons )
install( FILES ${qwit_QM} DESTINATION ${QT_TRANSLATIONS_DIR} )

