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


#include <QScrollBar>
#include <iostream>
#include <QMessageBox>

#include "FriendsMgmtWidget.h"
#include "TwitterWidgetItem.h"

using namespace std;

FriendsMgmtWidget::FriendsMgmtWidget(QScrollArea *scrollArea, const QString &serviceBaseURL) : QWidget()
{
    this->serviceBaseUrl = serviceBaseURL;
    this->scrollArea = scrollArea;
    this->tabIndex = tabIndex;
    nextItemIndex = 0;
}


void FriendsMgmtWidget::addItem(QString username, QString userpic, Categories category, QString statusText, uint messageId, QDateTime time, uint replyStatusId)
{
    FriendsMgmtWidgetItem *item = new FriendsMgmtWidgetItem(this, username, userpic, category, statusText, messageId, time, replyStatusId, this->serviceBaseUrl);

    item->loadIcon();

    if(nextItemIndex > items.size())
    nextItemIndex = items.size();

    items.insert(nextItemIndex++, item);
    cout << "added item" << endl;

    item->show();

    this->updateItems();
}


void FriendsMgmtWidget::updateItems()
{
    int height = 0;

    for (int i = 0; i < items.size(); ++i)
    {
	height += items[i]->update(height, (i & 1));
    }
    resize(width(), height);
}

void FriendsMgmtWidget::clear()
{
    if(nextItemIndex > 0)
    {
	for(int i=nextItemIndex-1; i>=0; i--)
	{
	    delete items[i];
	}
	nextItemIndex = 0;
	this->items.clear();
    }
}

void FriendsMgmtWidget::paintEvent(QPaintEvent *event)
{
    // create local painter object
    QPainter painter(this);

    for(int i=0; i< items.size(); ++i)
    {
	items[i]->paint(painter, palette(), width());
    }
    event->accept();
}

void FriendsMgmtWidget::resizeEvent(QResizeEvent *event)
{
    if(event->oldSize() == event->size())
    {
        event->ignore();
        return;
    }
    updateItems();
    event->accept();
}

void FriendsMgmtWidget::unfollowClicked(const QUrl &url)
{
    emit unfollow(url.path().remove(0, 1));
}

void FriendsMgmtWidget::followClicked(const QUrl &url)
{
    emit follow(url.path().remove(0, 1));
}

void FriendsMgmtWidget::blockClicked(const QUrl &url)
{
    QString screenName = url.path().remove(0, 1);

    QMessageBox msgBox;
    msgBox.setText(tr("Blocking will prevent %1 from following you. And you won't see their tweets in your timeline. Are you sure you want to block?").arg(screenName));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Ok)
    {
	this->removeItem(screenName);
	emit block(screenName);
    }
}

void FriendsMgmtWidget::unblockClicked(const QUrl &url)
{
    emit unblock(url.path().remove(0, 1));
}

void FriendsMgmtWidget::removeItem(QString screenName)
{
    for(int i=0; i<items.size(); i++)
    {
	if(items[i]->getUsername() == screenName)
	{
	    delete items[i];
	    this->items.remove(i);
	    nextItemIndex--;
	    updateItems();
	    return;
	}
    }
}

int FriendsMgmtWidget::getItemCount()
{
    return nextItemIndex;
}
