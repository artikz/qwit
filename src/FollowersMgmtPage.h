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
 *  FollowersMgmtPage class declaration
 *
 */

#ifndef FOLLOWERSMGMTPAGE_H
#define FOLLOWERSMGMTPAGE_H

#include "AbstractUserMgmtPage.h"

class FollowersMgmtPage : public AbstractUserMgmtPage
{
    Q_OBJECT

protected:
    int widgetType();

public:
    FollowersMgmtPage(QWidget *parent);
    void updateSize();
    void update(Account *account = 0);
signals:
    void follow(QString, UserMgmtWidgetItem*);
    void unfollow(QString, UserMgmtWidgetItem*);
    void block(QString, UserMgmtWidgetItem*);
};

#endif // FOLLOWERSMGMTPAGE_H
