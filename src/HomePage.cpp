/*!
 *  $Id: HomePage.cpp 195 2009-07-17 16:53:17Z artem.iglikov $
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
 *  HomePage class implementation
 */

#ifndef HomePage_cpp
#define HomePage_cpp

#include "QwitHeaders.h"

#include "HomePage.h"
#include "QwitTools.h"

HomePage::HomePage(QWidget* parent): AbstractPage(parent) {
	qDebug() << ("HomePage::HomePage()");

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

void HomePage::updateSize() {
	qDebug() << ("HomePage::updateSize()");
	twitterWidget->resize(scrollArea->width() - scrollArea->verticalScrollBar()->width() - 5, 500);
}

QString HomePage::title() {
	return tr("Home");
}

void HomePage::update(Account *account, bool initial) {
	qDebug() << ("HomePage::update()");
	Configuration *config = Configuration::getInstance();
	if (account) {
		account->receiveFriendsMessages(config->messagesPerPage, initial);
	} else {
		config->currentAccount()->receiveFriendsMessages(config->messagesPerPage, initial);
	}
}

void HomePage::updatePrevious() {
	qDebug() << ("HomePage::updatePrevious()");
	twitterWidget->disableMoreButton();
	Configuration *config = Configuration::getInstance();
	config->currentAccount()->receivePreviousFriendsMessages(config->messagesPerPage);
}

void HomePage::removePrevious() {
	qDebug() << ("HomePage::removePrevious()");
	twitterWidget->disableLessButton();
	Configuration *config = Configuration::getInstance();
	config->currentAccount()->removePreviousFriendsMessages(config->messagesPerPage);
	twitterWidget->enableLessButton();
}

bool HomePage::updateAutomatically() {
	Configuration *config = Configuration::getInstance();
	return config->autoUpdateHomeTab;
}

#endif
