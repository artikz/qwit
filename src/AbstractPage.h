/*! 
 *  $Id: AbstractPage.h 186 2009-07-11 14:38:30Z artem.iglikov $
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
 *  AbstractPage class declaration
 */

#ifndef AbstractPage_h
#define AbstractPage_h

#include "QwitHeaders.h"

#include "TwitterWidget.h"
#include "Configuration.h"

class AbstractPage: public QWidget {
	Q_OBJECT
	
public:
	QScrollArea *scrollArea;
	TwitterWidget *twitterWidget;
	bool visible;
	
	AbstractPage(QWidget *parent = 0);
	
	virtual void update(Account *account = 0, bool initial = false) = 0;
	virtual void redraw();
	virtual void addItem(const Message &message);
	virtual void updateSize() = 0;
	virtual QString title() = 0;
	virtual void reloadUserpics();
	void clear();
	
public slots:
	virtual void updateItems(const QVector<Message> &items, Account *account);
	virtual void updatePrevious();
	virtual void removePrevious();
	virtual bool updateAutomatically() = 0;
};

#endif
