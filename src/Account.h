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
	QStringList usernamesCache;

public:
    QString _serviceBaseUrl;
    QString _serviceApiUrl;
    QString _searchBaseUrl;
    QString _searchApiUrl;
    QString _singleMessageUrl;

    int id;
    QString type;
    QString username;
    QString password;
    QString oauthToken;
    QString oauthTokenSecret;
    bool useOAuth;
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
    QVector<Message> friendships;
    QVector<Message> followers;
    QVector<Message> blocks;

    Account();
    Account(const QString &type, const QString &username, const QString &password, bool useHttps = false, const QString &serviceBaseUrl = "", const QString &serviceApiUrl = "", bool useOAuth = false, const QString &oauthToken = "", const QString &oauthTokenSecret = "");
    void saveMessages(QSettings &messagesCache);
    void loadMessages(QSettings &messagesCache);
    QString serviceApiUrl();
    QString serviceBaseUrl();
    QString searchBaseUrl();
    QString searchApiUrl();
    QString singleMessageUrl();
    QOAuth::Interface *serviceOAuth();
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
	void receiveSearchMessages(int count, const QString &query, bool initial = false);
	void receivePublicMessages(int count, bool initial = false);
	void receiveFriendsMessages(int count, bool initial = false);
	void receiveReplies(int count, bool initial = false);
	void receiveFavorites(bool initial = false);
	void receiveInboxMessages(int count, bool initial = false);
	void receiveOutboxMessages(int count, bool initial = false);
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
	void receiveFriendships();
	void receiveFollowers();
	void receiveBlocks();
	void createFriendship(QString screenName, uint requestId = 0);
	void destroyFriendship(QString screenName, uint requestId);
	void createBlock(QString screenName, uint requestId);
	void destroyBlock(QString screenName, uint requestId);

    private slots:
	void updateFriendships(const QByteArray &data);
	void updateFollowers(const QByteArray &data);
	void updateBlocks(const QByteArray &data);
	void addFriendship(const QByteArray &data, uint requestId);
	void removeFriendship(const QByteArray &data, uint requestId);
	void addBlock(const QByteArray &data, uint requestId);
	void removeBlock(const QByteArray &data, uint requestId);
	
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
	// redirect incoming signal to the dedicated element
	void friendshipsUpdated(const QVector<Message> &);
	void followersUpdated(const QVector<Message> &);
	void blocksUpdated(const QVector<Message> &);
	void friendshipAdded(const Message&, uint);
	void friendshipRemoved(const Message&, uint);
	void blockAdded(const Message&, uint);
	void blockRemoved(const Message&, uint);
};

#endif
