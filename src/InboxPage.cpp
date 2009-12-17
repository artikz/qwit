/*!
 *  $Id: InboxPage.cpp 195 2009-07-17 16:53:17Z artem.iglikov $
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
 *  InboxPage class implementation
 */

#ifndef InboxPage_cpp
#define InboxPage_cpp

#include "QwitHeaders.h"

#include "InboxPage.h"
#include "QwitTools.h"

InboxPage::InboxPage(QWidget* parent): AbstractPage(parent) {
	qDebug() << ("InboxPage::InboxPage()");

	twitterWidget->setObjectName(QString::fromUtf8("inboxPageTwitterWidget"));
	connect(twitterWidget, SIGNAL(moreButtonClicked()), this, SLOT(updatePrevious()));
	connect(twitterWidget, SIGNAL(lessButtonClicked()), this, SLOT(removePrevious()));

	QGridLayout *gridLayout = new QGridLayout(this);
	gridLayout->setObjectName(QString::fromUtf8("inboxPageGridLayout"));

	scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	gridLayout->addWidget(scrollArea, 0, 0, 1, 1);
}

void InboxPage::updateSize() {
	qDebug() << ("InboxPage::updateSize()");
	twitterWidget->resize(scrollArea->width() - scrollArea->verticalScrollBar()->width() - 5, 500);
}

QString InboxPage::title() {
	return tr("Inbox");
}

void InboxPage::update(Account *account, bool initial) {
	qDebug() << ("InboxPage::update()");
	Configuration *config = Configuration::getInstance();
	if (account) {
		account->receiveInboxMessages(config->messagesPerPage, initial);
	} else {
		config->currentAccount()->receiveInboxMessages(config->messagesPerPage, initial);
	}
}

void InboxPage::updatePrevious() {
	qDebug() << ("InboxPage::updatePrevious()");
	twitterWidget->disableMoreButton();
	Configuration *config = Configuration::getInstance();
	config->currentAccount()->receivePreviousInboxMessages(config->messagesPerPage);
}

void InboxPage::removePrevious() {
	qDebug() << ("InboxPage::removePrevious()");
	twitterWidget->disableLessButton();
	Configuration *config = Configuration::getInstance();
	config->currentAccount()->removePreviousInboxMessages(config->messagesPerPage);
	twitterWidget->enableLessButton();
}

bool InboxPage::updateAutomatically() {
	Configuration *config = Configuration::getInstance();
	return config->autoUpdateInboxTab;
}

#endif
