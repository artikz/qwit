/*!
 *  $Id: Translator.h 186 2009-07-11 14:38:30Z artem.iglikov $
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
 *  Translator class declaration
 */

#ifndef Translator_h
#define Translator_h

#include "QwitHeaders.h"

class Translator: public QObject {
	Q_OBJECT

	QHttp *http;
	QBuffer buffer;
	QMap<int, QObject*> requestSender;
	static Translator *instance;
	Translator();

public:
	static Translator *getInstance();

public slots:
	void translate(const QString &url, QObject *sender);
	void requestFinished(int id, bool error);

signals:
	void translated(const QString &text);
};

#endif
