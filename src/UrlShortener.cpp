/*!
 *  $Id: UrlShortener.cpp 192 2009-07-15 05:38:41Z artem.iglikov $
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
 *  UrlShortener class implementation
 */

#ifndef UrlShortener_cpp
#define UrlShortener_cpp

#include "UrlShortener.h"
#include "Configuration.h"
#include "Services.h"

UrlShortener* UrlShortener::instance = 0;

UrlShortener::UrlShortener() {
	http = new QHttp(this);
	connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)));
}

UrlShortener* UrlShortener::getInstance() {
	if (!instance) {
		instance = new UrlShortener();
	}
	return instance;
}

void UrlShortener::setShorteningEnabled(bool yes) {
	enabled = yes;
}

void UrlShortener::shorten(const QString &url) {
	qDebug() << "UrlShortener::shorten() " + url;

	Configuration *config = Configuration::getInstance();

	if (!enabled || config->urlShortener == "0") {
		emit urlShortened(url);
		return;
	}

	currentUrl = url;

	QUrl shortenerUrl(Services::urlShorteners[config->urlShortener]["apiurl"]);

	if (config->useProxy) {
		http->setProxy(config->proxyAddress, config->proxyPort, config->proxyUsername, config->proxyPassword);
	} else {
		http->setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
	}

	if(shortenerUrl.toString().indexOf("https") == 0) {
		http->setHost(shortenerUrl.host(), QHttp::ConnectionModeHttps, shortenerUrl.port(443));
	} else {
		http->setHost(shortenerUrl.host(), QHttp::ConnectionModeHttp, shortenerUrl.port(80));
	}

	buffer.open(QIODevice::WriteOnly);
	QString request = Services::urlShorteners[config->urlShortener]["requesttemplate"];
	request.replace("%url", QUrl::toPercentEncoding(url));
	if(Services::urlShorteners[config->urlShortener]["useapikey"] == "true") {
		QString tmpapikey = config->urlShortenerAPIKey;
		if(tmpapikey == "") {
			tmpapikey = Services::urlShorteners[config->urlShortener]["defaultapikey"];
		}
		request.replace("%apikey", tmpapikey);
	}
	if(Services::urlShorteners[config->urlShortener]["useusername"] == "true") {
		QString tmpusername = config->urlShortenerUsername;
		if(tmpusername == "") {
			tmpusername = Services::urlShorteners[config->urlShortener]["defaultusername"];
		}
		request.replace("%username", tmpusername);
	}
	requestId = http->get(shortenerUrl.path() + request, &buffer);
}

void UrlShortener::requestFinished(int id, bool error) {
	if (id == requestId) {
		QString shortenedUrl = currentUrl;
		if (!error && (http->lastResponse().statusCode() == 200)) {
			qDebug() << ("UrlShortener::requestFinished() " + QString::number(id) + " " + currentUrl);
			Configuration *config = Configuration::getInstance();
			QString response = buffer.data();
			response.replace("\\/", "/");
			QRegExp responseRegexp(Services::urlShorteners[config->urlShortener]["responseregexp"]);
			if (responseRegexp.indexIn(response, 0) == -1) {
				qDebug() << ("UrlShortener::requestFinished() error parsing request");
			} else {
				shortenedUrl = responseRegexp.cap(0);
			}
		} else {
			qDebug() << ("UrlShortener::requestFinished() " + QString::number(id) + " error " + QString::number(http->lastResponse().statusCode()) + " " + http->lastResponse().reasonPhrase());
		}
		emit urlShortened(shortenedUrl);
	}
}

#endif
