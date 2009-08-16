/*  This file is part of Qwit.

    Copyright (C) 2008, 2009 Artem Iglikov

    Qwit is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Qwit is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Qwit.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef FriendsMgmtDialog_cpp
#define FriendsMgmtDialog_cpp

#include <QScrollArea>
#include <QScrollBar>
#include <iostream>
#include <QDir>
#include <QDateTime>
#include <iostream>

#include "FriendsMgmtDialog.h"
#include "MainWindow.h"

using namespace std;

FriendsMgmtDialog::FriendsMgmtDialog(QWidget *parent, Twitter *twitter, UserpicsDownloader *userpicsDownloader) : QDialog(parent)
{
        setupUi(this);
        this->twitter = twitter;
        this->userpicsDownloader = userpicsDownloader;

        // set up friends management tab
        QScrollArea *scrollArea = new QScrollArea(friendsTab);

	FriendsMgmtWidget *friendsMgmtWidget = new FriendsMgmtWidget(scrollArea, twitter->getServiceBaseURL());
        friendsMgmtWidget->setObjectName(QString::fromUtf8("friendsMgmtWidget"));
        friendsMgmtWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);

        scrollArea->setBackgroundRole(QPalette::Light);
        scrollArea->setWidget(friendsMgmtWidget);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        vBoxLayout2->addWidget(scrollArea);

        connect(friendsMgmtWidget, SIGNAL(unfollow(QString)), this, SLOT(unfollow(QString)));
	connect(friendsMgmtWidget, SIGNAL(block(const QString)), this, SLOT(block(const QString)));
	friendsMgmtTabs[FRIENDS] = FriendsMgmtTab(scrollArea, friendsMgmtWidget);

        // set up followers management tab
        QGridLayout *gridLayout = new QGridLayout(followersTab);
        gridLayout->setObjectName(QString::fromUtf8("followersGridLayout"));

        scrollArea = new QScrollArea(followersTab);

	friendsMgmtWidget = new FriendsMgmtWidget(scrollArea, twitter->getServiceBaseURL());
        friendsMgmtWidget->setObjectName(QString::fromUtf8("followersMgmtWidget"));
        friendsMgmtWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);

        scrollArea->setBackgroundRole(QPalette::Light);
        scrollArea->setWidget(friendsMgmtWidget);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

	connect(friendsMgmtWidget, SIGNAL(follow(QString)), this, SLOT(follow(QString)));
	connect(friendsMgmtWidget, SIGNAL(block(const QString)), this, SLOT(block(const QString)));
	friendsMgmtTabs[FOLLOWERS] = FriendsMgmtTab(scrollArea, friendsMgmtWidget);

	// set up blocked management tab
	gridLayout = new QGridLayout(blockedTab);
	gridLayout->setObjectName(QString::fromUtf8("blockedGridLayout"));

	scrollArea = new QScrollArea(blockedTab);

	friendsMgmtWidget = new FriendsMgmtWidget(scrollArea, twitter->getServiceBaseURL());
	friendsMgmtWidget->setObjectName(QString::fromUtf8("blockedMgmtWidget"));
	friendsMgmtWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);

	scrollArea->setBackgroundRole(QPalette::Light);
	scrollArea->setWidget(friendsMgmtWidget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	gridLayout->addWidget(scrollArea, 0, 0, 1, 1);
	connect(friendsMgmtWidget, SIGNAL(unblock(QString)), this, SLOT(unblock(QString)));
	friendsMgmtTabs[BLOCKED] = FriendsMgmtTab(scrollArea, friendsMgmtWidget);

        // bring friends tab to the front
        tabWidget->setCurrentIndex(0);

	connect(twitter, SIGNAL(friendshipsUpdated(const QByteArray&, int)), this, SLOT(friendshipsUpdated(const QByteArray&, int)));
	connect(twitter, SIGNAL(friendsMgmtEvent(QByteArray, int)), this, SLOT(friendsMgmtEvent(QByteArray, int)));
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}


void FriendsMgmtDialog::resizeEvent(QResizeEvent *event)
{
    for(int i=0; i<MGMT_TABS; ++i)
        friendsMgmtTabs[i].getFriendsMgmtWidget()->resize(friendsMgmtTabs[i].getScrollArea()->width() - friendsMgmtTabs[i].getScrollArea()->verticalScrollBar()->width() -5, 500);

    event->accept();
}

void FriendsMgmtDialog::showEvent(QShowEvent *event)
{
    MainWindow *mainWindow = MainWindow::getInstance();
    // update the displayed tab
    twitter->getFriendships(mainWindow->username, mainWindow->password, tabWidget->currentIndex());

    event->accept();
}

void FriendsMgmtDialog::unfollow(const QString screenName)
{
    MainWindow *mainWindow = MainWindow::getInstance();

    twitter->destroyFriendship(screenName, mainWindow->username, mainWindow->password);

    cout << "Unfollowing: " << screenName.toStdString() << endl;
    this->stateLabel->setText("Unfollowing " + screenName);
}

void FriendsMgmtDialog::follow(const QString screenName)
{
    followImpl(screenName);
}

void FriendsMgmtDialog::block(const QString screenName)
{
    MainWindow *mainWindow = MainWindow::getInstance();

    twitter->createBlock(screenName, mainWindow->username, mainWindow->password);

    cerr << "Blocking: " << screenName.toStdString() << endl;

    this->stateLabel->setText(tr("Blocking : %1").arg(screenName));
}

void FriendsMgmtDialog::unblock(const QString screenName)
{
    MainWindow *mainWindow = MainWindow::getInstance();

    twitter->destroyBlock(screenName, mainWindow->username, mainWindow->password);

    cerr << "Unblocking: " << screenName.toStdString() << endl;

    this->stateLabel->setText(tr("Unblocking : %1").arg(screenName));
}

void FriendsMgmtDialog::friendshipsUpdated(const QByteArray &friendshipsBuffer, int type)
{
    QDomDocument *document = new QDomDocument();

    document->setContent(friendshipsBuffer);

    QDomElement *root = new QDomElement(document->documentElement());

    if(root->tagName() == "users")
    {
	QDomNode *node = new QDomNode(root->firstChild());
	Categories category;

	switch(type)
	{
	    case 7:
		category = FRIENDS;
		break;
	    case 8:
		category = FOLLOWERS;
		break;
	    case 9:
		category = BLOCKED;
		break;
	}

	friendsMgmtTabs[category].getFriendsMgmtWidget()->clear();

	while(!node->isNull())
        {
	    if(node->toElement().tagName() != "user")
		return;

	    processUserXmlStructure(new QDomNode(node->firstChild()), category, NOTHING);
	    node = new QDomNode(node->nextSibling());
        }

	int itemCount = friendsMgmtTabs[category].getFriendsMgmtWidget()->getItemCount();

	switch(category)
	{
	    case FRIENDS:
		this->stateLabel->setText(tr("%n friend(s)", "", itemCount));
		break;
	    case FOLLOWERS:
		this->stateLabel->setText(tr("%n follower(s)", "", itemCount));
		break;
	    case BLOCKED:
		this->stateLabel->setText(tr("%n blocked", "", itemCount));
		break;
	    default:
		break;
	}
        this->saveState();
    }
}

void FriendsMgmtDialog::saveState()
{
    for(int i=0; i<MGMT_TABS; i++)
    {
        friendsMgmtTabs[i].getFriendsMgmtWidget()->resize(friendsMgmtTabs[i].getScrollArea()->width() - friendsMgmtTabs[i].getScrollArea()->verticalScrollBar()->width() -5, 500);
    }
}

void FriendsMgmtDialog::tabChanged(int index)
{
    if(index < MGMT_TABS)
    {
	int height = friendsMgmtTabs[index].getScrollArea()->width() - friendsMgmtTabs[index].getScrollArea()->verticalScrollBar()->width() - 5;
	if(friendsMgmtTabs[index].getFriendsMgmtWidget()->height() != height)
	{
	    friendsMgmtTabs[index].getFriendsMgmtWidget()->resize(height, 500);
	}
	MainWindow *mainWindow = MainWindow::getInstance();
	twitter->getFriendships(mainWindow->username, mainWindow->password, index);
    }
}
#endif


void FriendsMgmtDialog::friendsMgmtEvent(const QByteArray &friendsMgmtBuffer, int type)
{
    QDomDocument *document = new QDomDocument();

    document->setContent(friendsMgmtBuffer);

    QDomElement *root = new QDomElement(document->documentElement());

    Actions action;

    switch(type)
    {
	case 10:
	    action = FOLLOW;
	    break;
	case 11:
	    action = UNFOLLOW;
	    break;
	case 12:
	    action = BLOCK;
	    break;
	case 13:
	    action = UNBLOCK;
	    break;
    }

    if(root->tagName() == "user")
    {
//	QDomNode *node = new QDomNode(root->firstChild());
//
//	while(!node->isNull())
//	{
//	    if(node->toElement().tagName() != "user")
//		return;

	    processUserXmlStructure(new QDomNode(root->firstChild()), NONE, action);
//	    node = new QDomNode(node->nextSibling());
//	}
	this->saveState();
    }
    else if(root->tagName() == "hash")
    {
	QDomNode *node = new QDomNode(root->firstChild());

	while(!node->isNull())
	{
	    if(node->toElement().tagName() == "request"){}
	    else if(node->toElement().tagName() == "error")
	    {
		cout << node->toElement().text().toStdString() << endl;
	    }
	    node = new QDomNode(node->nextSibling());
	}
    }
}

void FriendsMgmtDialog::processUserXmlStructure(QDomNode *currentNode, Categories category, Actions action)
{
    QString screenName;
    bool following;
    QString statusText;
    uint statusId;
    QString timeStr;
    QString image;
    uint replyStatusId = 0;

    QDomNode *node = currentNode;

    while(!node->isNull())
    {
	if(node->toElement().tagName() == "id"){}
	else if(node->toElement().tagName() == "name"){}
	else if(node->toElement().tagName() == "screen_name")
	{
	    screenName = node->toElement().text();
	}
	else if(node->toElement().tagName() == "location"){}
//      else if(node->toElement().tagName() == "description"){}
	else if(node->toElement().tagName() == "profile_image_url")
	{
	    image = node->toElement().text();
	}
//	else if(node->toElement().tagName() == "url"){}
//	else if(node->toElement().tagName() == "protected"){}
//	else if(node->toElement().tagName() == "followers_count"){}
//	else if(node->toElement().tagName() == "profile_background_color"){}
//	else if(node->toElement().tagName() == "profile_text_color"){}
//	else if(node->toElement().tagName() == "profile_link_color"){}
//	else if(node->toElement().tagName() == "profile_sidebar_fill_color"){}
//	else if(node->toElement().tagName() == "profile_sidebar_border_color"){}
//	else if(node->toElement().tagName() == "friends_count"){}
//	else if(node->toElement().tagName() == "created_at"){}
//	else if(node->toElement().tagName() == "favourites_count"){}
//	else if(node->toElement().tagName() == "utc_offset"){}
//	else if(node->toElement().tagName() == "time_zone"){}
//	else if(node->toElement().tagName() == "profile_background_image_url"){}
//	else if(node->toElement().tagName() == "profile_background_tile"){}
//	else if(node->toElement().tagName() == "statuses_count"){}
//	else if(node->toElement().tagName() == "notifications"){}
//	else if(node->toElement().tagName() == "verified"){}
	else if(node->toElement().tagName() == "following")
	{
	    if(node->toElement().text() == "true")
		following = true;
	    else
		following = false;
	}
	else if(node->toElement().tagName() == "status")
	{
	    QDomNode *node2 = new QDomNode(node->firstChild());
	    while(!node2->isNull())
	    {
		if(node2->toElement().tagName() == "created_at")
		{
		    timeStr = node2->toElement().text();
		}
		else if(node2->toElement().tagName() == "id")
		{
		    statusId = node2->toElement().text().toUInt();
		}
		else if(node2->toElement().tagName() == "text")
		{
		    statusText = node2->toElement().text();
		}
//              else if(node2->toElement().tagName() == "source"){}
//              else if(node2->toElement().tagName() == "truncated"){}
		else if(node2->toElement().tagName() == "in_reply_to_status_id")
		{
		    replyStatusId = node2->toElement().text().toUInt();
		}
//		else if(node2->toElement().tagName() == "in_reply_to_user_id"){}
//		else if(node2->toElement().tagName() == "favorited"){}
//		else if(node2->toElement().tagName() == "in_reply_to_screen_name"){}
		node2 = new QDomNode(node2->nextSibling());
	    }
	}
	node = new QDomNode(node->nextSibling());
    }

    // process user entry

    // start with the image file
    QByteArray hash = QCryptographicHash::hash(image.toAscii(), QCryptographicHash::Md5);
    QString imageFileName = "";
    for (int i = 0; i < hash.size(); ++i){
	unsigned char c = hash[i];
	c >>= 4;
	if (c < 10) {
	    c += '0';
	} else {
	    c += 'A' - 10;
	}
	imageFileName += (char)c;
	c = hash[i];
	c &= 15;
	if (c < 10) {
	    c += '0';
	} else {
	    c += 'A' - 10;
	}
	imageFileName += (char)c;
    }
    //imageFileName += "." + QFileInfo(QUrl(image).path()).suffix();
    QDir dir(QDir::homePath());
    dir.mkdir(".qwit");
    imageFileName = dir.absolutePath() + "/.qwit/" + imageFileName;
    userpicsDownloader->download(image, imageFileName);

    MainWindow *mainWindow = MainWindow::getInstance();
    QDateTime time = mainWindow->dateFromString(timeStr);
    time = QDateTime(time.date(), time.time(), Qt::UTC);

    if(action == NOTHING)
    {
	friendsMgmtTabs[category].getFriendsMgmtWidget()->addItem(screenName, imageFileName, category, statusText.simplified(), statusId, time.toLocalTime(), replyStatusId);
    }
    else if(category == NONE)
    {
	switch(action)
	{
	    case FOLLOW:
		/*
		 * Following a user starts with sending a friendship request. If the user accepts this request, the friendship is created.
		 * As soon as the user accepts the request, he appears in the FRIENDS list.
		 */
		break;
	    case UNFOLLOW:
		friendsMgmtTabs[FRIENDS].getFriendsMgmtWidget()->removeItem(screenName);
		break;
	    case BLOCK:
		friendsMgmtTabs[BLOCKED].getFriendsMgmtWidget()->addItem(screenName, imageFileName, BLOCKED, statusText.simplified(), statusId, time.toLocalTime(), replyStatusId);
		break;
	    case UNBLOCK:
		friendsMgmtTabs[BLOCKED].getFriendsMgmtWidget()->removeItem(screenName);
		break;
	    default:
		break;
	}
    }

    return;
}

void FriendsMgmtDialog::on_addFriendPushButton_pressed()
{
    QString screenName = newFriendLineEdit->text();

    followImpl(screenName);
}

void FriendsMgmtDialog::on_closePushButton_pressed()
{
    hide();
}

void FriendsMgmtDialog::on_newFriendLineEdit_textEdited(QString )
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

void FriendsMgmtDialog::followImpl(QString screenName)
{
    for(int i=0; i<screenName.length(); i++)
    {
	if(!TwitterWidgetItem::isUsernameChar(screenName[i]))
	{
	    QString character = QString(screenName[i]);
	    cout << "Screenname contains illegal character: " << character.toStdString() << endl;

	    stateLabel->setText(tr("Screenname contains illegal character: %1").arg(character));

	    return;
	}
    }

    MainWindow *mainWindow = MainWindow::getInstance();

    twitter->createFriendship(screenName, mainWindow->username, mainWindow->password);

    newFriendLineEdit->setText("");

    cout << "Following: " << screenName.toStdString() << endl;
    this->stateLabel->setText("Following Request sent to " + screenName);

    this->tabWidget->setCurrentIndex(FRIENDS);
}
