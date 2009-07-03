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

#include <QScrollBar>

#include "TwitterWidget.h"
#include "TwitterWidgetItem.h"

TwitterWidgetItemData::~TwitterWidgetItemData() {
	delete status;
	delete icon;
	delete sign;
	delete contrl;
}

TwitterWidgetItem::TwitterWidgetItem(
		QWidget *twitterWidget,
		const QString &userpic,
		const QString &username,
		const QString &statusText,
		const QDateTime &time,
		uint messageId,
		uint replyStatusId,
		int i,
		const QString &serviceBaseURL,
		const QString &currentUsername) {
	d = new TwitterWidgetItemData();
	d->time = time;
	d->username = username;
	d->messageId = messageId;

// Save raw info for caching between sessions
	d->cacheUserpic = userpic;
	d->cacheUsername = username;
	d->cacheStatus = statusText;
	d->cacheTime = time;
	d->cacheMessageId = messageId;
	d->cacheReplyStatusId = replyStatusId;
	d->cacheIndex = i;

	d->cleanStatus = statusText;
	d->reply = isReplyTo(statusText, currentUsername);

	QTextBrowser *status = d->status = new QTextBrowser(twitterWidget);
	d->icon = new QLabel(twitterWidget);
	d->iconFileName = userpic;
	QLabel *sign = d->sign = new QLabel(twitterWidget);
	QLabel *contrl = d->contrl = new QLabel(twitterWidget);

	status->setHtml(prepare(statusText, replyStatusId, serviceBaseURL));
	status->setReadOnly(true);
	status->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	status->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	status->setFrameShape(QFrame::NoFrame);
	status->setOpenExternalLinks(true);

//	QFont font = status->document()->defaultFont();
//	font.setFamily("Verdana");
//	status->document()->setDefaultFont(font);

	QByteArray encodedStatusText(QUrl::toPercentEncoding(statusText));
	QString messageIdText(QString::number(messageId));
	QString formattedTime(TwitterWidget::formatDateTime(time));
	QString labelText("<a href=\"twitter://user/" + username + "\" style=\"font-weight:bold;text-decoration:none\">" +
		username + "</a> - <a href=\"http://twitter.com/" + username + "/statuses/" + messageIdText +
		"\" style=\"font-size:70%;text-decoration:none\">" + formattedTime + "</a> <a href=\"directMessages://" +
		username + ":" + messageIdText + "\" style=\"text-decoration:none\"><img src=\":/images/dms.png\"/></a> <a href=\"reply://tweet:" +
		messageIdText + "/q?user=" + username + "\" style=\"text-decoration:none\"><img src=\":/images/reply.png\"/></a> <a href=\"reply://tweet:" +
		messageIdText + "/q?user=" + username + "&status=" + encodedStatusText +
		 "\" style=\"text-decoration:none\"><img src=\":/images/rt.png\"/></a>");

	loadIcon();
	sign->setText(labelText);
	sign->setAlignment(Qt::AlignRight);
	sign->setOpenExternalLinks(true);
	contrl->setText(labelText);
	contrl->setAlignment(Qt::AlignRight);
	contrl->setOpenExternalLinks(true);
}

void TwitterWidgetItem::loadIcon() {
	QPixmap pixmap(d->iconFileName);
	if (!pixmap.isNull()) {
		d->icon->setPixmap(pixmap.scaled(ICON_SIZE, ICON_SIZE));
	}
	d->icon->resize(ICON_SIZE, ICON_SIZE);
}

void TwitterWidgetItem::show() {
	d->status->show();
	d->icon->show();
	d->sign->show();
	d->contrl->show();
}

bool TwitterWidgetItem::isReplyTo(const QString &text, const QString &username) {
	return (text.indexOf("@" + username) != -1);
}

bool TwitterWidgetItem::isUsernameChar(const QChar &c) {
	return c.isLetterOrNumber() || c == '_';
}

QString TwitterWidgetItem::prepare(const QString &text, const uint &replyStatusId, const QString &serviceBaseURL) {
	QString s = text;
	s.replace(" www.", " http://www.");
	if (s.startsWith("www.")) s = "http://" + s;
	QString t = "";
	int i = 0, j = 0;
	while ((j = s.indexOf("http://", i)) != -1) {
		t += s.mid(i, j - i);
		int k = s.indexOf(" ", j);
		if (k == -1) k = s.length();
		QString url = s.mid(j, k - j);
		t += "<a href=\"" + url + "\" style=\"text-decoration:none\">" + url + "</a>";
		i = k;
	}
	t += s.mid(i);
	if (replyStatusId && (t[0] == '@')) {
		s = t;
		int i = 1;
		while ((i < s.length()) && (QChar(s[i]).isLetterOrNumber() || (s[i] == '_'))) ++i;
		QString username = s.mid(1, i - 1);
		t = "@<a href=\"" + serviceBaseURL + "/" + username + "/statuses/" + QString::number(replyStatusId) + "\" style=\"text-decoration:none;font-weight:bold;\">" + username + "</a>" + s.mid(i);
	}
	s = t;
	t = "";
	for (int i = 0; i < s.length(); ++i) {
		t += s[i];
		if ((s[i] == '@') && (!i || !isUsernameChar(s[i - 1]))) {
			int j = i + 1;
			while ((j < s.length()) && isUsernameChar(s[j])) {
				++j;
			}
			if (j - i - 1 > 0) {
				QString username = s.mid(i + 1, j - i - 1);
				t += "<a href=\"twitter://user/" + username + "\" style=\"text-decoration:none;font-weight:bold;\">" + username + "</a>";
				i = j - 1;
			}
		}
	}
	s = t;
	t = "";
	for (int i = 0; i < s.length(); ++i) {
		t += s[i];
		if ((s[i] == '#') && (!i || !isUsernameChar(s[i - 1]))) {
			int j = i + 1;
			while ((j < s.length()) && isUsernameChar(s[j])) {
				++j;
			}
			if (j - i - 1 > 0) {
				QString hashtag = s.mid(i + 1, j - i - 1);
				// t += "<a href=\"http://search.twitter.com/search?q=" + QUrl::toPercentEncoding("#" + hashtag) + "\" style=\"text-decoration:none;font-weight:bold;\">" + hashtag + "</a>";
				t += "<a href=\"twitter://search/?q=" + QUrl::toPercentEncoding("#" + hashtag) + "\" style=\"text-decoration:none;font-weight:bold;\">" + hashtag + "</a>";
				i = j - 1;
			}
		}
	}
	return t;
}

int TwitterWidgetItem::update(int twitterWidgetWidth, int statusItemWidth, int top, bool odd, bool usernameUnderAvatar, bool verticalAlignControl) {
	QTextBrowser *status = d->status;
	QLabel *icon = d->icon;
	QLabel *sign = d->sign;
	QLabel *contrl = d->contrl;
	QFontMetrics fontMetrics(status->font());
	int statusItemHeight = fontMetrics.boundingRect(0, 0, statusItemWidth, 1000,
		Qt::AlignTop | Qt::TextWordWrap, status->toPlainText()).height() + 5;

	if (statusItemHeight < ICON_SIZE) {
		statusItemHeight = ICON_SIZE;
	}

	d->top = top;
	status->move(ICON_SIZE + 2 * MARGIN, top + MARGIN);
	status->resize(statusItemWidth, statusItemHeight);
	statusItemHeight += status->verticalScrollBar()->maximum() - status->verticalScrollBar()->minimum();
	status->resize(statusItemWidth, statusItemHeight);
	icon->move(MARGIN, top + MARGIN);

	QString messageIdText(QString::number(d->messageId));

	if (usernameUnderAvatar) {
		QString tablew = "";
		tablew.setNum(statusItemWidth + 70);
		sign->setText(
			"<table border=\"0\" width=\"" + tablew + "\" cellpadding=\"0\" cellspacing=\"0\"><tr valign=\"top\"><td width=\"50%\"><a href=\"twitter://user/" +
			d->username + "\" style=\"font-weight:bold;text-decoration:none;font-size:small\">" + d->username +
			"</a></td><td width=\"50%\"><p align=\"right\" style=\"margin-right:20px\"><a href=\"http://twitter.com/" + d->username + "/statuses/" +
			messageIdText + "\" style=\"font-size:small;text-decoration:none\">" + TwitterWidget::formatDateTime(d->time) + " </a></p></td>" +
			(verticalAlignControl ? "" : "<td><p align=\"right\"><a href=\"directMessages://" + d->username + ":" + messageIdText +
			"\" style=\"text-decoration:none\"><img src=\":/images/dms.png\"/></a><a href=\"reply://tweet:" + messageIdText + "/q?user=" +
			d->username + "\" style=\"text-decoration:none\"><img src=\":/images/reply.png\"/></a><a href=\"reply://tweet:" + messageIdText +
			"/q?user=" + d->username + "&status=" + QUrl::toPercentEncoding(d->cleanStatus) +
			"\" style=\"text-decoration:none\"><img src=\":/images/rt.png\"/></a></p></td>") + "</tr></table>");
			sign->resize(twitterWidgetWidth + (7 * MARGIN), 16);
			sign->move(MARGIN, top + statusItemHeight + MARGIN);
	} else {
		sign->setText(
			"<a href=\"twitter://user/" + d->username + "\" style=\"font-weight:bold;text-decoration:none;font-size:small\">" + d->username +
			"</a> - <a href=\"http://twitter.com/" + d->username + "/statuses/" + messageIdText + "\" style=\"font-size:small;text-decoration:none\">" +
			TwitterWidget::formatDateTime(d->time) + "</a> " + (verticalAlignControl ? "" : "<a href=\"directMessages://" + d->username + ":" + messageIdText +
			"\" style=\"text-decoration:none\"><img src=\":/images/dms.png\"/></a><a href=\"reply://tweet:" + messageIdText + "/q?user=" + d->username +
			"\" style=\"text-decoration:none\"><img src=\":/images/reply.png\"/></a><a href=\"reply://tweet:" + messageIdText + "/q?user=" + d->username +
			"&status=" + QUrl::toPercentEncoding(d->cleanStatus) +  "\" style=\"text-decoration:none\"><img src=\":/images/rt.png\"/></a>"));
		sign->adjustSize();
		sign->move(twitterWidgetWidth - sign->width() - MARGIN, top + statusItemHeight + MARGIN);
	}
	contrl->setText(
		verticalAlignControl ? "<a href=\"directMessages://" + d->username + ":" + messageIdText +
		"\" style=\"text-decoration:none\"><img src=\":/images/dms.png\"/></a><br><a href=\"reply://tweet:" + messageIdText + "/q?user=" + d->username +
		"\" style=\"text-decoration:none\"><img src=\":/images/reply.png\"/></a><br><a href=\"reply://tweet:" + messageIdText + "/q?user=" + d->username +
		"&status=" + QUrl::toPercentEncoding(d->cleanStatus) + "\" style=\"text-decoration:none\"><img src=\":/images/rt.png\"/></a>" : "");
	contrl->adjustSize();
	contrl->move(twitterWidgetWidth - contrl->width() - MARGIN, top + MARGIN);

	if (d->reply) {
		if (odd) {
			d->color.setRgb(128, 255, 128);
		} else {
			d->color.setRgb(200, 255, 200);
		}
	} else if (odd) {
		d->color.setRgb(230, 230, 230);
	} else {
		d->color.setRgb(255, 255, 255);
	}

	int itemHeight = sign->y() + sign->height();
	if (ICON_SIZE > itemHeight) {
		itemHeight = ICON_SIZE;
	}
	itemHeight += MARGIN - top;
	d->height = itemHeight;

	return itemHeight;
}

void TwitterWidgetItem::paint(QPainter &painter, QPalette palette, int width) {
	painter.fillRect(0, d->top, width, d->height, QBrush(d->color));
	palette.setColor(QPalette::Active, QPalette::Base, d->color);
	palette.setColor(QPalette::Inactive, QPalette::Base, d->color);
	d->status->setPalette(palette);
}
