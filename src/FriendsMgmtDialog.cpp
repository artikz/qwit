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
 *  FriendsMgmtDialog class implementation
 */

#ifndef FriendsMgmtDialog_cpp
#define FriendsMgmtDialog_cpp

#include <QScrollArea>
#include <QScrollBar>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>

#include "FriendsMgmtDialog.h"
#include "Configuration.h"


FriendsMgmtDialog::FriendsMgmtDialog(QWidget *parent) : QDialog(parent)
{
    qDebug() << ("FriendsMgmtDialog::FriendsMgmtDialog()");
    setupUi(this);

    statusBar = new QStatusBar(this);
    topLevelLayout->addWidget(statusBar);

    oldAccountId = -1;
    firstRun = true;

    tabWidget->removeTab(0);
    tabWidget->removeTab(0);
    tabWidget->removeTab(0);
    pages.push_back(friendshipsPage = new FriendshipsMgmtPage(this));
    pages.push_back(followersPage = new FollowersMgmtPage(this));
    pages.push_back(blocksPage = new BlocksMgmtPage(this));

    connect(friendshipsPage, SIGNAL(follow(QString)), this, SLOT(on_add_friend(QString)));
    connect(friendshipsPage, SIGNAL(unfollow(QString,UserMgmtWidgetItem*)), this, SLOT(on_unfollow_friend(QString,UserMgmtWidgetItem*)));
    connect(friendshipsPage, SIGNAL(block(QString,UserMgmtWidgetItem*)), this, SLOT(on_block_friend(QString,UserMgmtWidgetItem*)));
    connect(friendshipsPage, SIGNAL(stateChanged(QString)), this, SLOT(setState(QString)));
    connect(followersPage, SIGNAL(follow(QString,UserMgmtWidgetItem*)), this, SLOT(on_follow_follower(QString,UserMgmtWidgetItem*)));
    connect(followersPage, SIGNAL(unfollow(QString,UserMgmtWidgetItem*)), this, SLOT(on_unfollow_follower(QString,UserMgmtWidgetItem*)));
    connect(followersPage, SIGNAL(block(QString,UserMgmtWidgetItem*)), this, SLOT(on_block_follower(QString,UserMgmtWidgetItem*)));
    connect(followersPage, SIGNAL(stateChanged(QString)), this, SLOT(setState(QString)));
    connect(blocksPage, SIGNAL(unblock(QString,UserMgmtWidgetItem*)), this, SLOT(on_unblock_user(QString,UserMgmtWidgetItem*)));
    connect(blocksPage, SIGNAL(stateChanged(QString)), this, SLOT(setState(QString)));
    this->requestId = 1000;

    for(int i=0; i<pages.size(); i++)
	tabWidget->addTab(pages[i], pages[i]->getTitle());
}


FriendsMgmtDialog::~FriendsMgmtDialog()
{
    delete friendshipsPage;
    delete followersPage;
    delete blocksPage;
    delete statusBar;
}


void FriendsMgmtDialog::resizeEvent(QResizeEvent *event)
{
    qDebug() << ("FriendsMgmtDialog::resizeEvent()");

    for(int i=0; i<pages.size(); i++)
	pages[i]->updateSize();

    event->ignore();
}


void FriendsMgmtDialog::showEvent(QShowEvent *event)
{
    qDebug() << ("FriendsMgmtDialog::showEvent()");

    if(firstRun)
    {
	Configuration *config = Configuration::getInstance();
	oldAccountId = config->currentAccountId;

	if(config->currentAccountId != -1)
	{
	    connect(config->currentAccount(), SIGNAL(friendshipsUpdated(QVector<Message>)), friendshipsPage, SLOT(updateItems(QVector<Message>)));
	    connect(config->currentAccount(), SIGNAL(followersUpdated(QVector<Message>)), followersPage, SLOT(updateItems(QVector<Message>)));
	    connect(config->currentAccount(), SIGNAL(blocksUpdated(QVector<Message>)), blocksPage, SLOT(updateItems(QVector<Message>)));
	    connect(config->currentAccount(), SIGNAL(friendshipAdded(Message,uint)), this, SLOT(addFriend(Message,uint)));
	    connect(config->currentAccount(), SIGNAL(friendshipRemoved(Message,uint)), this, SLOT(removeFriend(Message,uint)));
	    connect(config->currentAccount(), SIGNAL(blockAdded(Message,uint)), this, SLOT(addBlock(Message,uint)));
	    connect(config->currentAccount(), SIGNAL(blockRemoved(Message,uint)), this, SLOT(removeBlock(Message,uint)));
	}
	firstRun = false;
    }
    else
	updateConnects();

    Configuration *config = Configuration::getInstance();
    config->currentAccount()->receiveFriendships();
    config->currentAccount()->receiveFollowers();
    config->currentAccount()->receiveBlocks();

    tabWidget->setCurrentIndex(0);

    for(int i=0; i<pages.size(); i++)
	pages[i]->updateSize();

    event->accept();
}


void FriendsMgmtDialog::updateConnects()
{
    qDebug() << ("FriendsMgmtDialog::updateConnects()");

    Configuration *config = Configuration::getInstance();

    if(config->currentAccountId == oldAccountId)
	return;

    oldAccountId = config->currentAccountId;

    disconnect(config->accounts[oldAccountId], SIGNAL(friendshipsUpdated(QVector<Message>)), 0, 0);
    connect(config->currentAccount(), SIGNAL(friendshipsUpdated(QVector<Message>)), friendshipsPage, SLOT(updateItems(QVector<Message>)));
    disconnect(config->accounts[oldAccountId], SIGNAL(followersUpdated(QVector<Message>)), 0, 0);
    connect(config->currentAccount(), SIGNAL(followersUpdated(QVector<Message>)), followersPage, SLOT(updateItems(QVector<Message>)));
    disconnect(config->accounts[oldAccountId], SIGNAL(blocksUpdated(QVector<Message>)), 0, 0);
    connect(config->currentAccount(), SIGNAL(blocksUpdated(QVector<Message>)), blocksPage, SLOT(updateItems(QVector<Message>)));
    disconnect(config->accounts[oldAccountId], SIGNAL(friendshipAdded(Message,uint)), 0, 0);
    connect(config->currentAccount(), SIGNAL(friendshipAdded(Message,uint)), this, SLOT(addFriend(Message,uint)));
    disconnect(config->accounts[oldAccountId], SIGNAL(friendshipRemoved(Message,uint)), 0, 0);
    connect(config->currentAccount(), SIGNAL(friendshipRemoved(Message,uint)), this, SLOT(removeFriend(Message,uint)));
    disconnect(config->accounts[oldAccountId], SIGNAL(blockAdded(Message,uint)), 0 , 0);
    connect(config->currentAccount(), SIGNAL(blockAdded(Message,uint)), this, SLOT(addBlock(Message,uint)));
    disconnect(config->accounts[oldAccountId], SIGNAL(blockRemoved(Message,uint)), 0, 0);
    connect(config->currentAccount(), SIGNAL(blockRemoved(Message,uint)), this, SLOT(removeBlock(Message,uint)));
}

void FriendsMgmtDialog::on_tabWidget_currentChanged(int index)
{
    qDebug() << ("FriendsMgmtDialog::on_tabWidget_currentChanged()");

    if((index >= 0) && (index < pages.size()) && pages[index])
    {
	pages[index]->updateSize();
    }
}

void FriendsMgmtDialog::on_add_friend(QString screenName)
{
    qDebug() << ("FriendsMgmtDialog::on_add_friend()");

    requestsFromFrienshipsPage.insert(requestId, 0);

    Configuration *config = Configuration::getInstance();
    config->currentAccount()->createFriendship(screenName, requestId++);
}

void FriendsMgmtDialog::on_unfollow_friend(QString screenName, UserMgmtWidgetItem *item)
{
   qDebug() << ("FriendsMgmtDialog::on_unfollow_friend()");

   requestsFromFrienshipsPage.insert(requestId, item);

   Configuration *config = Configuration::getInstance();
   config->currentAccount()->destroyFriendship(screenName, requestId++);
}

void FriendsMgmtDialog::on_block_friend(QString screenName, UserMgmtWidgetItem *item)
{
    qDebug() << ("FriendsMgmtDialog::on_block_friend()");

    QMessageBox msgBox;
    msgBox.setText(tr("Blocking will prevent %1 from following you. And you won't see their tweets in your timeline. Are you sure you want to block?").arg(screenName));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Ok)
    {
	requestsFromFrienshipsPage.insert(requestId, item);

	Configuration *config = Configuration::getInstance();
	config->currentAccount()->createBlock(screenName, requestId++);
    }
}

void FriendsMgmtDialog::on_follow_follower(QString screenName, UserMgmtWidgetItem *item)
{
    qDebug() << ("FriendsMgmtDialog::on_follow_follower()");

    requestsFromFollowersPage.insert(requestId, item);

    Configuration *config = Configuration::getInstance();
    config->currentAccount()->createFriendship(screenName, requestId++);
}

void FriendsMgmtDialog::on_unfollow_follower(QString screenName, UserMgmtWidgetItem *item)
{
   qDebug() << ("FriendsMgmtDialog::on_unfollow_follower()");

   requestsFromFollowersPage.insert(requestId, item);

   Configuration *config = Configuration::getInstance();
   config->currentAccount()->destroyFriendship(screenName, requestId++);
}

void FriendsMgmtDialog::on_block_follower(QString screenName, UserMgmtWidgetItem *item)
{
    qDebug() << ("FriendsMgmtDialog::on_block_follower()");

    QMessageBox msgBox;
    msgBox.setText(tr("Blocking will prevent %1 from following you. And you won't see their tweets in your timeline. Are you sure you want to block?").arg(screenName));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Ok)
    {
	requestsFromFollowersPage.insert(requestId, item);

	Configuration *config = Configuration::getInstance();
	config->currentAccount()->createBlock(screenName, requestId++);
    }
}

void FriendsMgmtDialog::on_unblock_user(QString screenName, UserMgmtWidgetItem *item)
{
    qDebug() << ("FriendsMgmtDialog::on_unblock_user()");

    requestsFromBlocksPage.insert(requestId, item);

    Configuration *config = Configuration::getInstance();
    config->currentAccount()->destroyBlock(screenName, requestId++);
}

void FriendsMgmtDialog::addFriend(Message message, uint requestId)
{
    qDebug() << ("FriendsMgmtDialog::addFriend()");

    Configuration *config = Configuration::getInstance();
    if(requestsFromFrienshipsPage.contains(requestId)) {
	if(requestsFromFrienshipsPage[requestId] == 0) {
	    config->currentAccount()->receiveFriendships();
	} else {
	    config->currentAccount()->receiveFriendships();
	}
    }
    if(requestsFromFollowersPage.contains(requestId)) {
	config->currentAccount()->receiveFriendships();
    }
    this->setState(tr("Following request sent to %1").arg(message.username));
}

void FriendsMgmtDialog::removeFriend(Message message, uint requestId)
{
    qDebug() << ("FriendsMgmtDialog::removeFriend()");

    if(requestsFromFrienshipsPage.contains(requestId)) {
	UserMgmtWidgetItem *item = requestsFromFrienshipsPage[requestId];
	friendshipsPage->removeItem(item);
	friendshipsPage->updateSize();
	this->setState(tr("Unfollowing %1").arg(message.username));
    }
}

void FriendsMgmtDialog::addBlock(Message message, uint requestId)
{
    qDebug() << ("FriendsMgmtDialog::addBlock()");

    Configuration *config = Configuration::getInstance();
    if(requestsFromFrienshipsPage.contains(requestId)) {
	UserMgmtWidgetItem *item = requestsFromFrienshipsPage[requestId];
	friendshipsPage->removeItem(item);
	friendshipsPage->updateSize();
//	config->currentAccount()->receiveBlocks();
    }
    if(requestsFromFollowersPage.contains(requestId)) {
	UserMgmtWidgetItem *item = requestsFromFollowersPage[requestId];
	followersPage->removeItem(item);
	followersPage->updateSize();
//	config->currentAccount()->receiveBlocks();
    }
    this->setState(tr("%1 blocked").arg(message.username));
}

void FriendsMgmtDialog::removeBlock(Message message, uint requestId)
{
    qDebug() << ("FriendsMgmtDialog::removeBlock()");

    if(requestsFromBlocksPage.contains(requestId))
    {
	UserMgmtWidgetItem *item = requestsFromBlocksPage[requestId];
	blocksPage->removeItem(item);
	blocksPage->updateSize();
	this->setState(tr("%1 unblocked").arg(message.username));
    }
}

void FriendsMgmtDialog::setState(QString state)
{
    statusBar->showMessage(state);
}

bool FriendsMgmtDialog::event(QEvent *e)
{
    if(e->type() == QEvent::StatusTip)
    {
	QStatusTipEvent *statusTipEvent = (QStatusTipEvent*) e;
	statusBar->showMessage(statusTipEvent->tip());
	return true;
    }
    return QDialog::event(e);
}
#endif
