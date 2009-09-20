/*! 
 *  $Id: Configuration.h 201 2009-07-21 16:04:47Z artem.iglikov $
 *
 *  @file
 *  @author Artem Iglikov <artem.iglikov@gmail.com>
 *  
 *  @section LICENSE
 *  
 *  This file is part of Qwit.
 *  
 *  Copyright (C) 2008, 2009 Artem Iglikov
 *  
 *  Qwit is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  Qwit is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with Qwit.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @section DESCRIPTION
 *
 *  Configuration class declaration
 */

#ifndef Configuration_h
#define Configuration_h

#include "QwitHeaders.h"

#include "Account.h"
#include "UrlShortener.h"

class Configuration {
	Configuration();
	static QSettings settings;
	static QSettings messagesCache;
	
public:
	static const QString CompanyName;
	static const QString ApplicationName;
	static const QString CacheDirectory;
	static const QString MessagesCacheFileName;
	static QVector<QString> TranslationsTitles;
	static QVector<QString> TranslationsCodes;
	static QVector<QString> TranslationsCountries;


	static QMap<QString, QString> ServicesNames;
	static QMap<QString, int> ServicesIds;
	static QVector<QString> Services;
	static QVector<QString> UrlShorteners;
	static QMap<QString, int> UrlShortenersIds;
	static QMap<QString, QString> UrlShortenersNames;
	
// State
	QPoint position;
	QSize size;
	int currentAccountId;
	QString searchQuery;
	
// User interface options
	bool showGreetingMessage;
	QString greetingMessage;
	bool showLeftCharactersNumber;
	int messagesPerPage;
	int messagesInPopup;
	QString retweetTag;
	bool retweetTagAfterText;
	bool placeUsernameUnderAvatar;
	bool showMessagesInTray;
	bool placeControlsVertically;
	bool placeTabsVertically;
	bool showLastMessage;
	bool showHomeTab;
	bool autoUpdateHomeTab;
	bool showPublicTab;
	bool autoUpdatePublicTab;
	bool showRepliesTab;
	bool autoUpdateRepliesTab;
	bool showCustomTab;
	bool autoUpdateCustomTab;
	bool showInboxTab;
	bool autoUpdateInboxTab;
	bool showOutboxTab;
	bool autoUpdateOutboxTab;
	bool showSearchTab;
	bool autoUpdateSearchTab;
	bool showFavoritesTab;
	bool autoUpdateFavoritesTab;
	int updateInterval;
	QColor commonMessagesOddColor;
	QColor commonMessagesEvenColor;
	QColor mentionsOddColor;
	QColor mentionsEvenColor;
	QString language;

// Connection
	bool useProxy;
	QString proxyAddress;
	int proxyPort;
	QString proxyUsername;
	QString proxyPassword;

// Accounts
	QVector<Account*> accounts;

	static Configuration* instance;
	
	int addAccount(Account *account);
	void deleteAccount(int accountId);
	void swapAccounts(int account1Id, int account2Id);
	Account* currentAccount();
	void saveMessages();
	void loadMessages();

// UrlShortener
	QString urlShortener;
	//UrlShortener* getUrlShortener();
	
public:
	static Configuration* getInstance();
	
	void save();
	void load();
};

#endif
