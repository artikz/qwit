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
 *  BlocksMgmtPage class implementation
 *
 */

#ifndef BlocksMgmtPage_cpp
#define BlocksMgmtPage_cpp

#include "BlocksMgmtPage.h"

BlocksMgmtPage::BlocksMgmtPage(QWidget *parent) : AbstractUserMgmtPage(parent)
{
    qDebug() << ("BlocksMgmtPage::BlocksMgmtPage()");
    this->title = tr("Blocks");

    userMgmtWidget->setObjectName(QString::fromUtf8("blocksMgmtWidget"));
    // define

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("blocksMgmtGridLayout"));

    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Light);
    scrollArea->setWidget(userMgmtWidget);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

    connect(userMgmtWidget, SIGNAL(destroyBlock(QString,UserMgmtWidgetItem*)), this, SIGNAL(unblock(QString,UserMgmtWidgetItem*)));
}

void BlocksMgmtPage::updateSize()
{
    qDebug() << ("BlocksMgmtPage::updateSize()");
    userMgmtWidget->resize(scrollArea->width() - scrollArea->verticalScrollBar()->width() -5, 500);
}

void BlocksMgmtPage::update(Account *account)
{
    qDebug() << ("BlocksMgmtPage::update()");
    Configuration *config = Configuration::getInstance();

    if(account)
    {
	account->receiveBlocks();
    }
    else
    {
	config->currentAccount()->receiveBlocks();
    }
}

int BlocksMgmtPage::widgetType()
{
    return 2;
}
#endif
