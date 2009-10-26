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
	languages["am"] = "Amharic";
	languages["ar"] = "Arabic";
	languages["hy"] = "Armenian";
	languages["az"] = "Azerbaijani";
	languages["eu"] = "Basque";
	languages["be"] = "Belarusian";
	languages["bn"] = "Bengali";
	languages["bh"] = "Bihari";
	languages["bg"] = "Bulgarian";
	languages["my"] = "Burmese";
	languages["ca"] = "Catalan";
	languages["chr"] = "Cherokee";
	languages["zh"] = "Chinese";
	languages["zh-CN"] = "Chinese Simplified";
	languages["zh-TW"] = "Chinese Traditional";
	languages["hr"] = "Croatian";
	languages["cs"] = "Czech";
	languages["da"] = "Danish";
	languages["dv"] = "Dhivehi";
	languages["nl"] = "Dutch";
	languages["en"] = "English";
	languages["eo"] = "Esperanto";
	languages["et"] = "Estonian";
	languages["tl"] = "Filipino";
	languages["fi"] = "Finnish";
	languages["fr"] = "French";
	languages["gl"] = "Galician";
	languages["ka"] = "Georgian";
	languages["de"] = "German";
	languages["el"] = "Greek";
	languages["gn"] = "Guarani";
	languages["gu"] = "Gujarati";
	languages["iw"] = "Hebrew";
	languages["hi"] = "Hindi";
	languages["hu"] = "Hungarian";
	languages["is"] = "Icelandic";
	languages["id"] = "Indonesian";
	languages["iu"] = "Inuktitut";
	languages["ga"] = "Irish";
	languages["it"] = "Italian";
	languages["ja"] = "Japanese";
	languages["kn"] = "Kannada";
	languages["kk"] = "Kazakh";
	languages["km"] = "Khmer";
	languages["ko"] = "Korean";
	languages["ku"] = "Kurdish";
	languages["ky"] = "Kyrgyz";
	languages["lo"] = "Laothian";
	languages["lv"] = "Latvian";
	languages["lt"] = "Lithuanian";
	languages["mk"] = "Macedonian";
	languages["ms"] = "Malay";
	languages["ml"] = "Malayalam";
	languages["mt"] = "Maltese";
	languages["mr"] = "Marathi";
	languages["mn"] = "Mongolian";
	languages["ne"] = "Nepali";
	languages["no"] = "Norwegian";
	languages["or"] = "Oriya";
	languages["ps"] = "Pashto";
	languages["fa"] = "Persian";
	languages["pl"] = "Polish";
	languages["pt-PT"] = "Portuguese";
	languages["pa"] = "Punjabi";
	languages["ro"] = "Romanian";
	languages["ru"] = "Russian";
	languages["sa"] = "Sanskrit";
	languages["sr"] = "Serbian";
	languages["sd"] = "Sindhi";
	languages["si"] = "Sinhalese";
	languages["sk"] = "Slovak";
	languages["sl"] = "Slovenian";
	languages["es"] = "Spanish";
	languages["sw"] = "Swahili";
	languages["sv"] = "Swedish";
	languages["tg"] = "Tajik";
	languages["ta"] = "Tamil";
	languages["tl"] = "Tagalog";
	languages["te"] = "Telugu";
	languages["th"] = "Thai";
	languages["bo"] = "Tibetan";
	languages["tr"] = "Turkish";
	languages["uk"] = "Ukrainian";
	languages["ur"] = "Urdu";
	languages["uz"] = "Uzbek";
	languages["ug"] = "Uighur";
	languages["vi"] = "Vietnamese";
	languages["cy"] = "Welsh";
	languages["yi"] = "Yiddish";

	countries["kk"] = "kz";
	countries["ru"] = "ru";
	countries["en"] = "us";
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
			QString response = buffer->data();
			if (response.indexOf("translatedText") != -1) {
				int position1 = response.indexOf("translatedText") + 17;
				int position2 = response.lastIndexOf("detectedSourceLanguage") - 3;
				translatedText = QString::fromUtf8(response.mid(position1, position2 - position1).trimmed().toLocal8Bit().data());
				qDebug() << translatedText;
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
	QMenu *languagesMenu = new QMenu(tr("Translate by GoogleTranslate"));
	actionLanguage[languagesMenu->addAction(tr("Restore original"))] = "-";
	languagesMenu->addSeparator();
	for (QMap<QString, QString>::iterator it = languages.begin(); it != languages.end(); ++it) {
		QString country = "";
		if (countries.find(it.key()) != countries.end()) country = countries[it.key()];
		else country = it.key().mid(0, 2);
		actionLanguage[languagesMenu->addAction(QIcon(":/images/countries/" + country + ".png"), it.value())] = it.key();
	}
	return languagesMenu;
}

#endif
