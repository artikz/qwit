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
 *  BlocksMgmtPage class declaration
 *
 */

#ifndef BLOCKSMGMTPAGE_H
#define BLOCKSMGMTPAGE_H

#include "AbstractUserMgmtPage.h"

class BlocksMgmtPage : public AbstractUserMgmtPage
{
    Q_OBJECT

protected:
    int widgetType();

public:
    BlocksMgmtPage(QWidget *parent);
    void updateSize();
    void update(Account *account = 0);
signals:
    void unblock(QString, UserMgmtWidgetItem*);
};

#endif // BLOCKSMGMTPAGE_H
