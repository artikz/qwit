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

#ifndef Twitter_cpp
#define Twitter_cpp

#include <QUrl>
#include <QHttpRequestHeader>
#include <QByteArray>
#include <QDateTime>
#include <QNetworkProxy>

#include "Twitter.h"

#include <iostream>

using namespace std;

Twitter::Twitter() {
	urls[0] = HOME_XML_URL;
	urls[1] = PUBLIC_XML_URL;
	urls[2] = REPLIES_XML_URL;
	urls[3] = "";
	urls[4] = INPUT_DIRECT_XML_URL;
	urls[5] = OUTPUT_DIRECT_XML_URL;
	urls[6] = SEARCH_ATOM_URL;
        urls[7] = FRIENDS_XML_URL;
	urls[8] = FOLLOWERS_XML_URL;
	urls[9] = BLOCKED_XML_URL;
	urls[10] = FOLLOW_USER_XML_URL;
	urls[11] = UNFOLLOW_USER_XML_URL;
	urls[12] = BLOCK_USER_XML;
	urls[13] = UNBLOCK_USER_XML;
	proxyAddress = "";
	connect(&statusHttp, SIGNAL(done(bool)), this, SLOT(statusHttpDone(bool)));
	connect(&timelineHttp, SIGNAL(done(bool)), this, SLOT(timelineHttpDone(bool)));
	connect(&timelineHttp, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(httpsError(const QList<QSslError> &)));
	connect(&statusHttp, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(httpsError(const QList<QSslError> &)));

	connect(&friendshipsHttp, SIGNAL(done(bool)), this, SLOT(friendshipsHttpDone(bool)));
	connect(&friendsMgmtHttp, SIGNAL(done(bool)), this, SLOT(friendsMgmtHttpDone(bool)));
}

void Twitter::setServiceBaseURL(const QString &url) {
	serviceBaseURL = url;
}

void Twitter::setServiceAPIURL(const QString &url) {
	serviceAPIURL = url;
}

QString Twitter::getServiceBaseURL() {
	return serviceBaseURL;
}

QString Twitter::getServiceAPIURL() {
	return serviceAPIURL;
}

void Twitter::useProxy(const QString &address, int port, const QString &username, const QString &password) {
	proxyAddress = address;
	proxyPort = port;
	proxyUsername = username;
	proxyPassword = password;
}

void Twitter::dontUseProxy() {
	proxyAddress = "";
}

void Twitter::sendStatus(QString username, QString password, QString status, QString replyID) {
	QUrl url(serviceAPIURL + STATUS_UPDATE_URL);

	QHttpRequestHeader header;
	header.setRequest("POST", url.path());
	header.setValue("Host", url.host());
	header.setContentType("application/x-www-form-urlencoded");

	setupConnection(&statusHttp, &url, username, password);

	QByteArray data = "status=";
	data += QUrl::toPercentEncoding(status);
        if (replyID != "") {
                data += "&in_reply_to_status_id=";
                data += replyID;
        }
	data += "&source=qwit";
	statusHttp.request(header, data);

	emit stateChanged(tr("Sending status: %1").arg(serviceAPIURL + STATUS_UPDATE_URL));
}

void Twitter::update(QString username, QString password, quint64 lastStatusId, int type, int count) {
	if (urls[type] == "") {
		cerr << "No url defined" << endl;
		return;
	}

	if (timelineHttp.state() != QHttp::Unconnected) {
		timelineHttp.abort();
	}

	currentType = type;
	QUrl url;
	if (type == 6) {
		url=("http://search.twitter.com" + urls[type]);
	} else {
		url=(serviceAPIURL + urls[type]);
	}

	setupConnection(&timelineHttp, &url, username, password);

	buffer.open(QIODevice::WriteOnly);

	if (type == 6) {
		timelineHttp.get(urls[type] + "&rpp=" + QString::number(count), &buffer);

		emit stateChanged(tr("Updating timeline: %1").arg("http://search.twitter.com" + urls[type] + "&rpp=" + QString::number(count)));
	} else {
		timelineHttp.get(url.path() + "?count=" + QString::number(count) + (lastStatusId ? "&since_id=" + QString::number(lastStatusId) : ""), &buffer);

		emit stateChanged(tr("Updating timeline: %1").arg(serviceAPIURL + urls[type]));
	}
}

void Twitter::statusHttpDone(bool error) {
	if (error) {
		emit stateChanged(tr("Error sending status: %1").arg(statusHttp.errorString()));
		return;
	}
	emit stateChanged(tr("Status sent: %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
	emit statusUpdated();
}

void Twitter::timelineHttpDone(bool error) {
	if (error) {
		emit stateChanged(tr("Error while updating timeline: %1").arg(timelineHttp.errorString()));
		return;
	}
	buffer.close();
	emit stateChanged(tr("Timeline updated: %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
	emit updated(buffer.data(), currentType);
}

void Twitter::friendshipsHttpDone(bool error)
{
    if(error)
    {
	cerr << friendshipsHttp.errorString().toStdString() << endl;
        // FIXME emit statusInformation
        return;
    }
    friendshipsBuffer.close();

    emit friendshipsUpdated(friendshipsBuffer.data(), currentType);
}

void Twitter::friendsMgmtHttpDone(bool error)
{
    if(error)
    {
	cerr << friendsMgmtBuffer.errorString().toStdString() << endl;
	return;
    }

    friendsMgmtBuffer.close();

    emit friendsMgmtEvent(friendsMgmtBuffer.data(), currentType);
}

void Twitter::httpsError(const QList<QSslError> & errors) {
    /*
     * maybe here should be implemtend some error handling
     * to just ignore all ssl errors (not recommended):
     *  timelineHttp.ignoreSslErrors();
     */
    emit stateChanged(tr("SSL Error while updating/sending: %1").arg(errors[0].errorString()));
}


void Twitter::abort() {
	timelineHttp.abort();
	statusHttp.abort();
	friendshipsHttp.abort();
	friendsMgmtHttp.abort();
}

void Twitter::setUrl(int index, const QString &url) {
	urls[index] = url;
	emit stateChanged(url);
}

void Twitter::getFriendships(QString username, QString password, int type)
{
    // infer url typ from the current tab widget index
    type = type +  7;

    if(urls[type] == "")
    {
        cerr << "No url defined" << endl;
        return;
    }

    if(friendshipsHttp.state() != QHttp::Unconnected)
    {
	friendshipsHttp.abort();
    }

    currentType = type;
    QUrl url;
    if((type == 7) || (type == 8) || (type == 9))
    {
        url=(serviceAPIURL + urls[type]);
    }
    else
    {
        cerr << "Unexpected url type: " << type << " received!" << endl;
        return;
    }

    setupConnection(&friendshipsHttp, &url, username, password);

    friendshipsBuffer.open(QIODevice::WriteOnly);

    friendshipsHttp.get(urls[type], &friendshipsBuffer);
}

void Twitter::createFriendship(QString screenName, QString username, QString password)
{
    int type = 10;
    currentType = type;
    QUrl url(serviceBaseURL + urls[type]);

    setupConnection(&friendsMgmtHttp, &url, username, password);

    friendsMgmtBuffer.open(QIODevice::WriteOnly);

    QByteArray data = "screen_name=";
    data += screenName;
    friendsMgmtHttp.post(url.path(), data, &friendsMgmtBuffer);
}

void Twitter::setupConnection(QHttp *qHttp, QUrl *url, QString username, QString password)
{
    if (proxyAddress != "") {
		qHttp->setProxy(proxyAddress, proxyPort, proxyUsername, proxyPassword);
	} else {
		qHttp->setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
	}
    /* FIXME
     * https (ssl) mode is not working if setProxy is called with ("", 0)
     *   } else {
     *	  timelineHttp.setProxy("", 0);
     *   }
     */

    if(url->toString().indexOf("https") == 0) {
	    qHttp->setHost(url->host(), QHttp::ConnectionModeHttps, 443);
    }
    else {
	qHttp->setHost(url->host(), url->port(80));
    }

	qHttp->setUser(username, password);
}

void Twitter::destroyFriendship(QString screenName, QString username, QString password)
{
    int type = 11;
    currentType = type;

    QUrl url(serviceBaseURL + urls[type]);

    setupConnection(&friendsMgmtHttp, &url, username, password);

    friendsMgmtBuffer.open(QIODevice::WriteOnly);

    QByteArray data = "screen_name=";
    data += screenName;
    friendsMgmtHttp.post(url.path(), data, &friendsMgmtBuffer);
}

void Twitter::createBlock(QString screenName, QString username, QString password)
{
    int type = 12;
    currentType = type;

    QUrl url(serviceBaseURL + urls[type] + screenName + ".xml");

    setupConnection(&friendsMgmtHttp, &url, username, password);

    friendsMgmtBuffer.open(QIODevice::WriteOnly);
    QByteArray data = "";
    friendsMgmtHttp.post(url.path(), data, &friendsMgmtBuffer);
}

void Twitter::destroyBlock(QString screenName, QString username, QString password)
{
    int type = 13;
    currentType = type;

    QUrl url(serviceBaseURL + urls[type] + screenName + ".xml");

    setupConnection(&friendsMgmtHttp, &url, username, password);

    friendsMgmtBuffer.open(QIODevice::WriteOnly);
    QByteArray data = "";
    friendsMgmtHttp.post(url.path(), data, &friendsMgmtBuffer);
}
#endif
