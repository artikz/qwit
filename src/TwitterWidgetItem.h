/*  This file is part of Qwit.

	Copyright (C) 2008, 2009 Artem Iglikov, Mathias Panzenböck

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

#ifndef TwitterWidgetItem_H
#define TwitterWidgetItem_H

#include <QWidget>
#include <QTextBrowser>
#include <QLabel>
#include <QDateTime>
#include <QSharedData>

class TwitterWidgetItemData : public QSharedData {
public:
	TwitterWidgetItemData() :
		QSharedData(),
		status(NULL),
		icon(NULL),
		sign(NULL),
		contrl(NULL),
		messageId(0),
		top(0),
		height(0),
		reply(false),
		cacheMessageId(0),
		cacheReplyStatusId(0),
		cacheIndex(-1) {}

	TwitterWidgetItemData(const TwitterWidgetItemData &other) :
		QSharedData(other),
		status(other.status),
		icon(other.icon),
		sign(other.sign),
		contrl(other.contrl),
		iconFileName(other.iconFileName),
		time(other.time),
		username(other.username),
		messageId(other.messageId),
		top(other.top),
		height(other.height),
		color(other.color),
		cleanStatus(other.cleanStatus),
		reply(other.reply),
		cacheUserpic(other.cacheUserpic),
		cacheUsername(other.cacheUsername),
		cacheStatus(other.cacheStatus),
		cacheTime(other.cacheTime),
		cacheMessageId(other.cacheMessageId),
		cacheReplyStatusId(other.cacheReplyStatusId),
		cacheIndex(other.cacheIndex) {}

	virtual ~TwitterWidgetItemData();

	QTextBrowser *status;
	QLabel *icon;
	QLabel *sign;
	QLabel *contrl;
	QString iconFileName;
	QDateTime time;
	QString username;
	quint64 messageId;

	int top;
	int height;
	QColor color;

	QString cleanStatus;

	bool reply;

	QString cacheUserpic;
	QString cacheUsername;
	QString cacheStatus;
	QDateTime cacheTime;
	quint64 cacheMessageId;
	quint64 cacheReplyStatusId;
	int cacheIndex;
};

class TwitterWidgetItem
{
public:
	TwitterWidgetItem() : d(new TwitterWidgetItemData()) {}

	TwitterWidgetItem(const TwitterWidgetItem &other) : d(other.d) {}

	TwitterWidgetItem(
		QWidget *twitterWidget,
		const QString &userpic,
		const QString &username,
		const QString &statusText,
		const QDateTime &time,
		quint64 messageId,
		quint64 replyStatusId,
		int i,
		const QString &serviceBaseURL,
		const QString &currentUsername);

	~TwitterWidgetItem() {}

	void loadIcon();
	void show();
	int update(int twitterWidgetWidth, int statusItemWidth, int top, bool odd, bool usernameUnderAvatar, bool verticalAlignControl);
	void paint(QPainter &painter, QPalette palette, int width);

	QString cacheUserpic()    const { return d->cacheUserpic; }
	QString cacheStatus()     const { return d->cacheStatus; }
	QString cacheUsername()   const { return d->cacheUsername; }
	QDateTime cacheTime()     const { return d->cacheTime; }
	quint64 cacheMessageId()     const { return d->cacheMessageId; }
	quint64 cacheReplyStatusId() const { return d->cacheReplyStatusId; }
	int cacheIndex()          const { return d->cacheIndex; }
	QString iconFileName()    const { return d->iconFileName; }
	static bool isUsernameChar(const QChar &c);
	static QString prepare(const QString &text, const quint64 &replyStatusId, const QString &serviceBaseURL);

private:
	QExplicitlySharedDataPointer<TwitterWidgetItemData> d;
	static bool isReplyTo(const QString &text, const QString &username);
	static bool isHashtagChar(const QChar &c);
};

#endif // TwitterWidgetItem_H
