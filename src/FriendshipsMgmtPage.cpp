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
 *  FriendshipsMgmtPage class implementation
 *
 */

#ifndef FriendshipsMgmtPage_cpp
#define FriendshipsMgmtPage_cpp

#include "FriendshipsMgmtPage.h"
#include "QwitTools.h"

FriendshipsMgmtPage::FriendshipsMgmtPage(QWidget *parent) : AbstractUserMgmtPage(parent)
{
    qDebug() << ("FriendshipsMgmtPage::FriendshipsMgmtPage()");
    this->title = tr("Friends");

    userMgmtWidget->setObjectName(QString::fromUtf8("friendshipsMgmtWidget"));
    // define connects

    vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setObjectName(QString::fromUtf8("friendshipsMgmtVerticalBoxLayout"));
    hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->setObjectName(QString::fromUtf8("friendshipsMgmtHorizontalBoxLayout"));
    vBoxLayout->addLayout(hBoxLayout);
    newFriendsLabel = new QLabel(tr("new friend:"), this);
    hBoxLayout->addWidget(newFriendsLabel);
    newFriendLineEdit = new QLineEdit(this);
    hBoxLayout->addWidget(newFriendLineEdit);
    addFriendPushButton = new QPushButton(tr("add"), this);
    addFriendPushButton->setEnabled(false);
    addFriendPushButton->setAutoDefault(true);
    addFriendPushButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    hBoxLayout->addWidget(addFriendPushButton);

    connect(newFriendLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_newFriendLineEdit_textEdited(QString)));
    connect(addFriendPushButton, SIGNAL(pressed()), this, SLOT(on_addFriendPushButton_pressed()));

    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Light);
    scrollArea->setWidget(userMgmtWidget);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    vBoxLayout->addWidget(scrollArea);

    connect(userMgmtWidget, SIGNAL(destroyFriendship(QString,UserMgmtWidgetItem*)), this, SIGNAL(unfollow(QString,UserMgmtWidgetItem*)));
    connect(userMgmtWidget, SIGNAL(createBlock(QString,UserMgmtWidgetItem*)), this, SIGNAL(block(QString,UserMgmtWidgetItem*)));
}

void FriendshipsMgmtPage::updateSize()
{
    qDebug() << ("FriendshipsMgmtPage::updateSize()");
    userMgmtWidget->resize(scrollArea->width() - scrollArea->verticalScrollBar()->width() -5, 500);
}


void FriendshipsMgmtPage::update(Account *accout)
{
    qDebug() << ("FriendshipsMgmtPage::update()");
    Configuration *config = Configuration::getInstance();

    if(accout)
    {
	accout->receiveFriendships();
    }
    else
    {
	config->currentAccount()->receiveFriendships();
    }
}

void FriendshipsMgmtPage::on_newFriendLineEdit_textEdited(QString )
{
    if(newFriendLineEdit->text().length() > 0)
    {
	addFriendPushButton->setEnabled(true);
    }
    else
    {
	addFriendPushButton->setEnabled(false);
    }
}

void FriendshipsMgmtPage::on_addFriendPushButton_pressed()
{
    QString screenName = newFriendLineEdit->text();
    newFriendLineEdit->setText("");

    // FIXEM check screenName synthax
    emit follow(screenName);
}

int FriendshipsMgmtPage::widgetType()
{
    return 0;
}
#endif
