/*!
 *  $Id: Message.cpp 225 2009-08-21 13:53:51Z artem.iglikov $
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
 *  Message class implementation
 */

#ifndef Message_cpp
#define Message_cpp

#include "QwitHeaders.h"
#include "QwitTools.h"

#include "Message.h"

Message::Message(quint64 id, const QString &text, const QString &username, const QString &userpicFilename, const QDateTime &time, bool favorited, Account *account, const QString &source, quint64 inReplyToMessageId, const QString &inReplyToUsername, bool following, bool directMessage) {
/// FIXIT: temporary workaround for strange id values in ~/.qwit2/messages after KDE shutdown
	if (id > ((quint64)1 << 60)) {
		id = (long long)id + ((long long) 1 << 32);
	}

	this->id = id;
	this->text = text;
	this->username = username;
	this->userpicFilename = userpicFilename;
	this->time = time;
	this->account = account;
	this->favorited = favorited;
	this->source = source;
        this->following = following;
	this->inReplyToMessageId = inReplyToMessageId;
	this->inReplyToUsername = inReplyToUsername;
	this->directMessage = directMessage;
	mention = QwitTools::isMention(*this);
}

bool Message::operator<(const Message &x) const {
	return id > x.id;
}

bool Message::operator==(const Message &x) const {
	return id == x.id;
}

bool Message::operator!=(const Message &x) const {
	return id != x.id;
}

void Message::save(QSettings &messagesCache) {
	messagesCache.setValue("id", id);
	messagesCache.setValue("text", text);
	messagesCache.setValue("username", username);
	messagesCache.setValue("userpicFilename", userpicFilename);
	messagesCache.setValue("time", time);
	messagesCache.setValue("favorited", favorited);
	messagesCache.setValue("source", source);
        messagesCache.setValue("following", following);
        messagesCache.setValue("inReplyToMessageId", inReplyToMessageId);
	messagesCache.setValue("inReplyToUsername", inReplyToUsername);
	messagesCache.setValue("directMessage", directMessage);
}

Message Message::load(QSettings &messagesCache, Account *account) {
	return Message(
		messagesCache.value("id").toULongLong(),
		messagesCache.value("text").toString(),
		messagesCache.value("username").toString(),
		messagesCache.value("userpicFilename").toString(),
		messagesCache.value("time").toDateTime(),
		messagesCache.value("favorited").toBool(),
		account,
		messagesCache.value("source").toString(),
		messagesCache.value("inReplyToMessageId").toULongLong(),
		messagesCache.value("inReplyToUsername").toString(),
                messagesCache.value("following").toBool(),
                messagesCache.value("directMessage").toBool()
	);
}

#endif
