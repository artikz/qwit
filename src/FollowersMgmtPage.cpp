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
 *  FollowersMgmtPage class implementation
 *
 */

#ifndef FollowersMgmtPage_cpp
#define FollowersMgmtPage_cpp

#include "FollowersMgmtPage.h"

FollowersMgmtPage::FollowersMgmtPage(QWidget *parent) : AbstractUserMgmtPage(parent)
{
    qDebug() << ("FollowersMgmtPage::FollowersMgmtPage()");
    this->title = tr("Followers");

    userMgmtWidget->setObjectName(QString::fromUtf8("followersMgmtWidget"));
    // define connects

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("followersMgmtGridLayout"));

    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Light);
    scrollArea->setWidget(userMgmtWidget);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

    connect(userMgmtWidget, SIGNAL(createFriendship(QString,UserMgmtWidgetItem*)), this, SIGNAL(follow(QString,UserMgmtWidgetItem*)));
    connect(userMgmtWidget, SIGNAL(destroyFriendship(QString,UserMgmtWidgetItem*)), this, SIGNAL(unfollow(QString,UserMgmtWidgetItem*)));
    connect(userMgmtWidget, SIGNAL(createBlock(QString,UserMgmtWidgetItem*)), this, SIGNAL(block(QString,UserMgmtWidgetItem*)));
}

void FollowersMgmtPage::updateSize()
{
    qDebug() << ("FollowersMgmtPage::updateSize()");
    userMgmtWidget->resize(scrollArea->width() - scrollArea->verticalScrollBar()->width() -5, 500);
}


void FollowersMgmtPage::update(Account *account)
{
    qDebug("FollowersMgmtPage::update()");
    Configuration *config = Configuration::getInstance();

    if(account)
    {
	account->receiveFollowers();
    }
    else
    {
	config->currentAccount()->receiveFollowers();
    }
}

int FollowersMgmtPage::widgetType()
{
    return 1;
}
#endif
