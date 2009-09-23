/*!
 *  $Id: TwitterWidget.cpp 201 2009-07-21 16:04:47Z artem.iglikov $
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
 *  TwitterWidget class implementation
 */

#ifndef TwitterWidget_cpp
#define TwitterWidget_cpp

#include "QwitHeaders.h"

#include "TwitterWidget.h"
#include "Translator.h"
#include "QwitException.h"
#include "QwitTools.h"
#include "Configuration.h"

int TwitterWidget::instances = 0;

TwitterWidget::TwitterWidget(QWidget *parent, bool paintMentions): QWidget(parent) {
	qDebug() << ("TwitterWidget::TwitterWidget()");
	instanceId = instances++;
	moreToolButton = 0;
	lessToolButton = 0;
	this->paintMentions = paintMentions;
	addMoreButton();
	connect(&retweetButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(retweetButtonClicked(int)));
	connect(&replyButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(replyButtonClicked(int)));
	connect(&directMessageButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(directMessageButtonClicked(int)));
	connect(&favorButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(favorButtonClicked(int)));
	connect(&destroyButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(destroyButtonClicked(int)));
	connect(Translator::getInstance(), SIGNAL(textTranslated(const QString&, QObject*)), this, SLOT(insertTranslation(const QString&, QObject*)));
}

void TwitterWidget::clear() {
	qDebug() << ("TwitterWidget::clear()");

	for (int i = 0; i < items.size(); ++i) {
		delete items[i];
	}
	items.clear();
	updateItems();
}

void TwitterWidget::addItem(const Message &message) {
	TwitterWidgetItem *item = new TwitterWidgetItem();

	item->message = message;

	item->messageTextBrowser = new TwitterWidgetItemMessage(this, message);
	item->messageTextBrowser->setHtml(QwitTools::prepareMessage(message.text, message.account));
	item->messageTextBrowser->setReadOnly(true);
	item->messageTextBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	item->messageTextBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	item->messageTextBrowser->setFrameShape(QFrame::NoFrame);
	item->messageTextBrowser->setOpenExternalLinks(true);

	item->userpicLabel = new QLabel(this);
	item->loadUserpic();
	item->signLabel = new QLabel(this);
	item->signLabel->setAlignment(Qt::AlignRight);
	item->signLabel->setOpenExternalLinks(true);
	item->signLabel->setStyleSheet("a{text-decoration:none;}");

	if (!message.directMessage) {
		item->favorButton = new QToolButton(this);
		item->favorButton->setIcon(QwitTools::getToolButtonIcon(":/images/favor.png", message.favorited));
		item->favorButton->setText("");
		item->favorButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		item->favorButton->setAutoRaise(true);
		item->favorButton->show();
	}

	item->replyButton = new QToolButton(this);
	item->replyButton->setIcon(QwitTools::getToolButtonIcon(":/images/reply.png"));
	item->replyButton->setText("");
	item->replyButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	item->replyButton->setAutoRaise(true);
	item->replyButton->show();

	if (!message.directMessage) {
		if (message.username == message.account->username) {
			item->destroyButton = new QToolButton(this);
			item->destroyButton->setIcon(QwitTools::getToolButtonIcon(":/images/destroy.png"));
			item->destroyButton->setText("");
			item->destroyButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
			item->destroyButton->setAutoRaise(true);
			item->destroyButton->show();
		} else {
			item->retweetButton = new QToolButton(this);
			item->retweetButton->setIcon(QwitTools::getToolButtonIcon(":/images/retweet.png"));
			item->retweetButton->setText("");
			item->retweetButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
			item->retweetButton->setAutoRaise(true);
			item->retweetButton->show();
		}
	} else {
		item->destroyButton = new QToolButton(this);
		item->destroyButton->setIcon(QwitTools::getToolButtonIcon(":/images/destroy.png"));
		item->destroyButton->setText("");
		item->destroyButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		item->destroyButton->setAutoRaise(true);
		item->destroyButton->show();
		item->retweetButton = new QToolButton(this);
		item->retweetButton->setIcon(QwitTools::getToolButtonIcon(":/images/retweet.png"));
		item->retweetButton->setText("");
		item->retweetButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		item->retweetButton->setAutoRaise(true);
		item->retweetButton->show();
	}
	item->directMessageButton = new QToolButton(this);
	item->directMessageButton->setIcon(QwitTools::getToolButtonIcon(":/images/directMessage.png"));
	item->directMessageButton->setText("");
	item->directMessageButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	item->directMessageButton->setAutoRaise(true);
	item->directMessageButton->resize(18, 18);
	item->directMessageButton->show();

	items.push_back(item);

	item->messageTextBrowser->show();
	item->userpicLabel->show();
	item->signLabel->show();

//	updateItems();
}

int TwitterWidget::arrangeMessage(TwitterWidgetItem *item, int index, int height) {
	QFontMetrics fontMetrics(item->messageTextBrowser->font());
	int messageItemWidth = width() - (ICON_SIZE + 4 * MARGIN + item->favorButton->width());
	int messageItemHeight = fontMetrics.boundingRect(0, 0, messageItemWidth, 1000, Qt::AlignTop | Qt::TextWordWrap, item->messageTextBrowser->toPlainText()).height() + MARGIN;
	if (messageItemHeight < ICON_SIZE) {
		messageItemHeight = ICON_SIZE;
	}
	item->messageTextBrowser->move(ICON_SIZE + 2 * MARGIN, height + MARGIN);
	item->messageTextBrowser->resize(messageItemWidth, messageItemHeight);
	messageItemHeight += item->messageTextBrowser->verticalScrollBar()->maximum() - item->messageTextBrowser->verticalScrollBar()->minimum();
	item->messageTextBrowser->resize(messageItemWidth, messageItemHeight);
	item->userpicLabel->move(MARGIN, height + MARGIN);

	item->favorButton->move(width() - MARGIN - item->favorButton->width(), height);
	item->favorButton->show();
	item->replyButton->move(width() - MARGIN - item->replyButton->width(), height + item->favorButton->height());
	item->replyButton->show();
	if (item->retweetButton) {
		item->retweetButton->move(width() - MARGIN - item->retweetButton->width(), height + item->favorButton->height() + item->replyButton->height());
		item->retweetButton->show();
	}
	if (item->destroyButton) {
		item->destroyButton->move(width() - MARGIN - item->destroyButton->width(), height + item->favorButton->height() + item->replyButton->height());
		item->destroyButton->show();
	}

	item->directMessageButton->move(MARGIN + ICON_SIZE / 2 - item->directMessageButton->width() / 2, height + ICON_SIZE + 2 * MARGIN);
	item->directMessageButton->show();

	QString messageUrl = item->message.account->singleMessageUrl().replace("%username", item->message.username).replace("%messageid", QString::number(item->message.id));
	QString sign = "<style>a{text-decoration:none;}</style><div style=\"font-size:small\"><a href=\"" + item->message.account->serviceBaseUrl() + "/" + item->message.username + "\" style=\"font-weight:bold\">" + item->message.username + "</a> - <a href=\"" + messageUrl + "\">" + QwitTools::formatDateTime(item->message.time) + "</a>";
	if (item->message.source != "") {
		sign += " - from " + item->message.source;
	}
	if (item->message.inReplyToMessageId) {
		QString inReplyToMessageUrl = item->message.account->singleMessageUrl().replace("%username", item->message.inReplyToUsername).replace("%messageid", QString::number(item->message.inReplyToMessageId));
		sign += " - <a href=\"" + inReplyToMessageUrl + "\">in reply to " + item->message.inReplyToUsername + "</a>";
	}
	sign += "</div>";
	item->signLabel->setText(sign);
	item->signLabel->adjustSize();
	item->signLabel->setToolTip(item->message.time.toString(Qt::DefaultLocaleLongDate));
	int signY = 0;
	if (item->retweetButton) {
		signY = item->retweetButton->pos().y() + item->retweetButton->height() - item->signLabel->height();
	} else {
		signY = item->destroyButton->pos().y() + item->destroyButton->height() - item->signLabel->height();
	}
	signY = max(signY, item->messageTextBrowser->pos().y() + item->messageTextBrowser->height() + MARGIN);
	int signX = width() - item->signLabel->width() - MARGIN - item->favorButton->width();
	if (signX < item->directMessageButton->pos().x() + item->directMessageButton->width() + MARGIN) {
		signY = max(signY, item->directMessageButton->pos().y() + item->directMessageButton->height());
	}
	item->signLabel->move(signX, signY);

	Configuration *config = Configuration::getInstance();
	if (paintMentions && item->message.mention) {
		if (index & 1) {
			item->color = config->mentionsEvenColor;
		} else {
			item->color = config->mentionsOddColor;
		}
	} else {
		if (index & 1) {
			item->color = config->commonMessagesEvenColor;
		} else {
			item->color = config->commonMessagesOddColor;
		}
	}

	int itemHeight = messageItemHeight + item->signLabel->height() + MARGIN;
	itemHeight = max(item->directMessageButton->y() + item->directMessageButton->height(), item->signLabel->y() + item->signLabel->height()) + MARGIN - height;
	item->top = height;
	item->height = itemHeight;
	return itemHeight;
}

int TwitterWidget::arrangeDirectMessage(TwitterWidgetItem *item, int index, int height) {
	QFontMetrics fontMetrics(item->messageTextBrowser->font());
	int messageItemWidth = width() - (ICON_SIZE + 4 * MARGIN + item->replyButton->width());
	int messageItemHeight = fontMetrics.boundingRect(0, 0, messageItemWidth, 1000, Qt::AlignTop | Qt::TextWordWrap, item->messageTextBrowser->toPlainText()).height() + MARGIN;
	if (messageItemHeight < ICON_SIZE) {
		messageItemHeight = ICON_SIZE;
	}
	item->messageTextBrowser->move(ICON_SIZE + 2 * MARGIN, height + MARGIN);
	item->messageTextBrowser->resize(messageItemWidth, messageItemHeight);
	messageItemHeight += item->messageTextBrowser->verticalScrollBar()->maximum() - item->messageTextBrowser->verticalScrollBar()->minimum();
	item->messageTextBrowser->resize(messageItemWidth, messageItemHeight);
	item->userpicLabel->move(MARGIN, height + MARGIN);

	item->replyButton->move(width() - MARGIN - item->replyButton->width(), height);
	item->replyButton->show();
	item->retweetButton->move(width() - MARGIN - item->retweetButton->width(), height + item->replyButton->height());
	item->retweetButton->show();
	item->destroyButton->move(width() - MARGIN - item->destroyButton->width(), height + item->replyButton->height() + item->retweetButton->height());
	item->destroyButton->show();

	item->directMessageButton->move(MARGIN + ICON_SIZE / 2 - item->directMessageButton->width() / 2, height + ICON_SIZE + 2 * MARGIN);
	item->directMessageButton->show();

	QString messageUrl = item->message.account->singleMessageUrl().replace("%username", item->message.username).replace("%messageid", QString::number(item->message.id));
	QString sign = "<style>a{text-decoration:none;}</style><div style=\"font-size:small\"><a href=\"" + item->message.account->serviceBaseUrl() + "/" + item->message.username + "\" style=\"font-weight:bold\">" + item->message.username + "</a> - <a href=\"" + messageUrl + "\">" + QwitTools::formatDateTime(item->message.time) + "</a>";
	if (item->message.source != "") {
		sign += " - from " + item->message.source;
	}
	if (item->message.inReplyToMessageId) {
		QString inReplyToMessageUrl = item->message.account->singleMessageUrl().replace("%username", item->message.inReplyToUsername).replace("%messageid", QString::number(item->message.inReplyToMessageId));
		sign += " - <a href=\"" + inReplyToMessageUrl + "\">in reply to " + item->message.inReplyToUsername + "</a>";
	}
	sign += "</div>";
	item->signLabel->setText(sign);
	item->signLabel->adjustSize();
	item->signLabel->setToolTip(item->message.time.toString(Qt::DefaultLocaleLongDate));
	int signY = 0;
	signY = item->retweetButton->pos().y() + item->retweetButton->height() - item->signLabel->height();
	signY = item->destroyButton->pos().y() + item->destroyButton->height() - item->signLabel->height();
	signY = max(signY, item->messageTextBrowser->pos().y() + item->messageTextBrowser->height() + MARGIN);
	int signX = width() - item->signLabel->width() - MARGIN - item->replyButton->width();
	if (signX < item->directMessageButton->pos().x() + item->directMessageButton->width() + MARGIN) {
		signY = max(signY, item->directMessageButton->pos().y() + item->directMessageButton->height());
	}
	item->signLabel->move(signX, signY);

	Configuration *config = Configuration::getInstance();
	if (paintMentions && item->message.mention) {
		if (index & 1) {
			item->color = config->mentionsEvenColor;
		} else {
			item->color = config->mentionsOddColor;
		}
	} else {
		if (index & 1) {
			item->color = config->commonMessagesEvenColor;
		} else {
			item->color = config->commonMessagesOddColor;
		}
	}

	int itemHeight = messageItemHeight + item->signLabel->height() + MARGIN;
	itemHeight = max(item->directMessageButton->y() + item->directMessageButton->height(), item->signLabel->y() + item->signLabel->height()) + MARGIN - height;
	item->top = height;
	item->height = itemHeight;
	return itemHeight;
}

void TwitterWidget::updateItems() {
//	qDebug() << ("TwitterWidget::updateItems()");

	retweetButtonGroup.buttons().clear();
	replyButtonGroup.buttons().clear();
	directMessageButtonGroup.buttons().clear();
	favorButtonGroup.buttons().clear();
	destroyButtonGroup.buttons().clear();
	int height = 0;
	for (int i = 0; i < items.size(); ++i) {
		TwitterWidgetItem *item = items[i];
		if (!item->message.directMessage) {
			replyButtonGroup.addButton(item->replyButton, i);
			directMessageButtonGroup.addButton(item->directMessageButton, i);
			favorButtonGroup.addButton(item->favorButton, i);
			if (item->retweetButton) {
				retweetButtonGroup.addButton(item->retweetButton, i);
			}
			if (item->destroyButton) {
				destroyButtonGroup.addButton(item->destroyButton, i);
			}
			height += arrangeMessage(item, i, height);
		} else {
			replyButtonGroup.addButton(item->replyButton, i);
			retweetButtonGroup.addButton(item->retweetButton, i);
			destroyButtonGroup.addButton(item->destroyButton, i);
			directMessageButtonGroup.addButton(item->directMessageButton, i);
			height += arrangeDirectMessage(item, i, height);
		}
	}
	Configuration *config = Configuration::getInstance();
	if (moreToolButton) {
		moreToolButton->move((width() - moreToolButton->width()) / 2, height + MARGIN);
		moreToolButton->show();
	}
	if (moreToolButton && (config->messagesPerPage < items.size())) {
		addLessButton();
	} else {
		removeLessButton();
	}
	if (lessToolButton) {
		moreToolButton->move((width() - (moreToolButton->width() + lessToolButton->width() + MARGIN)) / 2, height + MARGIN);
		moreToolButton->show();
		lessToolButton->move((width() - (moreToolButton->width() + lessToolButton->width() + MARGIN)) / 2 + moreToolButton->width() + MARGIN, height + MARGIN);
		lessToolButton->show();
	}
	if (moreToolButton) {
		height += moreToolButton->height() + 2 * MARGIN;
	}
	resize(width(), height);
//	qDebug() << ("TwitterWidget::updateItems() finished");
}

void TwitterWidget::resizeEvent(QResizeEvent *event) {
	qDebug() << ("TwitterWidget::resizeEvent()");

	if (event->oldSize() == event->size()) {
		event->ignore();
		return;
	}
	updateItems();
	event->accept();
}

void TwitterWidget::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	for (int i = 0; i < items.size(); ++i) {
		TwitterWidgetItem *item = items[i];
		painter.fillRect(0, item->top, width(), item->height, QBrush(item->color));
		QPalette p = palette();
		p.setColor(QPalette::Active, QPalette::Base, item->color);
		p.setColor(QPalette::Inactive, QPalette::Base, item->color);
		item->messageTextBrowser->setPalette(p);
	}
	event->accept();
}

void TwitterWidget::reloadUserpics() {
//	qDebug() << ("TwitterWidget::reloadUserpics()");

	for (int i = 0; i < items.size(); ++i) {
		items[i]->loadUserpic();
	}
}

void TwitterWidget::removeMoreButton() {
	if (moreToolButton) {
		qDebug() << ("TwitterWidget::removeMoreButton()");
		delete moreToolButton;
		moreToolButton = 0;
	}
}

void TwitterWidget::removeLessButton() {
	if (lessToolButton) {
		qDebug() << ("TwitterWidget::removeLessButton()");
		delete lessToolButton;
		lessToolButton = 0;
	}
}

void TwitterWidget::addMoreButton() {
	if (!moreToolButton) {
		qDebug() << ("TwitterWidget::addMoreButton()");
		moreToolButton = new QToolButton(this);
		moreToolButton->setText(tr("more"));
		moreToolButton->setCheckable(true);
		connect(moreToolButton, SIGNAL(clicked()), this, SIGNAL(moreButtonClicked()));
	}
}

void TwitterWidget::addLessButton() {
	if (!lessToolButton) {
		qDebug() << ("TwitterWidget::addLessButton()");
		lessToolButton = new QToolButton(this);
		lessToolButton->setText(tr("less"));
		lessToolButton->setCheckable(true);
		connect(lessToolButton, SIGNAL(clicked()), this, SIGNAL(lessButtonClicked()));
	}
}

void TwitterWidget::retweetButtonClicked(int id) {
	emit retweet(items[id]->message);
}

void TwitterWidget::destroyButtonClicked(int id) {
	if (QMessageBox::question(this, tr("Delete message"), tr("Are you sure to delete this message?"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Ok) {
		emit destroy(items[id]->message);
	}
}

void TwitterWidget::replyButtonClicked(int id) {
	emit reply(items[id]->message);
}

void TwitterWidget::directMessageButtonClicked(int id) {
	emit directMessage(items[id]->message);
}

void TwitterWidget::favorButtonClicked(int id) {
	if (items[id]->message.favorited) {
		emit unfavor(items[id]->message);
	} else {
		emit favor(items[id]->message);
	}
}

void TwitterWidget::enableMoreButton() {
	if (moreToolButton) {
		moreToolButton->setEnabled(true);
		moreToolButton->setChecked(false);
	}
}

void TwitterWidget::enableLessButton() {
	if (lessToolButton) {
		lessToolButton->setEnabled(true);
		moreToolButton->setChecked(false);
	}
}

void TwitterWidget::disableMoreButton() {
	if (moreToolButton) {
		moreToolButton->setEnabled(false);
	}
}

void TwitterWidget::disableLessButton() {
	if (lessToolButton) {
		lessToolButton->setEnabled(false);
	}
}

void TwitterWidget::insertTranslation(const QString &translation, QObject *item) {
	if (item && item->inherits("TwitterWidgetItemMessage")) {
		for (int i = 0; i < items.size(); ++i) {
			if (items[i]->messageTextBrowser == item) {
				TwitterWidgetItemMessage *receiver = (TwitterWidgetItemMessage*)item;
				receiver->insertTranslation(translation);
				break;
			}
		}
	}
}

#endif
