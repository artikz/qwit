/*!
 *  $Id: RepliesPage.cpp 195 2009-07-17 16:53:17Z artem.iglikov $
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
 *  RepliesPage class implementation
 */

#ifndef RepliesPage_cpp
#define RepliesPage_cpp

#include "QwitHeaders.h"

#include "RepliesPage.h"
#include "QwitTools.h"

RepliesPage::RepliesPage(QWidget* parent): AbstractPage(parent) {
	qDebug() << ("RepliesPage::RepliesPage()");

	twitterWidget->setObjectName(QString::fromUtf8("homePageTwitterWidget"));
	connect(twitterWidget, SIGNAL(moreButtonClicked()), this, SLOT(updatePrevious()));
	connect(twitterWidget, SIGNAL(lessButtonClicked()), this, SLOT(removePrevious()));

	QGridLayout *gridLayout = new QGridLayout(this);
	gridLayout->setObjectName(QString::fromUtf8("homePageGridLayout"));

	scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	gridLayout->addWidget(scrollArea, 0, 0, 1, 1);
}

void RepliesPage::updateSize() {
	qDebug() << ("RepliesPage::updateSize()");

	twitterWidget->resize(scrollArea->width() - scrollArea->verticalScrollBar()->width() - 5, 500);
}

QString RepliesPage::title() {
	return tr("Replies");
}

void RepliesPage::update(Account *account) {
	qDebug() << ("RepliesPage::update()");
	Configuration* config = Configuration::getInstance();
	if (account) {
		account->receiveReplies(20);
	} else {
		config->currentAccount()->receiveReplies(20);
	}
}

void RepliesPage::updatePrevious() {
	qDebug() << ("RepliesPage::updatePrevious()");
	twitterWidget->disableMoreButton();
	Configuration *config = Configuration::getInstance();
	config->currentAccount()->receivePreviousReplies(config->messagesPerPage);
}

void RepliesPage::removePrevious() {
	qDebug() << ("RepliesPage::removePrevious()");
	twitterWidget->disableLessButton();
	Configuration *config = Configuration::getInstance();
	config->currentAccount()->removePreviousReplies(config->messagesPerPage);
	twitterWidget->enableLessButton();
}

bool RepliesPage::updateAutomatically() {
	Configuration *config = Configuration::getInstance();
	return config->autoUpdateRepliesTab;
}

#endif
