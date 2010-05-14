/*!
 *  $Id: AccountConfigurationDialog.cpp 186 2009-07-11 14:38:30Z artem.iglikov $
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
 *  AccountConfigurationDialog class implementation
 */

#ifndef AccountConfigurationDialog_cpp
#define AccountConfigurationDialog_cpp

#include "QwitHeaders.h"

#include "AccountConfigurationDialog.h"

AccountConfigurationDialog::AccountConfigurationDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);

    oauthDialog = new OAuthDialog(this);

    connect(oauthAuthorizePushButton, SIGNAL(pressed()), this, SLOT(oauthAuthorize()));
    connect(useHttpsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(useHttpsCheckBoxChanged(int)));
    connect(plainGroupBox, SIGNAL(toggled(bool)), this, SLOT(plainGroupBoxToggled(bool)));
    connect(oauthGroupBox, SIGNAL(toggled(bool)), this, SLOT(oauthGroupBoxToggled(bool)));

    connect(oauthDialog, SIGNAL(accepted()), this, SLOT(commitAuthorization()));
    connect(oauthDialog, SIGNAL(rejected()), this, SLOT(stopAuthorization()));

}

void AccountConfigurationDialog::useHttpsCheckBoxChanged(int state) {
	if (state == Qt::Checked) {
		QString serviceBaseUrl = serviceBaseUrlLineEdit->text();
		if (serviceBaseUrl.startsWith("http://")) {
			serviceBaseUrl = "https://" + serviceBaseUrl.mid(7);
		}
		serviceBaseUrlLineEdit->setText(serviceBaseUrl);
		QString serviceApiUrl = serviceApiUrlLineEdit->text();
		if (serviceApiUrl.startsWith("http://")) {
			serviceApiUrl = "https://" + serviceApiUrl.mid(7);
		}
		serviceApiUrlLineEdit->setText(serviceApiUrl);
	} else {
		QString serviceBaseUrl = serviceBaseUrlLineEdit->text();
		if (serviceBaseUrl.startsWith("https://")) {
			serviceBaseUrl = "http://" + serviceBaseUrl.mid(8);
		}
		serviceBaseUrlLineEdit->setText(serviceBaseUrl);
		QString serviceApiUrl = serviceApiUrlLineEdit->text();
		if (serviceApiUrl.startsWith("https://")) {
			serviceApiUrl = "http://" + serviceApiUrl.mid(8);
		}
		serviceApiUrlLineEdit->setText(serviceApiUrl);
	}
}

void AccountConfigurationDialog::plainGroupBoxToggled(bool state) {
    oauthGroupBox->setChecked(!state);
}

void AccountConfigurationDialog::oauthGroupBoxToggled(bool state) {
    plainGroupBox->setChecked(!state);
}

void AccountConfigurationDialog::oauthAuthorize() {
    setEnabled(false);
    QOAuth::ParamMap reply = qoauth->requestToken(serviceOAuthRequestTokenUrl, QOAuth::GET, QOAuth::HMAC_SHA1 );
    if (qoauth->error() == QOAuth::NoError) {
        token = reply.value(QOAuth::tokenParameterName());
        tokenSecret = reply.value(QOAuth::tokenSecretParameterName());
        QString url = serviceOAuthAuthorizeUrl;
        url.replace("%token", token);
        QString text = oauthDialog->oauthLabel->text();
        text.replace("%url", url);
        oauthDialog->oauthLabel->setText(text);
        setEnabled(true);
        oauthDialog->showNormal();
    } else {
        QMessageBox::critical(this, "Error!", "Error while trying OAuth!");
        setEnabled(true);
    }
}

void AccountConfigurationDialog::commitAuthorization() {
    setEnabled(false);
    QString pin = oauthDialog->pinLineEdit->text();

    QOAuth::ParamMap otherArgs;
    otherArgs.insert(QByteArray("oauth_verifier"), pin.toAscii());

    QOAuth::ParamMap reply = qoauth->accessToken(serviceOAuthAccessTokenUrl, QOAuth::POST, token.toAscii(), tokenSecret.toAscii(), QOAuth::HMAC_SHA1, otherArgs);

    if (qoauth->error() == QOAuth::NoError) {
        QMessageBox::information(this, "", "Authorization successfull!");
        token = reply.value(QOAuth::tokenParameterName());
        tokenSecret = reply.value(QOAuth::tokenSecretParameterName());
        accountUsernameLineEdit->setText(reply.value("screen_name"));
        setEnabled(true);
    } else {
        setEnabled(true);
        QMessageBox::critical(this, "Error!", "Wrong pin, try again!");
        oauthDialog->showNormal();
    }
}

void AccountConfigurationDialog::stopAuthorization() {
    setEnabled(true);
}

#endif
