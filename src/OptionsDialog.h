/*! 
 *  $Id: OptionsDialog.h 201 2009-07-21 16:04:47Z artem.iglikov $
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
 *  OptionsDialog class declaration
 */

#ifndef OptionsDialog_h
#define OptionsDialog_h

#include "ui_OptionsDialog.h"

#include "QwitHeaders.h"

#include "AccountConfigurationDialog.h"
#include "Account.h"

class OptionsDialog: public QDialog, public Ui::OptionsDialog {
	Q_OBJECT

public:
	AccountConfigurationDialog *accountConfigurationDialog;
	
	OptionsDialog(QWidget *parent);
	
public slots:

	void changeOptionsGroup(QListWidgetItem *item);
	void addAccount();
	void deleteAccount();
	void editAccount();
	void commitAccount();
	void chooseCommonMessagesEvenColor();
	void chooseCommonMessagesOddColor();
	void chooseMentionsEvenColor();
	void chooseMentionsOddColor();
};

#endif
