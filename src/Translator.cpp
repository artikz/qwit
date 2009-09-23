/*!
 *  $Id: Translator.cpp 225 2009-08-21 13:53:51Z artem.iglikov $
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
 *  Translator class implementation
 */

#ifndef Translator_cpp
#define Translator_cpp

#include "QwitHeaders.h"

#include "Configuration.h"
#include "Translator.h"

Translator* Translator::instance = 0;

Translator::Translator() {
	http = new QHttp(this);
	connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)));
}

Translator* Translator::getInstance() {
	if (!instance) {
		instance = new Translator();
	}
	return instance;
}

void Translator::translate(const QString &text, QObject *sender) {
	qDebug() << "Translator::translate() " + text;

	Configuration *config = Configuration::getInstance();

	QUrl translatorUrl("http://ajax.googleapis.com/ajax/services/language/translate");

	if (config->useProxy) {
		http->setProxy(config->proxyAddress, config->proxyPort, config->proxyUsername, config->proxyPassword);
	} else {
		http->setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
	}

	if(translatorUrl.toString().indexOf("https") == 0) {
		http->setHost(translatorUrl.host(), QHttp::ConnectionModeHttps, translatorUrl.port(443));
	} else {
		http->setHost(translatorUrl.host(), QHttp::ConnectionModeHttp, translatorUrl.port(80));
	}

	buffer.open(QIODevice::WriteOnly);
	QString request = "?v=1.0&q=" + QUrl::toPercentEncoding(text) + "&format=html&langpair=%7Cen";
	qDebug() << translatorUrl.path() + request;
	int requestId = http->get(translatorUrl.path() + request, &buffer);
	requestSender[requestId] = sender;
}

void Translator::requestFinished(int id, bool error) {
	if (requestSender.find(id) != requestSender.end()) {
		QString translatedText = "";
		if (!error && (http->lastResponse().statusCode() == 200)) {
			qDebug() << ("Translator::requestFinished() " + QString::number(id));
			Configuration *config = Configuration::getInstance();
			QString response = buffer.data();
			int position1 = response.indexOf("translatedText") + 17;
			int position2 = response.lastIndexOf("detectedSourceLanguage") - 3;
			translatedText = response.mid(position1, position2 - position1);
		} else {
			qDebug() << ("Translator::requestFinished() " + QString::number(id) + " error " + QString::number(http->lastResponse().statusCode()) + " " + http->lastResponse().reasonPhrase());
		}
		QObject *sender = requestSender[id];
		requestSender.erase(requestSender.find(id));
		emit textTranslated(translatedText, sender);
	}
}

#endif
