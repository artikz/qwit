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
 *  UserMgmtWidget class declaration
 *
 */

#ifndef UserMgmtWidget_H
#define UserMgmtWidget_H

#include <QWidget>
#include <QScrollArea>
#include <QDateTime>
#include <QPainter>
#include <QPaintEvent>

#include "QwitHeaders.h"

#include "UserMgmtWidgetItem.h"
#include "Message.h"
//#include "Enumerations.h"

class UserMgmtWidget : public QWidget
{
    Q_OBJECT

private:
//    QScrollArea *scrollArea;
    QVector<UserMgmtWidgetItem*> items;
//    int nextItemIndex;
//    QString serviceBaseUrl;

public:
    UserMgmtWidget(QWidget *parent);
    void clear();
    void addItem(Message message, int widgetType);
    void removeItem(UserMgmtWidgetItem *item);
//    void removeItem(QString screenName);
    void updateItems();
//    const UserMgmtWidgetItem getItem(int index);
//    int getItemsCount();
//    void setMessagesPerPage(int value);
//    void setUsernameUnderAvatar(bool value);
//    void setVerticalAlignControl(bool value);
//    void saveScrollPosition();
//    void restoreScrollPosition();
//    int getItemCount();
//    void resizeWidget();
    void follow(QString screenName, UserMgmtWidgetItem *item);
    void unfollow(QString screenName, UserMgmtWidgetItem *item);
    void block(QString screenName, UserMgmtWidgetItem *item);
    void unblock(QString screenName, UserMgmtWidgetItem *item);

//    static QString formatDateTime(const QDateTime &time);

protected:
    // overwrite QWidget::paintEvent() method
    void paintEvent(QPaintEvent *event);
    // overwrite QWidget::resizeEvent() method
    void resizeEvent(QResizeEvent *event);

signals:
    void createFriendship(QString, UserMgmtWidgetItem*);
    void destroyFriendship(QString, UserMgmtWidgetItem*);
    void createBlock(QString, UserMgmtWidgetItem*);
    void destroyBlock(QString, UserMgmtWidgetItem*);
};

#endif // UserMgmtWidget_H
