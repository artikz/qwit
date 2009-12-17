/*!
 *  $Id: FavoritesPage.cpp 195 2009-07-17 16:53:17Z artem.iglikov $
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
 *  FavoritesPage class implementation
 */

#ifndef FavoritesPage_cpp
#define FavoritesPage_cpp

#include "QwitHeaders.h"

#include "FavoritesPage.h"
#include "QwitTools.h"

FavoritesPage::FavoritesPage(QWidget* parent): AbstractPage(parent) {
	qDebug() << ("FavoritesPage::FavoritesPage()");

	twitterWidget->setObjectName(QString::fromUtf8("favoritesPageTwitterWidget"));
	connect(twitterWidget, SIGNAL(moreButtonClicked()), this, SLOT(updatePrevious()));
	connect(twitterWidget, SIGNAL(lessButtonClicked()), this, SLOT(removePrevious()));

	QGridLayout *gridLayout = new QGridLayout(this);
	gridLayout->setObjectName(QString::fromUtf8("favoritesPageGridLayout"));

	scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(twitterWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	gridLayout->addWidget(scrollArea, 0, 0, 1, 1);
}

void FavoritesPage::updateSize() {
	qDebug() << ("FavoritesPage::updateSize()");
	twitterWidget->resize(scrollArea->width() - scrollArea->verticalScrollBar()->width() - 5, 500);
}

QString FavoritesPage::title() {
	return tr("Favorites");
}

void FavoritesPage::update(Account *account, bool initial) {
	qDebug() << ("FavoritesPage::update()");
	Configuration *config = Configuration::getInstance();
	if (account) {
		account->receiveFavorites(initial);
	} else {
		config->currentAccount()->receiveFavorites(initial);
	}
}

void FavoritesPage::updatePrevious() {
	qDebug() << ("FavoritesPage::updatePrevious()");
	twitterWidget->disableMoreButton();
	Configuration *config = Configuration::getInstance();
	config->currentAccount()->receivePreviousFavorites();
}

void FavoritesPage::removePrevious() {
	qDebug() << ("FavoritesPage::removePrevious()");
	twitterWidget->disableLessButton();
	Configuration *config = Configuration::getInstance();
	config->currentAccount()->removePreviousFavorites();
	twitterWidget->enableLessButton();
}

bool FavoritesPage::updateAutomatically() {
	Configuration *config = Configuration::getInstance();
	return config->autoUpdateFavoritesTab;
}

#endif
