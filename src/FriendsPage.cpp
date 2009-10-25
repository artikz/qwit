/*!
 *  $Id$
 *
 *  @file
 *  @author Thomas Salm <tosate@googlemail.com>
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
 *  FriendsPage class implementation
 */

#ifndef FriendsPage_cpp
#define FriendsPage_cpp

#include "FriendsPage.h"

FriendsPage::FriendsPage(QWidget *parent) : AbstractPage(parent)
{
    qDebug() << ("FriendsPage::FriendsPage()");

    friendsWidget->setObjectName(QString::fromUtf8("friendsPageFriendsMgmtWidget"));

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("friendsPageGridLayout"));

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Light);
    scrollArea->setWidget(friendsWidget);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    gridLayout->addWidget(scrollArea, 0, 0, 1, 1);
}

void FriendsPage::updateSize()
{
}

QString FriendsPage::title()
{
    return QString("Friends");
}

void FriendsPage::update(Account *account)
{
}

bool FriendsPage::updateAutomatically()
{
    return false;
}
#endif
