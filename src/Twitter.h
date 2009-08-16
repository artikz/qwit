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

#ifndef Twitter_h
#define Twitter_h

#include <QObject>
#include <QHttp>
#include <QString>
#include <QBuffer>
#include <QSslError>

#define URL_COUNT 14
#define HOME_XML_URL "/statuses/friends_timeline.xml"
#define PUBLIC_XML_URL "/statuses/public_timeline.xml"
#define REPLIES_XML_URL "/statuses/replies.xml"
#define CUSTOM_XML_URL "/statuses/user_timeline/"
#define INPUT_DIRECT_XML_URL "/direct_messages.xml"
#define OUTPUT_DIRECT_XML_URL "/direct_messages/sent.xml"
#define SEARCH_ATOM_URL "/search.atom"
#define FRIENDS_XML_URL "/statuses/friends.xml"
#define FOLLOWERS_XML_URL "/statuses/followers.xml"
#define BLOCKED_XML_URL "/blocks/blocking.xml"
#define FOLLOW_USER_XML_URL "/friendships/create.xml"
#define UNFOLLOW_USER_XML_URL "/friendships/destroy.xml"
#define BLOCK_USER_XML "/blocks/create/"
#define UNBLOCK_USER_XML "/blocks/destroy/"
//http://search.twitter.com/search.atom?q=twitter

#define STATUS_UPDATE_URL "/statuses/update.xml"

// #define HOME_XML_URL "http://twitter.com/statuses/friends_timeline.xml"
// #define PUBLIC_XML_URL "http://twitter.com/statuses/public_timeline.xml"
// #define REPLIES_XML_URL "http://twitter.com/statuses/replies.xml"
// #define CUSTOM_XML_URL "http://twitter.com/statuses/user_timeline/"
//
// #define STATUS_UPDATE_URL "http://twitter.com/statuses/update.xml"

class Twitter: public QObject {
	Q_OBJECT

	QHttp statusHttp;
	QHttp timelineHttp;
	QHttp friendshipsHttp;
	QHttp friendsMgmtHttp;
	QBuffer buffer;
	QBuffer friendshipsBuffer;
	QBuffer friendsMgmtBuffer;
	QString proxyAddress;
	int proxyPort;
	QString proxyUsername;
	QString proxyPassword;
	int currentType;
	QString urls[URL_COUNT];
	QString serviceBaseURL;
	QString serviceAPIURL;

public:

	Twitter();
	void useProxy(const QString &address, int port, const QString &username, const QString &password);
	void dontUseProxy();
	void sendStatus(QString username, QString password, QString status, QString replyID);
	void update(QString username, QString password, quint64 lastStatusId, int type, int count);
	void setUrl(int index, const QString &url);
	void abort();

	void getFriendships(QString username, QString password, int type);
	void createFriendship(QString screenName, QString username, QString password);
	void destroyFriendship(QString screenName, QString username, QString password);
	void createBlock(QString screenName, QString username, QString password);
	void destroyBlock(QString screenName, QString username, QString password);

	QString getServiceBaseURL();
	QString getServiceAPIURL();
	void setServiceBaseURL(const QString &url);
	void setServiceAPIURL(const QString &url);

public slots:

	void statusHttpDone(bool error);
	void timelineHttpDone(bool error);
	void httpsError(const QList<QSslError> & errors);
	void friendshipsHttpDone(bool error);
	void friendsMgmtHttpDone(bool error);

signals:

	void updated(const QByteArray &, int);
	void statusUpdated();
	void stateChanged(const QString &);
	void friendshipsUpdated(const QByteArray &, int);
	void friendsMgmtEvent(const QByteArray &, int);

private:
	void setupConnection(QHttp *qHttp, QUrl *url, QString username, QString password);
};

#endif
