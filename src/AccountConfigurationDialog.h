/*! 
 *  $Id: AccountConfigurationDialog.h 186 2009-07-11 14:38:30Z artem.iglikov $
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
 *  AccountConfigurationDialog class declaration
 */

#ifndef AccountConfigurationDialog_h
#define AccountConfigurationDialog_h

#include "ui_AccountConfigurationDialog.h"

#include "OAuthDialog.h"
#include "QwitHeaders.h"

class AccountConfigurationDialog: public QDialog, public Ui::AccountConfigurationDialog {
	Q_OBJECT
	
    OAuthDialog *oauthDialog;
public:
	enum {
		ActionAdd = 0,
		ActionEdit = 1
	};

	int action;
	int accountId;
	int accountType;
    QString serviceOAuthRequestTokenUrl;
    QString serviceOAuthAuthorizeUrl;
    QString serviceOAuthAccessTokenUrl;
    QString token;
    QString tokenSecret;
    QOAuth::Interface *qoauth;

	AccountConfigurationDialog(QWidget *parent);

public slots:
    void oauthAuthorize();
    void useHttpsCheckBoxChanged(int state);
    void plainGroupBoxToggled(bool state);
    void oauthGroupBoxToggled(bool state);
    void commitAuthorization();
    void stopAuthorization();
};

#endif
