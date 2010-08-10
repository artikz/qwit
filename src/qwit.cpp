/*!
 *  $Id: qwit.cpp 186 2009-07-11 14:38:30Z artem.iglikov $
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
 *  Qwit main function
 */

#include "QwitHeaders.h"

#include "MainWindow.h"
#include "Services.h"
#include "QwitTools.h"


int main(int argc, char *argv[]) {
	QDir dir(QDir::homePath());
	dir.mkdir(Configuration::CacheDirectory);
	
	qInstallMsgHandler(handleMessage);
	
	QApplication app(argc, argv);

	QLocale systemLocale = QLocale::system();

	Services::initialize();

	Configuration *config = Configuration::getInstance();
	config->load();

	QTranslator translator;
	translator.load(QString(":/translations/qwit_") + (config->language == "system" ? systemLocale.name() : config->language));
	app.installTranslator(&translator);

	Configuration::TranslationsCodes.push_back("system");
	Configuration::TranslationsCountries.push_back(systemLocale.name().mid(3, 2));
	Configuration::TranslationsTitles.push_back(app.tr("System (%1, %2)").arg(systemLocale.languageToString(systemLocale.language())).arg(systemLocale.countryToString(systemLocale.country())));

	QDir translationsDir(":/translations");
	QStringList translationNames = translationsDir.entryList(QStringList("*.qm"), QDir::Files, QDir::Name);
	QMutableStringListIterator i(translationNames);
	while (i.hasNext()) {
		i.next();
		QString languageCode = i.value().mid(5, 5);
		QLocale locale(languageCode);
		Configuration::TranslationsCodes.push_back(languageCode);
		Configuration::TranslationsCountries.push_back(languageCode.mid(3, 2));
		Configuration::TranslationsTitles.push_back(QString("%1, %2").arg(systemLocale.languageToString(locale.language())).arg(systemLocale.countryToString(locale.country())));
	}

    MainWindow *mainWindow = MainWindow::getInstance();
    if (!config->startMinimized) mainWindow->show();
	return app.exec();
}
