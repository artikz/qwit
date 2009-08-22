/*! 
 *  $Id: Account.h 207 2009-07-24 18:05:05Z artem.iglikov $
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
 *  Account class declaration
 */

#ifndef Account_h
#define Account_h

#include "QwitHeaders.h"

#include "Twitter.h"
#include "Message.h"

class Twitter;
class Message;

class Account: public QObject {
	Q_OBJECT
	
private:
	QString _serviceBaseUrl;
	QString _serviceApiUrl;
	QString _searchBaseUrl;
	QString _searchApiUrl;
	QString _singleMessageUrl;
	QStringList usernamesCache;

public:
	int id;
	QString type;
	QString username;
	QString password;
	Message lastMessage;
	Twitter *twitter;
	int remainingRequests;
	bool useHttps;
	bool sendingMessage;
	QString messageBeingSent;
	QStringListModel usernamesCacheModel;
	
	QVector<Message> friendsMessages;
	QVector<Message> replies;
	QVector<Message> publicMessages;
	QVector<Message> favorites;
	QVector<Message> inboxMessages;
	QVector<Message> outboxMessages;
	QVector<Message> searchMessages;

	Account();
	Account(const QString &type, const QString &username, const QString &password, bool useHttps = false);
	void saveMessages(QSettings &messagesCache);
	void loadMessages(QSettings &messagesCache);
	QString serviceApiUrl();
	QString serviceBaseUrl();
	QString searchBaseUrl();
	QString searchApiUrl();
	QString singleMessageUrl();
	void setRemainingRequests(int remainingRequests);
	void addUsernamesToCache(const QStringList &usernames);

public slots:
	void addFriendsMessages(const QByteArray &data);
	void addReplies(const QByteArray &data);
	void addPublicMessages(const QByteArray &data);
	void addFavorites(const QByteArray &data);
	void addPreviousFavorites(const QByteArray &data);
	void addInboxMessages(const QByteArray &data);
	void addOutboxMessages(const QByteArray &data);
	void addSearchMessages(const QByteArray &data);
	void updateLastMessage(const QByteArray &data);
	void updateLastMessage();
	void sendMessage(const QString &message, quint64 inReplyToMessageId);
	void messageSent(const QByteArray &data);
	void messageNotSent();
//	void directMessageSent(const QByteArray &data);
	void messageFavored(const QByteArray &data);
	void messageUnfavored(const QByteArray &data);
//	void messageFavorChanged(const QByteArray &data);
	void messageDestroyed(const QByteArray &data);
	void directMessageDestroyed(const QByteArray &data);
	void receiveSearchMessages(int count, const QString &query);
	void receivePublicMessages(int count);
	void receiveFriendsMessages(int count);
	void receiveReplies(int count);
	void receiveFavorites();
	void receiveInboxMessages(int count);
	void receiveOutboxMessages(int count);
	void receivePreviousSearchMessages(int count, const QString &query);
	void receivePreviousPublicMessages(int count);
	void receivePreviousFriendsMessages(int count);
	void receivePreviousReplies(int count);
	void receivePreviousFavorites();
	void receivePreviousInboxMessages(int count);
	void receivePreviousOutboxMessages(int count);
	void removePreviousSearchMessages(int count);
	void removePreviousFriendsMessages(int count);
	void removePreviousReplies(int count);
	void removePreviousFavorites();
	void removePreviousInboxMessages(int count);
	void removePreviousOutboxMessages(int count);
	void sendDirectMessage(const QString &username, const QString &message);
	void favorMessage(const Message &message);
	void unfavorMessage(const Message &message);
	void destroyMessage(const Message &message);
	
signals:
	void friendsMessagesUpdated(const QVector<Message> &, Account *);
	void repliesUpdated(const QVector<Message> &, Account *);
	void publicMessagesUpdated(const QVector<Message> &, Account *);
	void favoritesUpdated(const QVector<Message> &, Account *);
	void searchMessagesUpdated(const QVector<Message> &, Account *);
	void lastMessageReceived(const QString &, Account *);
	void messageSent(const QString &, Account *);
	void messageNotSent(Account *);
	void newMessagesReceived(const QVector<Message> &, Account *);
	void inboxMessagesUpdated(const QVector<Message> &, Account *);
	void outboxMessagesUpdated(const QVector<Message> &, Account *);
	void remainingRequestsUpdated(int, Account *);
	void previousFriendsMessagesReceived();
	void previousSearchMessagesReceived();
	void previousRepliesReceived();
	void previousPublicMessagesReceived();
	void previousFavoritesReceived();
	void previousInboxMessagesReceived();
	void previousOutboxMessagesReceived();
};

#endif
