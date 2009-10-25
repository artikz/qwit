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
 *  AbstractUserMgmtPage class implementation
 *
 */

#ifndef AbstractUserMgmtPage_cpp
#define AbstractUserMgmtPage_cpp

#include "AbstractUserMgmtPage.h"

AbstractUserMgmtPage::AbstractUserMgmtPage(QWidget *parent) : QWidget(parent)
{
    qDebug() << ("AbstractUserMgmtPage::AbstractUserMgmtPage()");
    userMgmtWidget = new UserMgmtWidget(this);
    userMgmtWidget->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
}

void AbstractUserMgmtPage::addItem(Message message)
{
    qDebug() << ("AbstractUserMgmtPage::addItem()");
    userMgmtWidget->addItem(message, widgetType());
}

void AbstractUserMgmtPage::clear()
{
    qDebug() << ("AbstractUserMgmtPage::clear()");
    userMgmtWidget->clear();
}

void AbstractUserMgmtPage::reloadUserpics()
{
    qDebug() << ("AbstractUserMgmtPage::reloadUserpics()");
//    userMgmtWidget->reloadUserpics();
}

void AbstractUserMgmtPage::updateItems(const QVector<Message> &items)
{
    qDebug() << ("AbstractUserMgmtPage::updateItems()");
    clear();
    for(int i=0; i<items.size(); i++)
    {
	addItem(items[i]);
    }

    userMgmtWidget->updateItems();

    emit stateChanged(this->title + " updated");
}

void AbstractUserMgmtPage::redraw()
{
    userMgmtWidget->updateItems();
}

void AbstractUserMgmtPage::removeItem(UserMgmtWidgetItem *item)
{
    qDebug() <<("AbstractUserMgmtPage::removeItem()");

    userMgmtWidget->removeItem(item);
    userMgmtWidget->updateItems();
}

QString AbstractUserMgmtPage::getTitle()
{
    return this->title;
}
#endif
