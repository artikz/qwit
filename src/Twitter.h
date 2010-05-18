/*! 
 *  $Id: Twitter.h 207 2009-07-24 18:05:05Z artem.iglikov $
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
 *  Twitter class declaration
 */

#ifndef Twitter_h
#define Twitter_h

#include "QwitHeaders.h"

#include "Account.h"

class Account;

class Twitter: public QObject {
	Q_OBJECT
	
	QBuffer buffer;
	QHttp *http;
	Account *account;
	QMap<int, QString> sendMessageRequests;
	QMap<int, QString> receiveFriendsMessagesRequests;
	QMap<int, QString> receiveRepliesRequests;
	QMap<int, QString> receivePublicMessagesRequests;
	QMap<int, QString> receiveFavoritesRequests;
	QMap<int, QString> receiveInboxMessagesRequests;
	QMap<int, QString> receiveOutboxMessagesRequests;
	QMap<int, QString> receiveSearchMessagesRequests;
	QMap<int, QString> receiveLastMessageRequests;
	QMap<int, QString> receivePreviousFriendsMessagesRequests;
	QMap<int, QString> receivePreviousRepliesRequests;
	QMap<int, QString> receivePreviousPublicMessagesRequests;
	QMap<int, QString> receivePreviousFavoritesRequests;
	QMap<int, QString> receivePreviousInboxMessagesRequests;
	QMap<int, QString> receivePreviousOutboxMessagesRequests;
	QMap<int, QString> receivePreviousSearchMessagesRequests;
	QMap<int, QString> sendDirectMessageRequests;
	QMap<int, QString> favorMessageRequests;
	QMap<int, QString> unfavorMessageRequests;
	QMap<int, QString> destroyMessageRequests;
	QMap<int, QString> destroyDirectMessageRequests;
	QMap<int, QString> receiveFriendshipsRequests;
	QMap<int, QString> receiveFollowersRequests;
	QMap<int, QString> receiveBlocksRequests;
	QMap<int, QString> createFriendshipRequests;
	QMap<int, QString> destroyFriendshipRequests;
	QMap<int, QString> createBlockRequests;
	QMap<int, QString> destroyBlockRequests;
	QMap<int, uint> httpRequestId2InternalRequestId;

public:

	Twitter(Account *account);
	void receiveFriendsMessages(quint64 lastMessageId, int count);
	void receiveReplies(quint64 lastMessageId, int count);
	void receivePublicMessages(quint64 lastMessageId, int count);
	void receiveInboxMessages(quint64 lastMessageId, int count);
	void receiveOutboxMessages(quint64 lastMessageId, int count);
	void receiveSearchMessages(int count, const QString &query);
	void receiveFavorites();
	void receiveLastMessage();
	void receivePreviousFriendsMessages(quint64 lastMessageId, int count);
	void receivePreviousReplies(quint64 lastMessageId, int count);
	void receivePreviousPublicMessages(quint64 lastMessageId, int count);
	void receivePreviousInboxMessages(quint64 lastMessageId, int count);
	void receivePreviousOutboxMessages(quint64 lastMessageId, int count);
	void receivePreviousFavorites(int page);
	void receivePreviousSearchMessages(int page, int count, const QString &query);
	void sendMessage(const QString &message, quint64 inReplyToMessageId);
	void sendDirectMessage(const QString &username, const QString &message);
	void favorMessage(quint64 messageId);
	void unfavorMessage(quint64 messageId);
	void destroyMessage(quint64 messageId);
	void destroyDirectMessage(quint64 messageId);
	void receiveFriendships();
	void receiveFollowers();
	void receiveBlocks();
	void createFriendship(QString screenName, uint requestId);
	void destroyFriendship(QString screenName, uint requestId);
	void createBlock(QString screenName, uint requestId);
	void destroyBlock(QString screenName, uint requestId);
	void abort();

signals:

	void friendsMessagesReceived(const QByteArray &);
	void repliesReceived(const QByteArray &);
	void publicMessagesReceived(const QByteArray &);
	void lastMessageReceived(const QByteArray &);
	void favoritesReceived(const QByteArray &);
	void inboxMessagesReceived(const QByteArray &);
	void outboxMessagesReceived(const QByteArray &);
	void searchMessagesReceived(const QByteArray &);
	void messageSent(const QByteArray &);
	void messageNotSent();
	void directMessageSent(const QByteArray &);
	void messageFavored(const QByteArray &);
	void messageUnfavored(const QByteArray &);
	void messageDestroyed(const QByteArray &);
	void directMessageDestroyed(const QByteArray &);
	void previousFriendsMessagesReceived(const QByteArray &);
	void previousRepliesReceived(const QByteArray &);
	void previousPublicMessagesReceived(const QByteArray &);
	void previousFavoritesReceived(const QByteArray &);
	void previousInboxMessagesReceived(const QByteArray &);
	void previousOutboxMessagesReceived(const QByteArray &);
	void previousSearchMessagesReceived(const QByteArray &);
	void friendshipsReceived(const QByteArray &);
	void followersReceived(const QByteArray &);
	void blocksReceived(const QByteArray &);
	void friendshipCreated(const QByteArray &, uint);
	void friendshipDestroyed(const QByteArray &, uint);
	void blockCreated(const QByteArray &, uint);
	void blockDestroyed(const QByteArray &, uint);

public slots:

	void requestStarted(int id);
	void requestFinished(int id, bool error);
	void sslErrors(const QList<QSslError> &errors);
	
};

#endif
