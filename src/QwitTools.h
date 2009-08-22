/*! 
 *  $Id: QwitTools.h 196 2009-07-19 14:09:42Z artem.iglikov $
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
 *  QwitTools class declaration
 */

#ifndef QwitTools_h
#define QwitTools_h

#include "QwitHeaders.h"

#include "Message.h"

class QwitTools: public QObject {
	Q_OBJECT
	
private:
	static QRegExp urlRegExp;
	static QRegExp usernameRegExp;
	static QRegExp hashtagRegExp;
	static QRegExp ipAddressRegExp;
	static QMap<QString, int> monthes;

public:
	static QDateTime dateFromString(QString date);
	static QDateTime dateFromAtomString(QString date);
	static QIcon getToolButtonIcon(const QString &iconFileName, bool active = false);
	static QString formatDateTime(const QDateTime &time);
	static QVector<Message> parseMessages(const QByteArray &data, Account *account);
	static QVector<Message> parseInboxMessages(const QByteArray &data, Account *account);
	static QVector<Message> parseOutboxMessages(const QByteArray &data, Account *account);
	static QVector<Message> parseSearchMessages(const QByteArray &data, Account *account);
	static Message parseUser(const QByteArray &data, Account *account);
	static Message parseMessage(const QByteArray &data, Account *account);
	static Message parseDirectMessage(const QByteArray &data, Account *account);
	static QString parseError(const QByteArray &data);
	static void makeMessagesUnique(QVector<Message> &messages);
	static QString prepareMessage(const QString &text, Account *account);
	static QVector<Message> mergeMessages(QVector<Message> &messages, QVector<Message> &receivedMessages);
	static bool isUrl(const QString &s);
	static bool isMention(const Message &message);
};

void handleMessage(QtMsgType type, const char *msg);

#endif
