/*! 
 *  $Id: Message.h 196 2009-07-19 14:09:42Z artem.iglikov $
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
 *  Message class declaration
 */

#ifndef Message_h
#define Message_h

#include "QwitHeaders.h"

class Account;

class Message {
public:
	quint64 id;
	QString text;
	QString username;
	QString userpicFilename;
	QDateTime time;
	Account *account;
	bool favorited;
	QString source;
	quint64 inReplyToMessageId;
	QString inReplyToUsername;
	bool directMessage;
        bool following;
	bool mention;
	Message() {}
        Message(quint64 id, const QString &text, const QString &username, const QString &userpicFilename, const QDateTime &time, bool favorited, Account *account, const QString &source, quint64 inReplyToMessageId, const QString &inReplyToUsername, bool following, bool directMessage);
	bool operator<(const Message &x) const;
	bool operator==(const Message &x) const;
	bool operator!=(const Message &x) const;
	void save(QSettings &messagesCache);
	static Message load(QSettings &messagesCache, Account *account);
};

#endif
