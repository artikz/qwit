/*!
 *  $Id: OptionsDialog.cpp 201 2009-07-21 16:04:47Z artem.iglikov $
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
 *  OptionsDialog class implementation
 */

#ifndef OptionsDialog_cpp
#define OptionsDialog_cpp

#include "QwitHeaders.h"

#include "MainWindow.h"
#include "OptionsDialog.h"
#include "Configuration.h"
#include "Account.h"
#include "Services.h"

OptionsDialog::OptionsDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);
	
	accountConfigurationDialog = new AccountConfigurationDialog(this);
	
	connect(optionsGroupListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(changeOptionsGroup(QListWidgetItem*)));
	optionsStackedWidget->setCurrentWidget(accountsPage);
	connect(addAccountPushButton, SIGNAL(pressed()), this, SLOT(addAccount()));
	connect(deleteAccountPushButton, SIGNAL(pressed()), this, SLOT(deleteAccount()));
	connect(editAccountPushButton, SIGNAL(pressed()), this, SLOT(editAccount()));
	connect(accountConfigurationDialog, SIGNAL(accepted()), this, SLOT(commitAccount()));
	
	Configuration *config = Configuration::getInstance();
	for (int i = 0; i < config->UrlShorteners.size(); ++i) {
		urlShortenersComboBox->addItem(QIcon(":/images/" + config->UrlShorteners[i] + ".png"), config->UrlShortenersNames[config->UrlShorteners[i]]);
	}
	for (int i = 0; i < config->Services.size(); ++i) {
		servicesComboBox->addItem(QIcon(":/images/" + config->Services[i] + ".png"), config->ServicesNames[config->Services[i]]);
	}
	for (int i = 0; i < config->TranslationsTitles.size(); ++i) {
		translationsComboBox->addItem(QIcon(":/images/countries/" + config->TranslationsCodes[i].mid(3, 2).toLower() + ".png"), config->TranslationsTitles[i]);
	}
	translationsComboBox->setCurrentIndex(config->TranslationsCodes.indexOf(config->language));
	servicesComboBox->setCurrentIndex(config->Services.indexOf("twitter"));

	optionsStackedWidget->setCurrentWidget(accountsPage);

	connect(commonMessagesEvenColorPushButton, SIGNAL(pressed()), this, SLOT(chooseCommonMessagesEvenColor()));
	connect(commonMessagesOddColorPushButton, SIGNAL(pressed()), this, SLOT(chooseCommonMessagesOddColor()));
	connect(mentionsEvenColorPushButton, SIGNAL(pressed()), this, SLOT(chooseMentionsEvenColor()));
	connect(mentionsOddColorPushButton, SIGNAL(pressed()), this, SLOT(chooseMentionsOddColor()));
}

void OptionsDialog::changeOptionsGroup(QListWidgetItem *item) {
	if (item->text() == tr("Accounts")) {
		optionsStackedWidget->setCurrentWidget(accountsPage);
	} else if (item->text() == tr("URL shortener")) {
		optionsStackedWidget->setCurrentWidget(urlShortenerPage);
	} else if (item->text() == tr("Connection")) {
		optionsStackedWidget->setCurrentWidget(connectionPage);
	} else if (item->text() == tr("User interface")) {
		optionsStackedWidget->setCurrentWidget(userInterfacePage);
	}
}

void OptionsDialog::addAccount() {
	accountConfigurationDialog->action = AccountConfigurationDialog::ActionAdd;
	accountConfigurationDialog->accountType = servicesComboBox->currentIndex();
	accountConfigurationDialog->accountUsernameLineEdit->setText("");
	accountConfigurationDialog->accountPasswordLineEdit->setText("");
	accountConfigurationDialog->accountUsernameLineEdit->setFocus();
	accountConfigurationDialog->useHttpsCheckBox->setChecked(Qt::Unchecked);
	int serviceId = servicesComboBox->currentIndex();
	if (serviceId == Configuration::ServicesIds["custom"]) {
		accountConfigurationDialog->serviceBaseUrlLineEdit->setText("");
		accountConfigurationDialog->serviceApiUrlLineEdit->setText("");
		accountConfigurationDialog->useHttpsCheckBox->setEnabled(false);
		accountConfigurationDialog->serviceBaseUrlLineEdit->setEnabled(true);
		accountConfigurationDialog->serviceApiUrlLineEdit->setEnabled(true);
	} else {
		accountConfigurationDialog->serviceBaseUrlLineEdit->setText(Services::options[Configuration::Services[serviceId]]["baseurl"]);
		accountConfigurationDialog->serviceApiUrlLineEdit->setText(Services::options[Configuration::Services[serviceId]]["apiurl"]);
		accountConfigurationDialog->useHttpsCheckBox->setEnabled(true);
		accountConfigurationDialog->serviceBaseUrlLineEdit->setEnabled(false);
		accountConfigurationDialog->serviceApiUrlLineEdit->setEnabled(false);
	}
	accountConfigurationDialog->showNormal();
}

void OptionsDialog::deleteAccount() {
	int accountId = accountsListWidget->currentRow();
	if (accountId == -1) return;
	QMessageBox messageBox(this);
	messageBox.setText("Do you really want to delete account \"" + accountsListWidget->currentItem()->text() + "\"?");
	messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	messageBox.setDefaultButton(QMessageBox::Ok);
	if (messageBox.exec() == QMessageBox::Ok) {
		Configuration *config = Configuration::getInstance();
		accountsListWidget->takeItem(accountId);
		MainWindow *mainWindow = MainWindow::getInstance();
		mainWindow->deleteAccountButton(config->accounts[accountId]);
		config->deleteAccount(accountId);
	}
}

void OptionsDialog::editAccount() {
	int accountId = accountsListWidget->currentRow();
	if (accountId == -1) return;
	Configuration *config = Configuration::getInstance();
	accountConfigurationDialog->action = AccountConfigurationDialog::ActionEdit;
	accountConfigurationDialog->accountId = accountId;
	accountConfigurationDialog->accountUsernameLineEdit->setText(config->accounts[accountId]->username);
	accountConfigurationDialog->accountPasswordLineEdit->setText(config->accounts[accountId]->password);
	accountConfigurationDialog->serviceBaseUrlLineEdit->setText(config->accounts[accountId]->serviceBaseUrl());
	accountConfigurationDialog->serviceApiUrlLineEdit->setText(config->accounts[accountId]->serviceApiUrl());
	if (config->accounts[accountId]->serviceBaseUrl().startsWith("https://")) {
		accountConfigurationDialog->useHttpsCheckBox->setChecked(Qt::Checked);
	} else {
		accountConfigurationDialog->useHttpsCheckBox->setChecked(Qt::Unchecked);
	}
	accountConfigurationDialog->accountUsernameLineEdit->setFocus();
	if (config->accounts[accountId]->type == "custom") {
		accountConfigurationDialog->useHttpsCheckBox->setChecked(Qt::Unchecked);
		accountConfigurationDialog->useHttpsCheckBox->setEnabled(false);
		accountConfigurationDialog->serviceBaseUrlLineEdit->setEnabled(true);
		accountConfigurationDialog->serviceApiUrlLineEdit->setEnabled(true);
	} else {
		accountConfigurationDialog->useHttpsCheckBox->setEnabled(true);
		accountConfigurationDialog->serviceBaseUrlLineEdit->setEnabled(false);
		accountConfigurationDialog->serviceApiUrlLineEdit->setEnabled(false);
	}
	accountConfigurationDialog->showNormal();
}

void OptionsDialog::commitAccount() {
	Configuration *config = Configuration::getInstance();
	MainWindow *mainWindow = MainWindow::getInstance();
	switch (accountConfigurationDialog->action) {
		case AccountConfigurationDialog::ActionAdd: {
                                Account *account = new Account(
                                        Configuration::Services[accountConfigurationDialog->accountType],
                                        accountConfigurationDialog->accountUsernameLineEdit->text(),
                                        accountConfigurationDialog->accountPasswordLineEdit->text(),
                                        accountConfigurationDialog->useHttpsCheckBox->checkState() == Qt::Checked,
                                        accountConfigurationDialog->serviceBaseUrlLineEdit->text(),
                                        accountConfigurationDialog->serviceApiUrlLineEdit->text());
				config->addAccount(account);
				accountsListWidget->addItem(new QListWidgetItem(QIcon(":/images/" + account->type + ".png"), account->username));
				accountsListWidget->setCurrentRow(account->id);
				mainWindow->addAccountButton(account);
			}
			break;
		case AccountConfigurationDialog::ActionEdit: {
				Account *account = config->accounts[accountConfigurationDialog->accountId];
				account->username = accountConfigurationDialog->accountUsernameLineEdit->text();
				account->password = accountConfigurationDialog->accountPasswordLineEdit->text();
				account->useHttps = (accountConfigurationDialog->useHttpsCheckBox->checkState() == Qt::Checked);
                                account->_serviceBaseUrl = accountConfigurationDialog->serviceBaseUrlLineEdit->text();
                                account->_serviceApiUrl = accountConfigurationDialog->serviceApiUrlLineEdit->text();
                                accountsListWidget->takeItem(account->id);
				accountsListWidget->insertItem(account->id, new QListWidgetItem(QIcon(":/images/" + account->type + ".png"), account->username));
				accountsListWidget->setCurrentRow(account->id);
				mainWindow->updateAccountButton(account);
			}
			break;
	}
}

void OptionsDialog::chooseCommonMessagesEvenColor() {
	QPalette palette = commonMessagesEvenColorPushButton->palette();
	QColor color = QColorDialog::getColor(commonMessagesEvenColorPushButton->palette().color(QPalette::Button), this);
	if (color.isValid()) {
		palette.setColor(QPalette::Button, color);
		commonMessagesEvenColorPushButton->setPalette(palette);
	}
}

void OptionsDialog::chooseCommonMessagesOddColor() {
	QPalette palette = commonMessagesOddColorPushButton->palette();
	QColor color = QColorDialog::getColor(commonMessagesOddColorPushButton->palette().color(QPalette::Button), this);
	if (color.isValid()) {
		palette.setColor(QPalette::Button, color);
		commonMessagesOddColorPushButton->setPalette(palette);
	}
}

void OptionsDialog::chooseMentionsEvenColor() {
	QPalette palette = mentionsEvenColorPushButton->palette();
	QColor color = QColorDialog::getColor(mentionsEvenColorPushButton->palette().color(QPalette::Button), this);
	if (color.isValid()) {
		palette.setColor(QPalette::Button, color);
		mentionsEvenColorPushButton->setPalette(palette);
	}
}

void OptionsDialog::chooseMentionsOddColor() {
	QPalette palette = mentionsOddColorPushButton->palette();
	QColor color = QColorDialog::getColor(mentionsOddColorPushButton->palette().color(QPalette::Button), this);
	if (color.isValid()) {
		palette.setColor(QPalette::Button, color);
		mentionsOddColorPushButton->setPalette(palette);
	}
}

#endif
