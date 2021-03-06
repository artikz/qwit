# #####################################################################
# Automatically generated by qmake (2.01a) Tue Dec 16 18:48:51 2008
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += src \
    images
INCLUDEPATH += src
OBJECTS_DIR = var
MOC_DIR = var
UI_SOURCES_DIR = var
UI_HEADERS_DIR = var
RCC_DIR = var
unix:# Prefix: base instalation directory
isEmpty( PREFIX ):PREFIX = /usr/local

# DEFINES += 'REVISION=\\\"$(shell svnversion -n .)\\\"'
DEFINES += 'VERSION=\\\"1.1\\\"'

# Input
HEADERS += src/Configuration.h \
    src/QwitException.h \
    src/MainWindow.h \
    src/MessageTextEdit.h \
    src/OptionsDialog.h \
    src/AboutDialog.h \
    src/DirectMessageDialog.h \
    src/TwitPicDialog.h \
    src/Account.h \
    src/AccountConfigurationDialog.h \
    src/AbstractPage.h \
    src/HomePage.h \
    src/PublicPage.h \
    src/RepliesPage.h \
    src/FavoritesPage.h \
    src/InboxPage.h \
    src/OutboxPage.h \
    src/SearchPage.h \
    src/TwitterWidget.h \
    src/Twitter.h \
    src/Services.h \
    src/Message.h \
    src/QwitTools.h \
    src/UserpicsDownloader.h \
    src/QwitHeaders.h \
    src/UrlShortener.h \
    src/TwitterWidgetItem.h \
    src/TwitterWidgetItemMessage.h \
    src/Translator.h \
    src/FriendsMgmtDialog.h \
    src/UserMgmtWidget.h \
    src/UserMgmtWidgetItem.h \
    src/AbstractUserMgmtPage.h \
    src/FriendshipsMgmtPage.h \
    src/FollowersMgmtPage.h \
    src/BlocksMgmtPage.h \
    src/OAuthDialog.h  \
    src/QProgressIndicator.h
FORMS += src/MainWindow.ui \
    src/OptionsDialog.ui \
    src/AboutDialog.ui \
    src/AccountConfigurationDialog.ui \
    src/DirectMessageDialog.ui \
    src/FriendsMgmtDialog.ui \
    src/OAuthDialog.ui
SOURCES += src/qwit.cpp \
    src/Configuration.cpp \
    src/MainWindow.cpp \
    src/MessageTextEdit.cpp \
    src/OptionsDialog.cpp \
    src/AboutDialog.cpp \
    src/DirectMessageDialog.cpp \
    src/TwitPicDialog.cpp \
    src/AccountConfigurationDialog.cpp \
    src/Account.cpp \
    src/AbstractPage.cpp \
    src/HomePage.cpp \
    src/PublicPage.cpp \
    src/RepliesPage.cpp \
    src/FavoritesPage.cpp \
    src/InboxPage.cpp \
    src/OutboxPage.cpp \
    src/SearchPage.cpp \
    src/TwitterWidget.cpp \
    src/Services.cpp \
    src/Twitter.cpp \
    src/Message.cpp \
    src/QwitTools.cpp \
    src/UserpicsDownloader.cpp \
    src/UrlShortener.cpp \
    src/TwitterWidgetItem.cpp \
    src/FriendsMgmtDialog.cpp \
    src/UserMgmtWidget.cpp \
    src/UserMgmtWidgetItem.cpp \
    src/TwitterWidgetItemMessage.cpp \
    src/Translator.cpp \
    src/AbstractUserMgmtPage.cpp \
    src/FriendshipsMgmtPage.cpp \
    src/FollowersMgmtPage.cpp \
    src/BlocksMgmtPage.cpp \
    src/OAuthDialog.cpp  \
    src/QProgressIndicator.cpp    
TRANSLATIONS += translations/qwit_en_US.ts \
    translations/qwit_es_ES.ts \
    translations/qwit_it_IT.ts \
    translations/qwit_pt_BR.ts \
    translations/qwit_ru_RU.ts \
    translations/qwit_kk_KZ.ts \
    translations/qwit_de_DE.ts \
    translations/qwit_fi_FI.ts \
    translations/qwit_zh_CN.ts \
    translations/qwit_pl_PL.ts \
    translations/qwit_ro_RO.ts \
    translations/qwit_ko_KR.ts \
    translations/qwit_id_ID.ts \
    translations/qwit_tr_TR.ts
QT += network \
    xml
RESOURCES = qwit.qrc

# Windows icons
RC_FILE = qwit-win.rc

# Mac OS X icon
ICON = images/qwit.icns
target.path = $${PREFIX}/bin
desktop.path = $${PREFIX}/share/applications
desktop.files += qwit.desktop
icon.path = $${PREFIX}/share/icons
icon.files += images/qwit.png
INSTALLS = target \
    desktop \
    icon
CONFIG += debug \
    x86 \
    ppc \
    x86_64 \
    ppc64 \
    oauth
#    console
OTHER_FILES += qwit-win.rc
