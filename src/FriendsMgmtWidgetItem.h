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

#ifndef FRIENDSMGMTWIDGETITEM_H
#define FRIENDSMGMTWIDGETITEM_H

#include <QLabel>
#include <QDateTime>
#include <QTextBrowser>
#include <QPushButton>
#include <QPainter>
#include <QDateTime>

#include "Enumerations.h"

class FriendsMgmtWidgetItem
{
private:
    // object that displays all the information about a friend
    QWidget *parent;
    QTextBrowser *status;
    QLabel *icon;
    QLabel *sign;
    QLabel *ctrl;
    QString iconFileName;
    int category;
    int topPos;
    int height;
    QColor color;
    QString username;
    uint messageId;
    uint replyStatusId;
    QDateTime time;

public:
    FriendsMgmtWidgetItem(QWidget *parent, const QString username, const QString iconFileName, const int category, const QString statusText, uint messageId, const QDateTime &time, const uint replyStatusId, const QString &serviceBaseUrl);
    ~FriendsMgmtWidgetItem();
    void loadIcon();
//    void setTopPosition(int top);
    int getTopPosition();
    void setHeight(int height);
    int getHeight();
    QColor getColor();
//    QTextBrowser* getStatus();
//    void setIcon(QLabel *icon);
//    void setSignText(QString text);
//    QLabel* getSign();
    void show();
    QLabel* getIcon();
    QString getUsername();
    void setColor(QColor color);
    int update(int top, bool odd);
    void paint(QPainter &painter, QPalette palette, int width);
    void hide();

};

#endif // FRIENDSMGMTWIDGETITEM_H
