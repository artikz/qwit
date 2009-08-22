/*!
 *  $Id: SearchPage.cpp 195 2009-07-17 16:53:17Z artem.iglikov $
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
 *  SearchPage class implementation
 */

#ifndef SearchPage_cpp
#define SearchPage_cpp

#include "QwitHeaders.h"

#include "SearchPage.h"
#include "QwitTools.h"

SearchPage::SearchPage(QWidget* parent): AbstractPage(parent) {
	qDebug() << ("SearchPage::SearchPage()");

	twitterWidget->setObjectName(QString::fromUtf8("searchPageTwitterWidget"));
	connect(twitterWidget, SIGNAL(moreButtonClicked()), this, SLOT(updatePrevious()));
	connect(twitterWidget, SIGNAL(lessButtonClicked()), this, SLOT(removePrevious()));

	QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
	vBoxLayout->setObjectName(QString::fromUtf8("searchPageVBoxLayout"));

	QHBoxLayout *hBoxLayout = new QHBoxLayout();
	hBoxLayout->addWidget(new QLabel(tr("Find:")));
	lineEdit = new QLineEdit(this);
	hBoxLayout->addWidget(lineEdit);
	
	scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	
	vBoxLayout->addLayout(hBoxLayout);
	vBoxLayout->addWidget(scrollArea);
	connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(update()));
}

void SearchPage::updateSize() {
	qDebug() << ("SearchPage::updateSize()");
	twitterWidget->resize(scrollArea->width() - scrollArea->verticalScrollBar()->width() - 5, 500);
}

QString SearchPage::title() {
	return tr("Search");
}

void SearchPage::update(Account *account) {
	qDebug() << ("SearchPage::update()");
	Configuration *config = Configuration::getInstance();
	config->searchQuery = lineEdit->text();
	if (account) {
		account->receiveSearchMessages(config->messagesPerPage, lineEdit->text());
	} else {
		config->currentAccount()->receiveSearchMessages(config->messagesPerPage, lineEdit->text());
	}
}

void SearchPage::updatePrevious() {
	qDebug() << ("SearchPage::updatePrevious()");
	twitterWidget->disableMoreButton();
	Configuration *config = Configuration::getInstance();
	config->currentAccount()->receivePreviousSearchMessages(config->messagesPerPage, lineEdit->text());
}

void SearchPage::removePrevious() {
	qDebug() << ("SearchPage::removePrevious()");
	twitterWidget->disableLessButton();
	Configuration *config = Configuration::getInstance();
	config->currentAccount()->removePreviousSearchMessages(config->messagesPerPage);
	twitterWidget->enableLessButton();
}

bool SearchPage::updateAutomatically() {
	Configuration *config = Configuration::getInstance();
	return config->autoUpdateSearchTab;
}

#endif
