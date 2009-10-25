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
 *  UserMgmtWidget class implementation
 *
 */

#ifndef UserMgmtWidget_cpp
#define UserMgmtWidget_cpp

#include <QScrollBar>

#include "UserMgmtWidget.h"

//UserMgmtWidget::UserMgmtWidget(QWidget *parent, QScrollArea *scrollArea, int widgetType) : QWidget(parent)
//{
//    qDebug() << ("UserMgmtWidget::UserMgmtWidget()");
//    this->scrollArea = scrollArea;
//    this->widgetType = widgetType;
//    nextItemIndex = 0;
//}

UserMgmtWidget::UserMgmtWidget(QWidget *parent) : QWidget(parent)
{
    qDebug() << ("UserMgmtWidget::UserMgmtWidget()");
//    nextItemIndex = 0;
}

void UserMgmtWidget::addItem(Message message, int widgetType)
{
    UserMgmtWidgetItem *item = new UserMgmtWidgetItem(message, this, widgetType);

    items.push_back(item);
}

void UserMgmtWidget::updateItems()
{
    int height = 0;

    for (int i = 0; i < items.size(); ++i)
    {
	height += items[i]->update(i, height);
    }
    resize(width(), height);
}

void UserMgmtWidget::clear()
{
    qDebug() << ("UserMgmtWidget::clear()");

    if(items.size() > 0)
    {
	for(int i=items.size()-1; i>=0; i--)
	{
	    delete items[i];
	}
	this->items.clear();
    }
}

void UserMgmtWidget::paintEvent(QPaintEvent *event)
{
    // create local painter object
    QPainter painter(this);

    for(int i=0; i< items.size(); ++i)
    {
	items[i]->paint(painter, palette(), width());
    }
    event->accept();
}

void UserMgmtWidget::resizeEvent(QResizeEvent *event)
{
    if(event->oldSize() == event->size())
    {
	event->ignore();
	return;
    }
    updateItems();
    event->accept();
}

void UserMgmtWidget::follow(QString screenName, UserMgmtWidgetItem *item)
{
    emit createFriendship(screenName, item);
}

void UserMgmtWidget::unfollow(QString screenName, UserMgmtWidgetItem *item)
{
    emit destroyFriendship(screenName, item);
}

void UserMgmtWidget::block(QString screenName, UserMgmtWidgetItem *item)
{
    emit createBlock(screenName, item);
}

void UserMgmtWidget::unblock(QString screenName, UserMgmtWidgetItem *item)
{
    emit destroyBlock(screenName, item);
}

//void UserMgmtWidget::resizeWidget()
//{
//    this->resize(scrollArea->width() - scrollArea->verticalScrollBar()->width() - 5, 500);
//}

void UserMgmtWidget::removeItem(UserMgmtWidgetItem *item)
{
    qDebug() << ("UserMgmtWidget::removeItem()");

    if(items.contains(item))
    {
	int index = items.indexOf(item);
	delete items[index];
	items.remove(index);
    }
}
#endif
