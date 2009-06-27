/*  This file is part of Qwit.

    Copyright (C) 2008, 2009 Artem Iglikov
    
    Qwit is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Qwit is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Qwit.  If not, see <http://www.gnu.org/licenses/>. */

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <iostream>

#include "MainWindow.h"

using namespace std;

int main(int argc, char *argv[]) {
	cout << "Qwit 0.9 - free cross-platform Qt4-based Twitter client (http://code.google.com/p/qwit/)" << endl;
	cout << "Copyright (C) 2008, 2009 Artem Iglikov (http://arti.kz/)" << endl;
	
	cout << "\nLICENSE\n\nThis program is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program.  If not, see <http://www.gnu.org/licenses/>." << endl;

	QApplication app(argc, argv);

	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(QString(":/translations/qwit_") + locale);
	app.installTranslator(&translator);

	MainWindow::getInstance()->show();
	return app.exec();
}
