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
        languages["af"] = "Afrikaans";
        languages["sq"] = "Albanian";
        languages["ar"] = "Arabic";
        languages["be"] = "Belarusian";
        languages["bg"] = "Bulgarian";
        languages["ca"] = "Catalan";
        languages["zh"] = "Chinese";
        languages["zh-CN"] = "Chinese Simplified";
        languages["zh-TW"] = "Chinese Traditional";
        languages["hr"] = "Croatian";
        languages["cs"] = "Czech";
        languages["da"] = "Danish";
        languages["nl"] = "Dutch";
        languages["en"] = "English";
        languages["et"] = "Estonian";
        languages["tl"] = "Filipino";
        languages["fi"] = "Finnish";
        languages["fr"] = "French";
        languages["gl"] = "Galician";
        languages["de"] = "German";
        languages["el"] = "Greek";
        languages["iw"] = "Hebrew";
        languages["hi"] = "Hindi";
        languages["hu"] = "Hungarian";
        languages["is"] = "Icelandic";
        languages["id"] = "Indonesian";
        languages["ga"] = "Irish";
        languages["it"] = "Italian";
        languages["ja"] = "Japanese";
        languages["ko"] = "Korean";
        languages["lv"] = "Latvian";
        languages["lt"] = "Lithuanian";
        languages["mk"] = "Macedonian";
        languages["ms"] = "Malay";
        languages["mt"] = "Maltese";
        languages["no"] = "Norwegian";
        languages["fa"] = "Persian";
        languages["pl"] = "Polish";
        languages["pt-PT"] = "Portuguese";
        languages["ro"] = "Romanian";
        languages["ru"] = "Russian";
        languages["sr"] = "Serbian";
        languages["sk"] = "Slovak";
        languages["sl"] = "Slovenian";
        languages["es"] = "Spanish";
        languages["sw"] = "Swahili";
        languages["sv"] = "Swedish";
        languages["th"] = "Thai";
        languages["tr"] = "Turkish";
        languages["uk"] = "Ukrainian";
        languages["vi"] = "Vietnamese";
        languages["cy"] = "Welsh";
        languages["yi"] = "Yiddish";

	countries["kk"] = "kz";
	countries["ru"] = "ru";
	countries["en"] = "us";

        QVector<QPair<QString, QString> > list;
        for (QMap<QString, QString>::iterator it = languages.begin(); it != languages.end(); ++it) {
                list.push_back(qMakePair(it.value(), it.key()));
        }
        qSort(list);
        for (int i = 0; i < list.size(); ++i) {
            languageCodesSorted.push_back(list[i].second);
        }
}

Translator::~Translator() {
}

Translator* Translator::getInstance() {
	if (!instance) {
		instance = new Translator();
	}
	return instance;
}

void Translator::translate(const QString &text, const QString &language, QObject *sender) {
	qDebug() << "Translator::translate() " + text;

	Configuration *config = Configuration::getInstance();

	QUrl translatorUrl("http://ajax.googleapis.com/ajax/services/language/translate");
	QString request = "?v=1.0&q=" + QUrl::toPercentEncoding(text) + "&format=html&langpair=%7C" + language;

	QHttpRequestHeader header;
	header.setRequest("GET", translatorUrl.path() + request);
	header.setValue("Host", translatorUrl.host());
	header.setValue("Referer", "Qwit");

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

	buffer = new QBuffer(this);
	buffer->open(QIODevice::WriteOnly);
	int requestId = http->request(header, 0, buffer);
	requestSender[requestId] = sender;
}

void Translator::requestFinished(int id, bool error) {
	if (requestSender.find(id) != requestSender.end()) {
		QString translatedText = "";
		if (!error && (http->lastResponse().statusCode() == 200)) {
			qDebug() << ("Translator::requestFinished() " + QString::number(id));
			Configuration *config = Configuration::getInstance();
                        QString response = QString::fromUtf8(buffer->data().data());
			if (response.indexOf("translatedText") != -1) {
				int position1 = response.indexOf("translatedText") + 17;
				int position2 = response.lastIndexOf("detectedSourceLanguage") - 3;
                                translatedText = response.mid(position1, position2 - position1).trimmed();
			}
		} else {
			qDebug() << ("Translator::requestFinished() " + QString::number(id) + " error " + QString::number(http->lastResponse().statusCode()) + " " + http->lastResponse().reasonPhrase());
		}
		delete buffer;
		QObject *sender = requestSender[id];
		requestSender.erase(requestSender.find(id));
		emit textTranslated(translatedText, sender);
	}
}

QMenu* Translator::createLanguagesMenu(QMap<QAction*, QString> &actionLanguage) {
        QMenu *languagesMenu = new QMenu(tr("GoogleTranslate"));
        languagesMenu->setIcon(QIcon(":/images/google.png"));
	actionLanguage[languagesMenu->addAction(tr("Restore original"))] = "-";
	languagesMenu->addSeparator();
        for (int i = 0; i < languageCodesSorted.size(); ++i) {
                QString languageCode = languageCodesSorted[i];
                QString country = "";
                if (countries.find(languageCode) != countries.end()) country = countries[languageCode];
                else country = languageCode.mid(0, 2);
                actionLanguage[languagesMenu->addAction(QIcon(":/images/countries/" + country + ".png"), languages[languageCode])] = languageCode;
	}
	return languagesMenu;
}

#endif
