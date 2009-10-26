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
 *  UserMgmtWidgetItem class declaration
 *
 */

#ifndef UserMgmtWidgetITEM_H
#define UserMgmtWidgetITEM_H

#include <QLabel>
#include <QDateTime>
#include <QTextBrowser>
#include <QPushButton>
#include <QPainter>
#include <QDateTime>

#include "QwitHeaders.h"
#include "Message.h"
//#include "Enumerations.h"

class UserMgmtWidgetItem : public QObject
{
    Q_OBJECT

private:
    QWidget *parent;
    int widgetType;
    QTextBrowser *messageTextBrowser;
    QToolButton *followButton;
    QToolButton *unfollowButton;
    QToolButton *blockButton;
    QToolButton *unblockButton;
    QLabel *userpicLabel;
    QLabel *signLabel;
    int topPosition;
    int height;
    QColor color;
    Message message;

public:
    static const int IconSize = 48;
    static const int Margin = 5;
    UserMgmtWidgetItem(Message message, QWidget *parent, int widgetType);
    ~UserMgmtWidgetItem();
    void loadUserpic();
    int arrangeMessage(int index, int currentHeight);
    int update(int index, int currentHeight);
    void paint(QPainter &painter, QPalette palette, int width);

private slots:
    void on_followToolButton_pressed();
    void on_unfollowToolButton_pressed();
    void on_blockToolButton_pressed();
    void on_unblockToolButton_pressed();
};

#endif // UserMgmtWidgetITEM_H
