/*!
 *  $Id: Configuration.cpp 226 2009-08-22 10:23:33Z artem.iglikov $
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
 *  Configuration class implementation
 */

#ifndef Configuration_cpp
#define Configuration_cpp

#include "QwitHeaders.h"

#include "Configuration.h"
#include "Services.h"

QByteArray Configuration::key;
const QString Configuration::CompanyName = "arti";
const QString Configuration::ApplicationName = "qwit2";
const QString Configuration::CacheDirectory = QDir::homePath() + "/.qwit2/";
const QString Configuration::MessagesCacheFileName = Configuration::CacheDirectory + "/messages";

Configuration* Configuration::instance = NULL;
QSettings Configuration::settings(CompanyName, ApplicationName);
QSettings Configuration::messagesCache(Configuration::MessagesCacheFileName, QSettings::IniFormat);

QMap<QString, QString> Configuration::ServicesNames;
QMap<QString, int> Configuration::ServicesIds;
QVector<QString> Configuration::Services;
QMap<QString, QString> Configuration::UrlShortenersNames;
QMap<QString, int> Configuration::UrlShortenersIds;
QVector<QString> Configuration::UrlShorteners;
QVector<QString> Configuration::TranslationsTitles;
QVector<QString> Configuration::TranslationsCodes;
QVector<QString> Configuration::TranslationsCountries;

Configuration::Configuration() {
	for (QMap<QString, QMap<QString, QString> >::iterator it = Services::options.begin(); it != Services::options.end(); ++it) {
		ServicesNames[it.key()] = it.value()["title"];
		Services.push_back(it.key());
		ServicesIds[it.key()] = Services.size() - 1;
	}
	QFile file(settings.fileName());
	currentAccountId = -1;
	file.setPermissions(QFile::ReadUser | QFile::WriteUser);
	for (QMap<QString, QMap<QString, QString> >::iterator it = Services::urlShorteners.begin(); it != Services::urlShorteners.end(); ++it) {
		UrlShortenersNames[it.key()] = it.value()["title"];
		UrlShorteners.push_back(it.key());
		UrlShortenersIds[it.key()] = UrlShorteners.size() - 1;
	}
	int x = 113;
	for (int i = 0; i < 100; ++i) {
		key.append((char)x);
		x = (x * x) % 253;
	}
}

Configuration* Configuration::getInstance() {
	if (!instance) instance = new Configuration();
	return instance;
}

void Configuration::load() {
// State
	settings.beginGroup("State");
	position = settings.value("position", QPoint(100, 100)).toPoint();
	size = settings.value("size", QSize(300, 600)).toSize();
	currentAccountId = settings.value("currentAccountId", -1).toInt();
	searchQuery = settings.value("searchQuery", "").toString();
	bool encryptPasswords = settings.value("encryptPasswords", false).toBool();
	settings.endGroup();
	
// User interface
	settings.beginGroup("User interface");
	
	settings.beginGroup("Misc");
	showGreetingMessage = settings.value("showGreetingMessage", true).toBool();
	greetingMessage = settings.value("greetingMessage", "What's up?").toString();
	showLeftCharactersNumber = settings.value("showLeftCharactersNumber", true).toBool();
	messagesPerPage = settings.value("messagesPerPage", 20).toInt();
	messagesInPopup = settings.value("messagesInPopup", 10).toInt();
	retweetTag = settings.value("retweetTag", "RT @:").toString();
	retweetTagAfterText = settings.value("retweetTagAfterText", false).toBool();
	placeUsernameUnderAvatar = settings.value("placeUsernameUnderAvatar", false).toBool();
	showMessagesInTray = settings.value("showMessagesInTray", true).toBool();
	placeControlsVertically = settings.value("placeControlsVertically", true).toBool();
	placeTabsVertically = settings.value("placeTabsVertically", true).toBool();
	showLastMessage = settings.value("showLastMessage", true).toBool();
    startMinimized = settings.value("startMinimized", false).toBool();
    settings.endGroup();
	
	settings.beginGroup("Tabs");
	showHomeTab = settings.value("showHomeTab", true).toBool();
	autoUpdateHomeTab = settings.value("autoUpdateHomeTab", true).toBool();
	showPublicTab = settings.value("showPublicTab", true).toBool();
	autoUpdatePublicTab = settings.value("autoUpdatePublicTab", false).toBool();
	showRepliesTab = settings.value("showRepliesTab", true).toBool();
	autoUpdateRepliesTab = settings.value("autoUpdateRepliesTab", true).toBool();
	showCustomTab = settings.value("showCustomTab", true).toBool();
	autoUpdateCustomTab = settings.value("autoUpdateCustomTab", false).toBool();
	showInboxTab = settings.value("showInboxTab", true).toBool();
	autoUpdateInboxTab = settings.value("autoUpdateInboxTab", true).toBool();
	showOutboxTab = settings.value("showOutboxTab", true).toBool();
	autoUpdateOutboxTab = settings.value("autoUpdateOutboxTab", false).toBool();
	showSearchTab = settings.value("showSearchTab", true).toBool();
	autoUpdateSearchTab = settings.value("autoUpdateSearchTab", false).toBool();
	showFavoritesTab = settings.value("showFavoritesTab", true).toBool();
	autoUpdateFavoritesTab = settings.value("autoUpdateFavoritesTab", false).toBool();
	updateInterval = settings.value("updateInterval", 300).toInt();
	settings.endGroup();
	
	settings.beginGroup("Colors");
	int color = settings.value("commonMessagesOddColor", (((180 << 8) + 180) << 8) + 180).toInt();
	commonMessagesOddColor.setRgb(color >> 16, (color >> 8) & 255, color & 255);
	color = settings.value("commonMessagesEvenColor", (((230 << 8) + 230) << 8) + 230).toInt();
	commonMessagesEvenColor.setRgb(color >> 16, (color >> 8) & 255, color & 255);
	color = settings.value("mentionsOddColor", (((128 << 8) + 255) << 8) + 128).toInt();
	mentionsOddColor.setRgb(color >> 16, (color >> 8) & 255, color & 255);
	color = settings.value("mentionsEvenColor", (((200 << 8) + 255) << 8) + 200).toInt();
	mentionsEvenColor.setRgb(color >> 16, (color >> 8) & 255, color & 255);
	settings.endGroup();

	settings.beginGroup("Language");
	language = settings.value("language", "system").toString();
	settings.endGroup();

    settings.beginGroup("Integration");
    notificationSubsystem = settings.value("notificationSubsystem", "qt").toString();
    settings.endGroup();

    settings.endGroup();

// Accounts
	settings.beginGroup("Accounts");
	int accountsNumber = settings.beginReadArray("Accounts");
	accounts.clear();
	for (int i = 0; i < accountsNumber; ++i) {
		settings.setArrayIndex(i);
		QString username = settings.value("username", "").toString();
		QString password = settings.value("password", "").toString();
		if (encryptPasswords) {
			password = decrypt(password);
		}
		QString type = settings.value("type", "").toString();
		bool useHttps = settings.value("useHttps", false).toBool();
        QString serviceBaseUrl = settings.value("serviceBaseUrl", "").toString();
        QString serviceApiUrl = settings.value("serviceApiUrl", "").toString();
        bool useOAuth = settings.value("useOAuth", false).toBool();
        QString oauthToken = settings.value("oauthToken", "").toString();
        QString oauthTokenSecret = settings.value("oauthTokenSecret", "").toString();
        Account *account = new Account(type, username, password, useHttps, serviceBaseUrl, serviceApiUrl, useOAuth, oauthToken, oauthTokenSecret);
		addAccount(account);
	}
	settings.endArray();
	settings.endGroup();
	
// Connection
	settings.beginGroup("Connection");
	useProxy = settings.value("useProxy", false).toBool();
	proxyAddress = settings.value("proxyAddress", "").toString();
	proxyPort = settings.value("proxyPort", "").toInt();
	proxyUsername = settings.value("proxyUsername", "").toString();
	proxyPassword = settings.value("proxyPassword", "").toString();
	if (encryptPasswords) {
		proxyPassword = decrypt(proxyPassword);
	}
	settings.endGroup();

// UrlShortener
	settings.beginGroup("UrlShortener");
	urlShortener = settings.value("urlShortener", "unu").toString();
	urlShortenerUsername = settings.value("urlShortenerUsername", "unu").toString();
	urlShortenerAPIKey = settings.value("urlShortenerAPIKey", "unu").toString();
	settings.endGroup();

	loadMessages();
}

void Configuration::save() {
// State
	settings.beginGroup("State");
	settings.setValue("position", position);
	settings.setValue("size", size);
	settings.setValue("currentAccountId", currentAccountId);
	settings.setValue("searchQuery", searchQuery);
	settings.setValue("encryptPasswords", true);
	settings.endGroup();

// User interface
	settings.beginGroup("User interface");
	
	settings.beginGroup("Misc");
	settings.setValue("showGreetingMessage", showGreetingMessage);
	settings.setValue("greetingMessage", greetingMessage);
	settings.setValue("showLeftCharactersNumber", showLeftCharactersNumber);
	settings.setValue("messagesPerPage", messagesPerPage);
	settings.setValue("messagesInPopup", messagesInPopup);
	settings.setValue("retweetTag", retweetTag);
	settings.setValue("retweetTagAfterText", retweetTagAfterText);
	settings.setValue("placeUsernameUnderAvatar", placeUsernameUnderAvatar);
	settings.setValue("showMessagesInTray", showMessagesInTray);
	settings.setValue("placeControlsVertically", placeControlsVertically);
	settings.setValue("placeTabsVertically", placeTabsVertically);
	settings.setValue("showLastMessage", showLastMessage);
    settings.setValue("startMinimized", startMinimized);
    settings.endGroup();

	settings.beginGroup("Tabs");
	settings.setValue("showHomeTab", showHomeTab);
	settings.setValue("autoUpdateHomeTab", autoUpdateHomeTab);
	settings.setValue("showPublicTab", showPublicTab);
	settings.setValue("autoUpdatePublicTab", autoUpdatePublicTab);
	settings.setValue("showRepliesTab", showRepliesTab);
	settings.setValue("autoUpdateRepliesTab", autoUpdateRepliesTab);
	settings.setValue("showCustomTab", showCustomTab);
	settings.setValue("autoUpdateCustomTab", autoUpdateCustomTab);
	settings.setValue("showInboxTab", showInboxTab);
	settings.setValue("autoUpdateInboxTab", autoUpdateInboxTab);
	settings.setValue("showOutboxTab", showOutboxTab);
	settings.setValue("autoUpdateOutboxTab", autoUpdateOutboxTab);
	settings.setValue("showSearchTab", showSearchTab);
	settings.setValue("autoUpdateSearchTab", autoUpdateSearchTab);
	settings.setValue("showFavoritesTab", showFavoritesTab);
	settings.setValue("autoUpdateFavoritesTab", autoUpdateFavoritesTab);
	settings.setValue("updateInterval", updateInterval);
	settings.endGroup();

	settings.beginGroup("Colors");
	settings.setValue("commonMessagesOddColor", (((commonMessagesOddColor.red() << 8) + commonMessagesOddColor.green()) << 8) + commonMessagesOddColor.blue());
	settings.setValue("commonMessagesEvenColor", (((commonMessagesEvenColor.red() << 8) + commonMessagesEvenColor.green()) << 8) + commonMessagesEvenColor.blue());
	settings.setValue("mentionsOddColor", (((mentionsOddColor.red() << 8) + mentionsOddColor.green()) << 8) + mentionsOddColor.blue());
	settings.setValue("mentionsEvenColor", (((mentionsEvenColor.red() << 8) + mentionsEvenColor.green()) << 8) + mentionsEvenColor.blue());
	settings.endGroup();

	settings.beginGroup("Language");
	settings.setValue("language", language);
	settings.endGroup();

    settings.beginGroup("Integration");
    settings.setValue("notificationSubsystem", notificationSubsystem);
    settings.endGroup();

	settings.endGroup();

// Accounts
	settings.beginGroup("Accounts");
	settings.beginWriteArray("Accounts");
	for (int i = 0; i < accounts.size(); ++i) {
		settings.setArrayIndex(i);
		settings.setValue("username", accounts[i]->username);
        if (accounts[i]->useOAuth) {
            settings.setValue("password", "");
        } else {
            settings.setValue("password", encrypt(accounts[i]->password));
        }
		settings.setValue("type", accounts[i]->type);
		settings.setValue("useHttps", accounts[i]->useHttps);
        if (accounts[i]->type == "custom") {
                settings.setValue("serviceBaseUrl", accounts[i]->serviceBaseUrl());
                settings.setValue("serviceApiUrl", accounts[i]->serviceApiUrl());
        }
        settings.setValue("useOAuth", accounts[i]->useOAuth);
        settings.setValue("oauthToken", accounts[i]->oauthToken);
        settings.setValue("oauthTokenSecret", accounts[i]->oauthTokenSecret);
    }
	settings.endArray();
	settings.endGroup();
	
// Connection
	settings.beginGroup("Connection");
	settings.setValue("useProxy", useProxy);
	settings.setValue("proxyAddress", proxyAddress);
	settings.setValue("proxyPort", proxyPort);
	settings.setValue("proxyUsername", proxyUsername);
	settings.setValue("proxyPassword", encrypt(proxyPassword));
	settings.endGroup();
	
// UrlShortener
	settings.beginGroup("UrlShortener");
	settings.setValue("urlShortener", urlShortener);
	settings.setValue("urlShortenerUsername", urlShortenerUsername);
	settings.setValue("urlShortenerAPIKey", urlShortenerAPIKey);
	settings.endGroup();

	saveMessages();
}

int Configuration::addAccount(Account *account) {
	accounts.push_back(account);
	account->id = accounts.size() - 1;
	if (currentAccountId == -1) {
		currentAccountId = account->id;
	}
	return account->id;
}

void Configuration::deleteAccount(int accountId) {
	delete accounts[accountId];
	accounts.erase(accounts.begin() + accountId);
	for (int i = 0; i < accounts.size(); ++i) {
		accounts[i]->id = i;
	}
}

void Configuration::swapAccounts(int account1Id, int account2Id) {
	Account *temp = accounts[account1Id];
	accounts[account1Id] = accounts[account2Id];
	accounts[account2Id] = temp;
}

Account* Configuration::currentAccount() {
	return accounts[currentAccountId];
}

void Configuration::saveMessages() {
	messagesCache.beginWriteArray("Accounts");
	for (int i = 0; i < accounts.size(); ++i) {
		messagesCache.setArrayIndex(i);
		accounts[i]->saveMessages(messagesCache);
	}
	messagesCache.endArray();
}

void Configuration::loadMessages() {
	int n = messagesCache.beginReadArray("Accounts");
	for (int i = 0; i < min(n, accounts.size()); ++i) {
		messagesCache.setArrayIndex(i);
		accounts[i]->loadMessages(messagesCache);
	}
	messagesCache.endArray();
}

QString Configuration::encrypt(const QString &s) {
	QByteArray data = s.toUtf8();
	for (int i = 0; i < data.size(); ++i) {
		data[i] = data[i] ^ key[i % key.size()];
	}
	QString result = data.toBase64();
	return result;
}

QString Configuration::decrypt(const QString &s) {
	QByteArray data = QByteArray::fromBase64(s.toUtf8());
	for (int i = 0; i < data.size(); ++i) {
		data[i] = data[i] ^ key[i % key.size()];
	}
	QString result = QString::fromUtf8(data);
	return result;
}

#endif
