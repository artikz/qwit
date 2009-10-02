/*!
 *  $Id: TwitterWidgetItem.cpp 186 2009-07-11 14:38:30Z artem.iglikov $
 *
 *  @file
 *  @author Artem Iglikov <artem.iglikov@gmail.com>
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
 *  TwitterWidgetItem class implementation
 */

#ifndef TwitterWidgetItem_cpp
#define TwitterWidgetItem_cpp

#include <QPainterPath>
#include "QwitHeaders.h"
#include "QwitTools.h"
#include "UserpicsDownloader.h"
#include "Account.h"

#include "TwitterWidgetItem.h"

void TwitterWidgetItem::loadUserpic() {
	QPixmap pixmap(message.userpicFilename);
	QString inReplyToUserImageFileName = UserpicsDownloader::getInstance()->userImageFileName(
											message.account->serviceBaseUrl(), message.inReplyToUsername);
	QPixmap inReplyToUserPic(inReplyToUserImageFileName);
	if (!inReplyToUserPic.isNull()) {
		// if this is a reply, show the replied-to person in a smaller inset thumbnail
		QPixmap comboPic(ICON_SIZE, ICON_SIZE);
		comboPic.fill(Qt::transparent);
		QPainter painter(&comboPic);
		painter.drawPixmap(0, ICON_SIZE * 0.2,
						   pixmap.scaled(ICON_SIZE * 0.8, ICON_SIZE * 0.8, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		painter.drawPixmap(ICON_SIZE * 0.5, 0,
						   inReplyToUserPic.scaled(ICON_SIZE * 0.5, ICON_SIZE * 0.5, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		pixmap = comboPic;
	}
	if (!pixmap.isNull()) {
		userpicLabel->setPixmap(pixmap.scaled(ICON_SIZE, ICON_SIZE));
	}
	userpicLabel->resize(ICON_SIZE, ICON_SIZE);
}

TwitterWidgetItem::TwitterWidgetItem() {
	messageTextBrowser = 0;
	userpicLabel = 0;
	signLabel = 0;
	replyButton = 0;
	favorButton = 0;
	destroyButton = 0;
	retweetButton = 0;
	unfollowButton = 0;
	followButton = 0;
	directMessageButton = 0;
}

TwitterWidgetItem::~TwitterWidgetItem() {
	delete messageTextBrowser;
	delete userpicLabel;
	delete signLabel;
	delete replyButton;
	delete favorButton;
	delete unfollowButton;
	delete followButton;
	delete directMessageButton;
	if (retweetButton) {
		delete retweetButton;
	}
	if (destroyButton) {
		delete destroyButton;
	}
}

#endif
