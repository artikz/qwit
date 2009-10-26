/*! 
 *  $Id: MessageTextEdit.h 186 2009-07-11 14:38:30Z artem.iglikov $
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
 *  MessageTextEdit class declaration
 */

#ifndef MessageTextEdit_h
#define MessageTextEdit_h

#include "QwitHeaders.h"

#include "Message.h"

class MessageTextEdit: public QTextEdit {
	Q_OBJECT

private:
	quint64 inReplyToMessageId;
	QCompleter *_completer;
	QString textUnderCursor() const;
	QMenu *languagesMenu;
	QMap<QAction*, QString> actionLanguage;

public:
	static const int MaxMessageCharacters = 140;
	static const int StandardHeight = 45;
	QCompleter *completer;

	MessageTextEdit(QWidget *parent = 0);

	void focusInEvent(QFocusEvent *event);
	void focusOutEvent(QFocusEvent *event);
	int getMaxMessageCharactersNumber();
	void insertFromMimeData(const QMimeData *source);
	void clear();

protected:

	void keyPressEvent(QKeyEvent *e);
	void contextMenuEvent(QContextMenuEvent *event);
	
signals:

	void messageEntered(const QString &, quint64);
	void leftCharsNumberChanged(int);

private slots:

	void insertCompletion(const QString &completion);
	void insertTranslation(const QString &translation, QObject *item);

public slots:

	void textChangedToCharsNumberChanged();
	void updateSize();
	void retweet(const Message &message);
	void reply(const Message &message);
	void insertUrl(const QString &url);
};

#endif
