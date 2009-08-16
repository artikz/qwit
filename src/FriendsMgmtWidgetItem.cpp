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

#include <QDesktopServices>
#include <QScrollBar>

#include "FriendsMgmtWidgetItem.h"
#include "TwitterWidget.h"

#include <iostream>

using namespace std;

FriendsMgmtWidgetItem::FriendsMgmtWidgetItem(QWidget *parent, const QString username, const QString iconFileName, const int category, const QString statusText, uint messageId, const QDateTime &time, const uint replyStatusId, const QString &serviceBaseUrl)
{
    this->parent = parent;
    this->status = new QTextBrowser(parent);
    this->status->setHtml(TwitterWidgetItem::prepare(statusText,replyStatusId, serviceBaseUrl));
    this->status->setReadOnly(true);
    this->status->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->status->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->status->setFrameShape(QFrame::NoFrame);
    this->status->setOpenExternalLinks(true);

    QFont font = status->document()->defaultFont();
    font.setFamily("Verdana");
    this->status->document()->setDefaultFont(font);

    this->username = username;
    this->category = category;
    this->icon = new QLabel(parent);
    this->messageId = messageId;
    this->time = time;

    this->sign = new QLabel(parent);
    this->sign->setAlignment(Qt::AlignRight);
    this->sign->setOpenExternalLinks(true);

    this->ctrl = new QLabel(parent);
    this->ctrl->setAlignment(Qt::AlignRight);
    this->ctrl->setOpenExternalLinks(true);

    this->iconFileName = iconFileName;

    QDesktopServices::setUrlHandler("unfollow", parent, "unfollowClicked");
    QDesktopServices::setUrlHandler("follow", parent, "followClicked");
    QDesktopServices::setUrlHandler("block", parent, "blockClicked");
    QDesktopServices::setUrlHandler("unblock", parent, "unblockClicked");
}

FriendsMgmtWidgetItem::~FriendsMgmtWidgetItem()
{
    delete status;
    delete icon;
    delete sign;
    delete ctrl;
    cout << "item destroyed" << endl;
}

int FriendsMgmtWidgetItem::getTopPosition()
{
    return this->topPos;
}

int FriendsMgmtWidgetItem::getHeight()
{
    return this->height;
}

QColor FriendsMgmtWidgetItem::getColor()
{
    return this->color;
}

//QTextBrowser* FriendsMgmtWidgetItem::getStatus()
//{
//    return this->status;
//}

//void FriendsMgmtWidgetItem::setIcon(QLabel *icon)
//{
//    this->icon = icon;
//}

void FriendsMgmtWidgetItem::show()
{
    status->show();
    icon->show();
    sign->show();
    ctrl->show();
}

void FriendsMgmtWidgetItem::hide()
{
    status->hide();
    icon->hide();
    sign->hide();
    ctrl->hide();
}

QLabel* FriendsMgmtWidgetItem::getIcon()
{
    return icon;
}

//void FriendsMgmtWidgetItem::setSignText(QString text)
//{
//    this->sign->setText(text);
//}

//QLabel* FriendsMgmtWidgetItem::getSign()
//{
//    return this->sign;
//}

//void FriendsMgmtWidgetItem::setTopPosition(int top)
//{
//    this->topPos = top;
//}

void FriendsMgmtWidgetItem::setHeight(int height)
{
    this->height = height;
}

QString FriendsMgmtWidgetItem::getUsername()
{
    return this->username;
}

void FriendsMgmtWidgetItem::setColor(QColor color)
{
    this->color = color;
}

void FriendsMgmtWidgetItem::loadIcon()
{
        QPixmap pixmap(iconFileName);
        if (!pixmap.isNull()) {
                icon->setPixmap(pixmap.scaled(ICON_SIZE, ICON_SIZE));
        }
        icon->resize(ICON_SIZE, ICON_SIZE);
}

int FriendsMgmtWidgetItem::update(int top, bool odd)
{
    
    // calculate the size of the status element
    int statusItemWidth = parent->width() - (ICON_SIZE + 5 * MARGIN);
    QFontMetrics fontMetrics(status->font());
    int statusItemHeight = fontMetrics.boundingRect(0, 0, statusItemWidth, 1000, Qt::AlignTop | Qt::TextWordWrap, status->toPlainText()).height() + 5;
    
    if(statusItemHeight < ICON_SIZE)
    {
    statusItemHeight = ICON_SIZE;
    }
    
    // positioning and scaling of the status element
    this->topPos = top;
    status->move(ICON_SIZE + 2 * MARGIN, top + MARGIN);
    status->resize(statusItemWidth, statusItemHeight);
    statusItemHeight += status->verticalScrollBar()->maximum() - status->verticalScrollBar()->minimum();
    status->resize(statusItemWidth, statusItemHeight);
    // positioning of the icon
    icon->move(MARGIN, top + MARGIN);
    
    QString messageIdText(QString::number(this->messageId));

    QString tablew = "";
    tablew.setNum(statusItemWidth + 70);
    sign->setText("<table border=\"0\" width=\"" + tablew + "\" cellpadding=\"0\" cellspacing=\"0\">"
			+ "<tr valign=\"top\"><td width=\"50%\">"
			+ "<a href=\"twitter://user/" + this->username + "\" style=\"font-weight:bold;text-decoration:none;font-size:small\">" + this->username + "</a>"
			+ "</td>"
			+ "<td width=\"50%\">"
			+ "<p align=\"right\" style=\"margin-right:20px\">"
			+ "<a href=\"http://twitter.com/" + this->username + "/statuses/"+ messageIdText + "\" style=\"font-size:small;text-decoration:none\">"
			+ TwitterWidget::formatDateTime(this->time) + " </a>"
			+ "</p>"
			+ "</td>"
			+ "</tr></table>");

    sign->resize(parent->width() + (7 * MARGIN), 16);
    sign->move(MARGIN, top + statusItemHeight + MARGIN);

    switch(category)
    {
	case FRIENDS:
	    ctrl->setText("<a href=\"unfollow://twitter.com/" + username + "\" style=\"text-decoration:none\"><img src=\":/images/unfollow.png\"/></a><br>"
	    + "<a href=\"block://twitter.com/" + username + "\" style=\"text-decoration:none\"><img src=\":/images/block.png\"/></a>");
	    break;
	case FOLLOWERS:
	    ctrl->setText("<a href=\"follow://twitter.com/" + username + "\" style=\"text-decoration:none\"><img src=\":/images/follow.png\"/></a><br>"
	    + "<a href=\"block://twitter.com/" + username + "\" style=\"text-decoration:none\"><img src=\":/images/block.png\"/></a>");
	    break;
	case BLOCKED:
	    ctrl->setText("<a href=\"unblock://twitter.com/" + username + "\" style=\"text-decoration:none\"><img src=\":/images/follow.png\"/></a>");
	    break;
	default:
	    break;
    }
    // positioning of the ctrl element
    ctrl->adjustSize();
    ctrl->move(parent->width() - ctrl->width() - MARGIN, top + MARGIN);
    
    // set color of the FriendMgmtWidgetItem
    if(odd)
    {
	this->color.setRgb(230, 230, 230);
    }
    else
    {
	this->color.setRgb(255, 255, 255);
    }
    
    int itemHeight = sign->y() + sign->height();
    if(ICON_SIZE > itemHeight)
    {
    itemHeight = ICON_SIZE;
    }
    itemHeight += MARGIN - top;
    this->setHeight(itemHeight);
    height = itemHeight;

    return itemHeight;
}

void FriendsMgmtWidgetItem::paint(QPainter &painter, QPalette palette, int width)
{
    painter.fillRect(0, topPos, width, height, QBrush(color));
    palette.setColor(QPalette::Active, QPalette::Base, color);
    palette.setColor(QPalette::Inactive, QPalette::Base, color);
    status->setPalette(palette);
}
