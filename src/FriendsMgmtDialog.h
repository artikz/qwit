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
 *  FriendsMgmtDialog class declaration
 *
 */

#ifndef FriendsMgmtDialog_h
#define FriendsMgmtDialog_h

#include "ui_FriendsMgmtDialog.h"

#include "QwitHeaders.h"

#include <QtXml/QDomDocument>
#include "UserpicsDownloader.h"
#include "FriendshipsMgmtPage.h"
#include "FollowersMgmtPage.h"
#include "BlocksMgmtPage.h"
#include "UserMgmtWidgetItem.h"

class FriendsMgmtDialog : public QDialog, public Ui::FriendsMgmtDialog
{
        Q_OBJECT

private:
    QVector<AbstractUserMgmtPage*> pages;
    FriendshipsMgmtPage *friendshipsPage;
    FollowersMgmtPage *followersPage;
    BlocksMgmtPage *blocksPage;
    QMap<uint, UserMgmtWidgetItem*> requestsFromFrienshipsPage;
    QMap<uint, UserMgmtWidgetItem*> requestsFromFollowersPage;
    QMap<uint, UserMgmtWidgetItem*> requestsFromBlocksPage;
    uint requestId;
    QStatusBar *statusBar;

    void updateConnects();
    int oldAccountId;
    bool firstRun;

public:
    FriendsMgmtDialog(QWidget *parent);
    ~FriendsMgmtDialog();

protected:
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
    bool event(QEvent *e);

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_add_friend(QString screenName);
    void on_unfollow_friend(QString screenName, UserMgmtWidgetItem *item);
    void on_block_friend(QString screenName, UserMgmtWidgetItem *item);
    void on_follow_follower(QString screenName, UserMgmtWidgetItem *item);
    void on_unfollow_follower(QString screenName, UserMgmtWidgetItem *item);
    void on_block_follower(QString screenName, UserMgmtWidgetItem *item);
    void on_unblock_user(QString screenName, UserMgmtWidgetItem *item);
    void addFriend(Message message, uint requestId);
    void removeFriend(Message Message, uint requestId);
    void addBlock(Message message, uint requestId);
    void removeBlock(Message message, uint requestId);
    void setState(QString state);
};

#endif
