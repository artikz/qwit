/*! 
 *  $Id: TwitterWidgetItem.h 186 2009-07-11 14:38:30Z artem.iglikov $
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
 *  TwitterWidgetItem class declaration
 */

#ifndef TwitterWidgetItem_h
#define TwitterWidgetItem_h

#include "QwitHeaders.h"

#include "Message.h"
#include "TwitterWidgetItemMessage.h"

const int ICON_SIZE = 48;

class TwitterWidgetItem {
public:
	TwitterWidgetItemMessage *messageTextBrowser;
	QLabel *userpicLabel;
	QLabel *signLabel;
	QToolButton *replyButton;
	QToolButton *favorButton;
	QToolButton *retweetButton;
        QToolButton *followButton;
        QToolButton *unfollowButton;
        QToolButton *directMessageButton;
	QToolButton *destroyButton;
	
	int top;
	int height;
	Message message;
	QColor color;

	void loadUserpic();
	TwitterWidgetItem();
	~TwitterWidgetItem();
};

#endif
