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

#ifndef TwitterWidget_h
#define TwitterWidget_h

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTextBrowser>
#include <QLabel>
#include <QDateTime>
#include <QScrollArea>

#include "TwitterWidgetItem.h"

const int ICON_SIZE = 48;
const int MARGIN = 5;

class TwitterWidget: public QWidget {
	Q_OBJECT

private:
	int messagesPerPage;
	bool usernameUnderAvatar;
	bool verticalAlignControl;
	QScrollArea *scrollArea;
	int scrollPosition;

	QVector<TwitterWidgetItem> items;
	QString serviceBaseURL;

public:

	TwitterWidget(QScrollArea *scrollArea);
	void clear();
	void addItem(const QString &userpic, const QString &username, const QString &status, const QDateTime &time, quint64 messageId, quint64 replyStatusId, int i, const QString &serviceBaseURL, const QString &currentUsername);
	void updateItems();
	const TwitterWidgetItem getItem(int index);
	int getItemsCount();
	void setMessagesPerPage(int value);
	void setUsernameUnderAvatar(bool value);
	void setVerticalAlignControl(bool value);
	void saveScrollPosition();
	void restoreScrollPosition();

	static QString formatDateTime(const QDateTime &time);

protected:

	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);

public slots:

	void replyClicked(const QUrl &url);
	void directMessagesClicked(const QUrl &url);
	void twitterClicked(const QUrl &url);
	void reloadUserpic(const QString &userpic);

signals:

	void reply(const QString &);
	void retweet(const QString &, const QString &);
	void directMessages(const QString &);
	void replyID(const QString &);
	void directMessagesID(const QString &);
	void openUser(const QString &);
	void openSearch(const QString &);
};

#endif
