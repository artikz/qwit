/*!
 *  $Id: AbstractPage.cpp 186 2009-07-11 14:38:30Z artem.iglikov $
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
 *  AbstractPage class implementation
 */

#ifndef AbstractPage_cpp
#define AbstractPage_cpp

#include "QwitHeaders.h"

#include "MainWindow.h"
#include "TwitterWidget.h"

#include "AbstractPage.h"
#include "QwitTools.h"

AbstractPage::AbstractPage(QWidget *parent): QWidget(parent) {
	qDebug() << ("AbstractPage::AbstractPage()");
	twitterWidget = new TwitterWidget(this);
	twitterWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
	connect(twitterWidget, SIGNAL(retweet(const Message &)), MainWindow::getInstance(), SIGNAL(retweet(const Message &)));
	connect(twitterWidget, SIGNAL(reply(const Message &)), MainWindow::getInstance(), SIGNAL(reply(const Message &)));
	connect(twitterWidget, SIGNAL(directMessage(const Message &)), MainWindow::getInstance(), SLOT(directMessage(const Message &)));
	connect(twitterWidget, SIGNAL(favor(const Message &)), MainWindow::getInstance(), SLOT(favor(const Message &)));
	connect(twitterWidget, SIGNAL(unfavor(const Message &)), MainWindow::getInstance(), SLOT(unfavor(const Message &)));
	connect(twitterWidget, SIGNAL(destroy(const Message &)), MainWindow::getInstance(), SLOT(destroy(const Message &)));
}

void AbstractPage::addItem(const Message &message) {
//	qDebug() << ("AbstractPage::addItem()");
	twitterWidget->addItem(message);
}

void AbstractPage::clear() {
	qDebug() << ("AbstractPage::clear()");
	twitterWidget->clear();
}

void AbstractPage::reloadUserpics() {
	qDebug() << ("AbstractPage::reloadUserpics()");
	twitterWidget->reloadUserpics();
}

void AbstractPage::updateItems(const QVector<Message> &items, Account *account) {
	qDebug() << ("AbstractPage::updateItems()");
	int scrollPosition = scrollArea->verticalScrollBar()->value();
	clear();
	for (int i = 0; i < items.size(); ++i) {
		addItem(items[i]);
	}
	twitterWidget->updateItems();
	scrollArea->verticalScrollBar()->setValue(scrollPosition);
}

void AbstractPage::updatePrevious() {
}

void AbstractPage::removePrevious() {
}

void AbstractPage::redraw() {
	twitterWidget->updateItems();
}

#endif
