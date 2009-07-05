project(qwit)

cmake_minimum_required(VERSION 2.6.2 FATAL_ERROR)

set(QT_MIN_VERSION 4.5.0)
find_package(Qt4 REQUIRED)

find_package(Automoc4 REQUIRED)
include (CheckCXXCompilerFlag)

add_definitions( ${QT_DEFINITIONS} )
remove_definitions(-DQT3_SUPPORT_WARNINGS -DQT3_SUPPORT)

include_directories( 
	${QT_INCLUDES} 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

set( LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )

set( qwit_UI
	src/LogsDialog.ui 
	src/MainWindow.ui 
	src/OptionsDialog.ui
	)

set( qwit_SRCS
	src/LogsDialog.cpp
	src/MainWindow.cpp
	src/OptionsDialog.cpp
	src/qwit.cpp
	src/StatusTextEdit.cpp
	src/Twitter.cpp
	src/TwitterWidget.cpp
	src/TwitterWidgetItem.cpp
	src/UserpicsDownloader.cpp
	src/TwitPicDialog.cpp
	)

file( GLOB qwit_TS translations/*.ts )

set( qwit_RCC
	qwit.qrc 
	)

add_definitions( -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_XML_LIB -DQT_SHARED )

qt4_wrap_ui( qwit_UI_SRCS ${qwit_UI} )
qt4_add_resources( qwit_RCC_SRCS ${qwit_RCC} )
qt4_add_translation( qwit_QM ${qwit_TS} )

IF( MINGW )
# resource compilation for MinGW
ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/qwit-win.o
		COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/qwit-win.rc 
		-o ${CMAKE_CURRENT_BINARY_DIR}/qwit-win.o )
	SET(qwit_SRCS ${qwit_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/qwit-win.o)
ELSE( MINGW )
	SET(qwit_SRCS ${qwit_SRCS} qwit-win.rc)
ENDIF( MINGW )


automoc4_add_executable( qwit WIN32
	${qwit_SRCS}
	${qwit_UI_SRCS}
	${qwit_RCC_SRCS}
	)

add_custom_target (translations_target DEPENDS ${qwit_QM} )
add_dependencies( qwit translations_target )

target_link_libraries( qwit
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTXML_LIBRARY}
	${QT_QTNETWORK_LIBRARY}
	)

install( TARGETS qwit DESTINATION ${QT_BINARY_DIR} )
install( FILES qwit.desktop DESTINATION share/applications )
install( FILES images/qwit.png DESTINATION share/icons )
install( FILES ${qwit_QM} DESTINATION ${QT_TRANSLATIONS_DIR} )

