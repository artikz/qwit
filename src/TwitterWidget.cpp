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

#ifndef TwitterWidget_cpp
#define TwitterWidget_cpp

#include <QScrollBar>
#include <QDesktopServices>
#include <QPalette>
#include <QChar>
#include <QScrollArea>

#include "TwitterWidget.h"
#include "QwitException.h"

#include <iostream>

using namespace std;

TwitterWidget::TwitterWidget(QScrollArea *scrollArea): QWidget() {
	this->scrollArea = scrollArea;
	QDesktopServices::setUrlHandler("twitter", this, "twitterClicked");
	QDesktopServices::setUrlHandler("reply", this, "replyClicked");
	QDesktopServices::setUrlHandler("directMessages", this, "directMessagesClicked");
}

void TwitterWidget::clear() {
	items.clear();
}

void TwitterWidget::addItem(const QString &userpic, const QString &username, const QString &status, const QDateTime &time, quint64 messageId,
		quint64 replyStatusId, int i, const QString &serviceBaseURL, const QString &currentUsername) {
// Possible fix for issue 55
	for (int j = 0; j < items.size(); ++j) {
		if (items[j].cacheMessageId() == messageId) {
			return;
		}
	}

	TwitterWidgetItem item = TwitterWidgetItem(
		this,
		userpic,
		username,
		status,
		time,
		messageId,
		replyStatusId,
		i,
		serviceBaseURL,
		currentUsername);

	if (i == -1) {
		items.push_back(item);
		cout << "added item" << endl;
	} else {
		if (i < 0) i = 0;
		else if (i > items.size()) i = items.size();
		items.insert(i, item);
		cout << "added item" << endl;
	}

	item.show();

	while (items.size() > messagesPerPage) {
		items.pop_back();
		cout << "popped item" << endl;
	}

	updateItems();
}

void TwitterWidget::updateItems() {
	int height = 0;
	int statusItemWidth = width() - (ICON_SIZE + 5 * MARGIN);
	for (int i = 0; i < items.size(); ++ i) {
		height += items[i].update(width(), statusItemWidth, height, (i & 1), usernameUnderAvatar, verticalAlignControl);
	}
	resize(width(), height);
}

void TwitterWidget::resizeEvent(QResizeEvent *event) {
	if (event->oldSize() == event->size()) {
		event->ignore();
		return;
	}
	updateItems();
	event->accept();
}

void TwitterWidget::replyClicked(const QUrl &url) {
	if (url.hasQueryItem("status")) {
		emit retweet(QUrl::fromPercentEncoding((url.queryItemValue("user")).toAscii()), QUrl::fromPercentEncoding((url.queryItemValue("status")).toAscii()));
	}
	else emit reply(QUrl::fromPercentEncoding((url.queryItemValue("user")).toAscii()));
	emit replyID(QString::number((quint64)url.port()));
}

void TwitterWidget::directMessagesClicked(const QUrl &url) {
	emit directMessages(url.host());
        //emit directMessagesID(QString::number(url.port()));
}

void TwitterWidget::twitterClicked(const QUrl &url) {
    if (url.host() == "user") {
        emit openUser(url.path().mid(1));
    }
    else if ((url.host() == "search") && url.hasQueryItem("q")) {
        emit openSearch(url.queryItemValue("q"));
    }
    else {
        qWarning("unknown action for twitter scheme: %s", qPrintable(url.host()));
    }
}

void TwitterWidget::reloadUserpic(const QString &userpic) {
	for (int i = 0; i < items.size(); ++i) if (items[i].iconFileName() == userpic) {
		TwitterWidgetItem &item = items[i];
		item.loadIcon();
//		item.icon->resize(ICON_SIZE, ICON_SIZE);
	}
}

void TwitterWidget::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	for (int i = 0; i < items.size(); ++i) {
		items[i].paint(painter, palette(), width());
	}
	event->accept();
}


QString TwitterWidget::formatDateTime(const QDateTime &time) {
	int seconds = time.secsTo(QDateTime::currentDateTime());
	if (seconds <= 15) return tr("Just now");
	if (seconds <= 45) return tr("about %n second(s) ago", "", seconds);
	int minutes = (seconds - 45 + 59) / 60;
	if (minutes <= 45) return tr("about %n minute(s) ago", "", minutes);
	int hours = (seconds - 45 * 60 + 3599) / 3600;
	if (hours <= 18) return tr("about %n hour(s) ago", "", hours);
	int days = (seconds - 18 * 3600 + 24 * 3600 - 1) / (24 * 3600);
	return tr("about %n day(s) ago", "", days);
}

const TwitterWidgetItem TwitterWidget::getItem(int index) {
// Possible fix for issue 10
	if (index >= items.size()) {
		throw QwitException("Trying to get a nonexisting item from TwitterWidget::items");
	}
	return items[index];
}

int TwitterWidget::getItemsCount() {
	return items.size();
}

void TwitterWidget::setMessagesPerPage(int value) {
	messagesPerPage = value;
}

void TwitterWidget::setUsernameUnderAvatar(bool value) {
	usernameUnderAvatar = value;
}

void TwitterWidget::setVerticalAlignControl(bool value) {
	verticalAlignControl = value;
}

void TwitterWidget::saveScrollPosition() {
	scrollPosition = scrollArea->horizontalScrollBar()->value();
}

void TwitterWidget::restoreScrollPosition() {
	scrollArea->horizontalScrollBar()->setValue(scrollPosition);
}

#endif
