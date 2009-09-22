/*!
 *  $Id: TwitterWidgetItemMessage.cpp 186 2009-07-11 14:38:30Z artem.iglikov $
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
 *  TwitterWidgetItemMessage class implementation
 */

#ifndef TwitterWidgetItemMessage_cpp
#define TwitterWidgetItemMessage_cpp

#include "QwitHeaders.h"
#include "QwitTools.h"

#include "Translator.h"
#include "TwitterWidgetItemMessage.h"

TwitterWidgetItemMessage::TwitterWidgetItemMessage(QWidget *parent, const Message &message): QTextBrowser(parent) {
	this->message = message;
}

void TwitterWidgetItemMessage::contextMenuEvent(QContextMenuEvent *event) {
	QMenu *menu = createStandardContextMenu();
	QAction *action = menu->addAction(tr("Translate by GoogleTranslate"));
	if (menu->exec(event->globalPos()) == action) {
		Translator::getInstance()->translate(message.text, this);
		qDebug() << message.text;
/*		if (textCursor().selectedText() != "") {
			text = textCursor().selectedText();
		} else {
			text = toPlainText();
		}
		cout << qPrintable(text) << endl;*/
	}

	delete menu;
}

void TwitterWidgetItemMessage::insertTranslation(const QString &text) {
	qDebug() << "TwitterWidgetItemMessage::insertTranslation()";
	qDebug() << text;
	setHtml(QwitTools::prepareMessage(text, message.account));
}

#endif
