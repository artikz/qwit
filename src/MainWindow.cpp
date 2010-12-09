/*!
 *  $Id: MainWindow.cpp 213 2009-07-30 12:40:27Z artem.iglikov $
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
 *  MainWindow class implementation
 */

#ifndef MainWindow_cpp
#define MainWindow_cpp

#include "QwitHeaders.h"

#include "TwitterWidget.h"
#include "MainWindow.h"
#include "UserpicsDownloader.h"
#include "Configuration.h"
#include "TwitPicDialog.h"
#include "QwitTools.h"

MainWindow* MainWindow::instance = 0;

MainWindow* MainWindow::getInstance() {
	if (!instance) {
		instance = new MainWindow();
	}
	return instance;
}


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
	qDebug() << ("MainWindow::MainWindow()");

	instance = this;

	setupUi(this);

	greetingMessageLabel = new QLabel(this);
	leftCharactersNumberLabel = new QLabel(this);

	messageTextEdit = new MessageTextEdit(this);

	messageTextEdit->setObjectName(QString::fromUtf8("messageTextEdit"));
	messageHorizontalLayout->insertWidget(0, messageTextEdit);
	connect(messageTextEdit, SIGNAL(leftCharsNumberChanged(int)), this, SLOT(leftCharsNumberChanged(int)));
	connect(this, SIGNAL(retweet(const Message &)), messageTextEdit, SLOT(retweet(const Message &)));
	connect(this, SIGNAL(reply(const Message &)), messageTextEdit, SLOT(reply(const Message &)));
	
	lastMessageLabel->setTextFormat(Qt::AutoText);
	
	optionsDialog = new OptionsDialog(this);
	connect(optionsDialog, SIGNAL(accepted()), this, SLOT(saveOptions()));
	connect(optionsDialog, SIGNAL(rejected()), this, SLOT(resetOptionsDialog()));
	connect(optionsDialog, SIGNAL(rejected()), this, SLOT(ensureThereAreAccounts()));

	aboutDialog = new AboutDialog(this);
	
	directMessageDialog = new DirectMessageDialog(this);
	connect(directMessageDialog , SIGNAL(accepted()), this, SLOT(sendDirectMessage()));

	connect(urlShorteningEnabledButton, SIGNAL(toggled(bool)), UrlShortener::getInstance(), SLOT(setShorteningEnabled(bool)));
	connect(urlShorteningEnabledButton, SIGNAL(toggled(bool)), this, SLOT(updateUrlShorteningButtonTooltip(bool)));
	UrlShortener::getInstance()->setShorteningEnabled(urlShorteningEnabledButton->isChecked());
	updateUrlShorteningButtonTooltip(urlShorteningEnabledButton->isChecked());
	connect(twitPicButton, SIGNAL(clicked()), this, SLOT(postTwitPic()));

	friendsMgmtDialog = new FriendsMgmtDialog(this);

	connect(refreshToolButton, SIGNAL(clicked()), this, SLOT(refresh()));
	connect(optionsToolButton, SIGNAL(clicked()), this, SLOT(showOptionsDialog()));
	connect(aboutToolButton, SIGNAL(clicked()), aboutDialog, SLOT(show()));
	connect(exitToolButton, SIGNAL(clicked()), this, SLOT(quit()));
	connect(friendsMgmtToolButton, SIGNAL(clicked()), friendsMgmtDialog, SLOT(show()));

	connect(&accountsButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(accountButtonClicked(int)));
	
	connect(mainTabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
	mainTabWidget->removeTab(0);
	
	accountsLayout = 0;
	
	redrawTimer = new QTimer(this);
	connect(redrawTimer, SIGNAL(timeout()), this, SLOT(redrawPages()));
	redrawTimer->start(10000);
	
	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateAll()));

	setupTrayIcon();
	loadState();
	if (accountsButtons.size() > 0) {
		accountsButtons[0]->setChecked(true);
		accountsButtons[0]->click();
	}
	
	acceptClose = false;
	
	connect(UserpicsDownloader::getInstance(), SIGNAL(userpicDownloaded()), this, SLOT(reloadUserpics()));
	
	connect(UrlShortener::getInstance(), SIGNAL(urlShortened(const QString &)), messageTextEdit, SLOT(insertUrl(const QString &)));

	updateAll(true);

	Configuration *config = Configuration::getInstance();
	if (config->accounts.size() == 0) {
		showOptionsDialog();
    }
}

void MainWindow::leftCharsNumberChanged(int count) {
	leftCharactersNumberLabel->setText(QString::number(count));
}

void MainWindow::loadState() {
	qDebug() << ("MainWindow::loadState()");

	Configuration *config = Configuration::getInstance();
	
	config->load();

	updateState();
	
	for (int i = 0; i < config->accounts.size(); ++i) {
		addAccountButton(config->accounts[i]);
	}
	
	resetOptionsDialog();
}

void MainWindow::saveState() {
	qDebug() << ("MainWindow::saveState()");

	Configuration *config = Configuration::getInstance();
	
	config->position = pos();
	config->size = size();
	
	config->save();
}

void MainWindow::saveOptions() {
	qDebug() << ("MainWindow::saveOptions()");

	Configuration *config = Configuration::getInstance();

	config->showGreetingMessage = (optionsDialog->showGreetingMessageCheckBox->checkState() == Qt::Checked);
	config->greetingMessage = optionsDialog->greetingMessageLineEdit->text();
	config->showLeftCharactersNumber = (optionsDialog->showLeftCharactersNumberCheckBox->checkState() == Qt::Checked);
	config->showLastMessage = (optionsDialog->showLastMessageCheckBox->checkState() == Qt::Checked);
	config->messagesPerPage = optionsDialog->messagesPerPageLineEdit->text().toInt();
	config->messagesInPopup = optionsDialog->messagesInPopupLineEdit->text().toInt();
	config->retweetTag = optionsDialog->retweetTagLineEdit->text();
	config->retweetTagAfterText = (optionsDialog->retweetTagAfterTextCheckBox->checkState() == Qt::Checked);
	config->placeControlsVertically = (optionsDialog->placeControlsVerticallyCheckBox->checkState() == Qt::Checked);
	config->placeTabsVertically = (optionsDialog->placeTabsVerticallyCheckBox->checkState() == Qt::Checked);
	config->showMessagesInTray = (optionsDialog->showMessagesInTrayCheckBox->checkState() == Qt::Checked);
	config->placeUsernameUnderAvatar = (optionsDialog->placeUsernameUnderAvatarCheckBox->checkState() == Qt::Checked);
    config->startMinimized = (optionsDialog->startMinimizedCheckBox->checkState() == Qt::Checked);

	config->showHomeTab = (optionsDialog->homeTabCheckBox->checkState() == Qt::Checked);
	config->autoUpdateHomeTab = (optionsDialog->homeTabAutoUpdateCheckBox->checkState() == Qt::Checked);
	config->showPublicTab = (optionsDialog->publicTabCheckBox->checkState() == Qt::Checked);
	config->autoUpdatePublicTab = (optionsDialog->publicTabAutoUpdateCheckBox->checkState() == Qt::Checked);
	config->showRepliesTab = (optionsDialog->repliesTabCheckBox->checkState() == Qt::Checked);
	config->autoUpdateRepliesTab = (optionsDialog->repliesTabAutoUpdateCheckBox->checkState() == Qt::Checked);
	config->showCustomTab = (optionsDialog->customTabCheckBox->checkState() == Qt::Checked);
	config->autoUpdateCustomTab = (optionsDialog->customTabAutoUpdateCheckBox->checkState() == Qt::Checked);
	config->showInboxTab = (optionsDialog->inboxTabCheckBox->checkState() == Qt::Checked);
	config->autoUpdateInboxTab = (optionsDialog->inboxTabAutoUpdateCheckBox->checkState() == Qt::Checked);
	config->showOutboxTab = (optionsDialog->outboxTabCheckBox->checkState() == Qt::Checked);
	config->autoUpdateOutboxTab = (optionsDialog->outboxTabAutoUpdateCheckBox->checkState() == Qt::Checked);
	config->showSearchTab = (optionsDialog->searchTabCheckBox->checkState() == Qt::Checked);
	config->autoUpdateSearchTab = (optionsDialog->searchTabAutoUpdateCheckBox->checkState() == Qt::Checked);
	config->showFavoritesTab = (optionsDialog->favoritesTabCheckBox->checkState() == Qt::Checked);
	config->autoUpdateFavoritesTab = (optionsDialog->favoritesTabAutoUpdateCheckBox->checkState() == Qt::Checked);
	config->updateInterval = optionsDialog->updateIntervalSpinBox->value();

	config->commonMessagesEvenColor = optionsDialog->commonMessagesEvenColorPushButton->palette().color(QPalette::Button);
	config->commonMessagesOddColor = optionsDialog->commonMessagesOddColorPushButton->palette().color(QPalette::Button);
	config->mentionsEvenColor = optionsDialog->mentionsEvenColorPushButton->palette().color(QPalette::Button);
	config->mentionsOddColor = optionsDialog->mentionsOddColorPushButton->palette().color(QPalette::Button);
	config->language = config->TranslationsCodes[optionsDialog->translationsComboBox->currentIndex()];
    config->notificationSubsystem = (optionsDialog->kdialogRadioButton->isChecked() ? "kdialog" : (optionsDialog->libnotifyRadioButton->isChecked() ? "libnotify" : "qt"));

	config->useProxy = (optionsDialog->useProxyCheckBox->checkState() == Qt::Checked);
	config->proxyAddress = optionsDialog->proxyAddressLineEdit->text();
	config->proxyPort = optionsDialog->proxyPortLineEdit->text().toInt();
	config->proxyUsername = optionsDialog->proxyUsernameLineEdit->text();
	config->proxyPassword = optionsDialog->proxyPasswordLineEdit->text();
	
	config->urlShortener = Configuration::UrlShorteners[optionsDialog->urlShortenersComboBox->currentIndex()];
	config->urlShortenerUsername = optionsDialog->urlShortenersUsername->text();
	config->urlShortenerAPIKey = optionsDialog->urlShortenersAPIKey->text();

	saveState();
	updateState();

	ensureThereAreAccounts();
}

void MainWindow::updateState() {
	qDebug() << ("MainWindow::updateState()");

	Configuration *config = Configuration::getInstance();

	if ((greetingMessageLabel->isVisible() != config->showGreetingMessage) || (leftCharactersNumberLabel->isVisible() != config->showLeftCharactersNumber)) {
		if (greetingMessageLabel->isVisible()) {
			QLayout *layout = (QLayout*)verticalLayout->itemAt(0);
			verticalLayout->removeItem(verticalLayout->itemAt(0));
			delete layout;
		} else {
			toolbuttonsVerticalLayout->removeWidget(leftCharactersNumberLabel);
		}
		if (config->showGreetingMessage) {
			QHBoxLayout *layout = new QHBoxLayout(this);
			layout->addWidget(greetingMessageLabel);
			layout->addStretch();
			if (config->showLeftCharactersNumber) {
				layout->addWidget(leftCharactersNumberLabel);
			}
			verticalLayout->insertLayout(0, layout);
		} else {
			if (config->showLeftCharactersNumber) {
				toolbuttonsVerticalLayout->insertWidget(0, leftCharactersNumberLabel);
			}
		}
		greetingMessageLabel->setVisible(config->showGreetingMessage);
		leftCharactersNumberLabel->setVisible(config->showLeftCharactersNumber);
	}

	greetingMessageLabel->setText(config->greetingMessage);
	leftCharactersNumberLabel->setText(QString::number(MessageTextEdit::MaxMessageCharacters - messageTextEdit->toPlainText().length()));
	lastMessageLabel->setVisible(config->showLastMessage);

	if (config->placeTabsVertically) {
		mainTabWidget->setTabPosition(QTabWidget::West);
	} else {
		mainTabWidget->setTabPosition(QTabWidget::South);
	}
	
	for (int i = 0; i < pages.size(); ++i) {
		mainTabWidget->removeTab(i);
		delete pages[i];
		pages[i] = 0;
	}
	pages.clear();

	homePage = 0;
	repliesPage = 0;
	publicPage = 0;
	favoritesPage = 0;
	inboxPage = 0;
	outboxPage = 0;
	searchPage = 0;
	
	if (config->showHomeTab) {
		pages.push_back(homePage = new HomePage());
	}
	if (config->showPublicTab) {
		pages.push_back(publicPage = new PublicPage());
	}
	if (config->showRepliesTab) {
		pages.push_back(repliesPage = new RepliesPage());
	}
	if (config->showFavoritesTab) {
		pages.push_back(favoritesPage = new FavoritesPage());
	}
	if (config->showInboxTab) {
		pages.push_back(inboxPage = new InboxPage());
	}
	if (config->showOutboxTab) {
		pages.push_back(outboxPage = new OutboxPage());
	}
	if (config->showSearchTab) {
		pages.push_back(searchPage = new SearchPage());
		searchPage->lineEdit->setText(config->searchQuery);
	}

	for (int i = 0; i < pages.size(); ++i) {
		mainTabWidget->addTab(pages[i], pages[i]->title());
	}

	urlShorteningEnabledButton->setIcon(QIcon(":/images/" + config->urlShortener + ".png"));
	
	updateCurrentAccount(config->currentAccountId);
	
	move(config->position);
	resize(config->size);
	for (int i = 0; i < pages.size(); ++i) {
		pages[i]->updateSize();
	}

	updateTimer->start(config->updateInterval * 1000);
}

void MainWindow::resetOptionsDialog() {
	qDebug() << ("MainWindow::resetOptionsDialog()");

	Configuration *config = Configuration::getInstance();
	
// User interface
	optionsDialog->showGreetingMessageCheckBox->setCheckState(config->showGreetingMessage ? Qt::Checked : Qt::Unchecked);
	optionsDialog->greetingMessageLineEdit->setText(config->greetingMessage);
	optionsDialog->showLeftCharactersNumberCheckBox->setCheckState(config->showLeftCharactersNumber ? Qt::Checked : Qt::Unchecked);
	optionsDialog->showLastMessageCheckBox->setCheckState(config->showLastMessage ? Qt::Checked : Qt::Unchecked);
	optionsDialog->messagesPerPageLineEdit->setText(QString::number(config->messagesPerPage));
	optionsDialog->messagesInPopupLineEdit->setText(QString::number(config->messagesInPopup));
	optionsDialog->retweetTagAfterTextCheckBox->setCheckState(config->retweetTagAfterText ? Qt::Checked : Qt::Unchecked);
	optionsDialog->retweetTagLineEdit->setText(config->retweetTag);
	optionsDialog->placeControlsVerticallyCheckBox->setCheckState(config->placeControlsVertically ? Qt::Checked : Qt::Unchecked);
	optionsDialog->placeTabsVerticallyCheckBox->setCheckState(config->placeTabsVertically ? Qt::Checked : Qt::Unchecked);
	optionsDialog->showMessagesInTrayCheckBox->setCheckState(config->showMessagesInTray ? Qt::Checked : Qt::Unchecked);
	optionsDialog->placeUsernameUnderAvatarCheckBox->setCheckState(config->placeUsernameUnderAvatar ? Qt::Checked : Qt::Unchecked);
    optionsDialog->startMinimizedCheckBox->setCheckState(config->startMinimized ? Qt::Checked : Qt::Unchecked);

	optionsDialog->homeTabCheckBox->setCheckState(config->showHomeTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->homeTabAutoUpdateCheckBox->setCheckState(config->autoUpdateHomeTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->publicTabCheckBox->setCheckState(config->showPublicTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->publicTabAutoUpdateCheckBox->setCheckState(config->autoUpdatePublicTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->repliesTabCheckBox->setCheckState(config->showRepliesTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->repliesTabAutoUpdateCheckBox->setCheckState(config->autoUpdateRepliesTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->customTabCheckBox->setCheckState(config->showCustomTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->customTabAutoUpdateCheckBox->setCheckState(config->autoUpdateCustomTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->inboxTabCheckBox->setCheckState(config->showInboxTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->inboxTabAutoUpdateCheckBox->setCheckState(config->autoUpdateInboxTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->outboxTabCheckBox->setCheckState(config->showOutboxTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->outboxTabAutoUpdateCheckBox->setCheckState(config->autoUpdateOutboxTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->searchTabCheckBox->setCheckState(config->showSearchTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->searchTabAutoUpdateCheckBox->setCheckState(config->autoUpdateSearchTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->favoritesTabCheckBox->setCheckState(config->showFavoritesTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->favoritesTabAutoUpdateCheckBox->setCheckState(config->autoUpdateFavoritesTab ? Qt::Checked : Qt::Unchecked);
	optionsDialog->updateIntervalSpinBox->setValue(config->updateInterval);

	QPalette palette = optionsDialog->commonMessagesEvenColorPushButton->palette();
	palette.setColor(QPalette::Button, config->commonMessagesEvenColor);
	optionsDialog->commonMessagesEvenColorPushButton->setPalette(palette);

	palette = optionsDialog->commonMessagesOddColorPushButton->palette();
	palette.setColor(QPalette::Button, config->commonMessagesOddColor);
	optionsDialog->commonMessagesOddColorPushButton->setPalette(palette);

	palette = optionsDialog->mentionsEvenColorPushButton->palette();
	palette.setColor(QPalette::Button, config->mentionsEvenColor);
	optionsDialog->mentionsEvenColorPushButton->setPalette(palette);

	palette = optionsDialog->mentionsOddColorPushButton->palette();
	palette.setColor(QPalette::Button, config->mentionsOddColor);
	optionsDialog->mentionsOddColorPushButton->setPalette(palette);

	optionsDialog->translationsComboBox->setCurrentIndex(config->TranslationsCodes.indexOf(config->language));

    if (config->notificationSubsystem == "qt") {
        optionsDialog->qtRadioButton->setChecked(true);
        optionsDialog->kdialogRadioButton->setChecked(false);
        optionsDialog->libnotifyRadioButton->setChecked(false);
    } else if (config->notificationSubsystem == "kdialog") {
        optionsDialog->qtRadioButton->setChecked(false);
        optionsDialog->kdialogRadioButton->setChecked(true);
        optionsDialog->libnotifyRadioButton->setChecked(false);
    } else if (config->notificationSubsystem == "libnotify") {
        optionsDialog->qtRadioButton->setChecked(false);
        optionsDialog->kdialogRadioButton->setChecked(false);
        optionsDialog->libnotifyRadioButton->setChecked(true);
    }

// Accounts
	optionsDialog->accountsListWidget->clear();
	for (int i = 0; i < config->accounts.size(); ++i) {
		optionsDialog->accountsListWidget->addItem(new QListWidgetItem(QIcon(":/images/" + config->accounts[i]->type + ".png"), config->accounts[i]->username));
	}

// Connection
	optionsDialog->useProxyCheckBox->setCheckState(config->useProxy ? Qt::Checked : Qt::Unchecked);
	optionsDialog->proxyAddressLineEdit->setText(config->proxyAddress);
	optionsDialog->proxyPortLineEdit->setText(QString::number(config->proxyPort));
	optionsDialog->proxyUsernameLineEdit->setText(config->proxyUsername);
	optionsDialog->proxyPasswordLineEdit->setText(config->proxyPassword);

// UrlShortener
	optionsDialog->urlShortenersComboBox->setCurrentIndex(Configuration::UrlShortenersIds[config->urlShortener]);
	optionsDialog->urlShortenersUsername->setText(config->urlShortenerUsername);
	optionsDialog->urlShortenersAPIKey->setText(config->urlShortenerAPIKey);

}

void MainWindow::addAccountButton(Account *account) {
	qDebug() << ("MainWindow::addAccountButton()");

	QObject::connect(account, SIGNAL(newMessagesReceived(const QVector<Message>&, Account *)), this, SLOT(showNewMessages(const QVector<Message>&, Account *)));

	QToolButton *accountButton = new QToolButton(this);
	accountButton->setIcon(QIcon(":/images/" + account->type + ".png"));
	accountButton->setText(account->username);
	accountButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	accountButton->setAutoRaise(true);
	accountButton->setCheckable(true);
	accountsButtons.push_back(accountButton);
	accountsButtonGroup.addButton(accountButton, accountsButtons.size() - 1);
	if (accountsButtons.size() == 1) {
		accountsButtons[0]->setVisible(false);
	}
	if (accountsButtons.size() == 2) {
		accountsLayout = new QHBoxLayout();
		accountsLayout->addWidget(accountsButtons[0]);
		accountsButtons[0]->setVisible(true);
		verticalLayout->insertLayout(verticalLayout->count() - 1, accountsLayout);
		accountsLayout->addStretch();
	}
	if (accountsButtons.size() >= 2) {
		accountsLayout->insertWidget(accountsLayout->count() - 1, accountButton);
	}
}

void MainWindow::updateAccountButton(Account *account) {
	qDebug() << ("MainWindow::updateAccountButton()");

	accountsButtons[account->id]->setText(account->username);
}

void MainWindow::deleteAccountButton(Account *account) {
	qDebug() << ("MainWindow::deleteAccountButton()");

	if (accountsLayout) {
		accountsLayout->removeWidget(accountsButtons[account->id]);
	}
	int checkedId = accountsButtonGroup.checkedId();
	accountsButtonGroup.removeButton(accountsButtons[account->id]);
	delete accountsButtons[account->id];
	accountsButtons.erase(accountsButtons.begin() + account->id);
	if (accountsButtons.size() <= 1) {
		if (accountsButtons.size() == 1) {
			accountsLayout->removeWidget(accountsButtons[0]);
			accountsButtons[0]->setVisible(false);
		}
		verticalLayout->removeItem(accountsLayout);
		delete accountsLayout;
		accountsLayout = 0;
	}
	for (int i = 0; i < accountsButtons.size(); ++i) {
		accountsButtonGroup.removeButton(accountsButtons[i]);
	}
	for (int i = 0; i < accountsButtons.size(); ++i) {
		accountsButtonGroup.addButton(accountsButtons[i], i);
	}
	if (checkedId == account->id) {
		checkedId = min(accountsButtons.size() - 1, checkedId);
	} else if (checkedId > account->id) {
		--checkedId;
	}
	if (checkedId != -1) {
		accountsButtons[checkedId]->setChecked(true);
		accountButtonClicked(checkedId);
	}
}

void MainWindow::showOptionsDialog() {
	qDebug() << ("MainWindow::showOptionsDialog()");

	resetOptionsDialog();
	optionsDialog->showNormal();
	optionsToolButton->setChecked(false);
}

void MainWindow::accountButtonClicked(int id) {
	qDebug() << ("MainWindow::accountButtonClicked()");

	Configuration *config = Configuration::getInstance();
	setWindowTitle("Qwit - " + config->accounts[id]->username + "@" + config->accounts[id]->type);
	updateCurrentAccount(id);
}

void MainWindow::updateCurrentAccount(int id) {
	qDebug() << ("MainWindow::updateCurrentAccount()");

	Configuration *config = Configuration::getInstance();
	if (config->accounts.size() == 0) {
		return;
	}
	if (config->currentAccountId == -1) {
		return;
	}
	
	int oldAccountId = config->currentAccountId;
	config->currentAccountId = id;

    Account *account = config->currentAccount();
    twitPicButton->setEnabled((account->type == "twitter") && account->useOAuth);

    if (homePage) {
		disconnect(config->accounts[oldAccountId], SIGNAL(friendsMessagesUpdated(const QVector<Message> &, Account *)), 0, 0);
		connect(config->currentAccount(), SIGNAL(friendsMessagesUpdated(const QVector<Message> &, Account *)), homePage, SLOT(updateItems(const QVector<Message> &, Account *)));
		homePage->updateItems(config->currentAccount()->friendsMessages, config->currentAccount());
		disconnect(config->accounts[oldAccountId], SIGNAL(previousFriendsMessagesReceived()), 0, 0);
		connect(config->currentAccount(), SIGNAL(previousFriendsMessagesReceived()), homePage->twitterWidget, SLOT(enableMoreButton()));
	}
	if (repliesPage) {
		disconnect(config->accounts[oldAccountId], SIGNAL(repliesUpdated(const QVector<Message> &, Account *)), 0, 0);
		connect(config->currentAccount(), SIGNAL(repliesUpdated(const QVector<Message> &, Account *)), repliesPage, SLOT(updateItems(const QVector<Message> &, Account *)));
		repliesPage->updateItems(config->currentAccount()->replies, config->currentAccount());
		disconnect(config->accounts[oldAccountId], SIGNAL(previousRepliesReceived()), 0, 0);
		connect(config->currentAccount(), SIGNAL(previousRepliesReceived()), repliesPage->twitterWidget, SLOT(enableMoreButton()));
	}
	if (publicPage) {
		disconnect(config->accounts[oldAccountId], SIGNAL(publicMessagesUpdated(const QVector<Message> &, Account *)), 0, 0);
		connect(config->currentAccount(), SIGNAL(publicMessagesUpdated(const QVector<Message> &, Account *)), publicPage, SLOT(updateItems(const QVector<Message> &, Account *)));
		publicPage->updateItems(config->currentAccount()->publicMessages, config->currentAccount());
		disconnect(config->accounts[oldAccountId], SIGNAL(previousPublicMessagesReceived()), 0, 0);
		connect(config->currentAccount(), SIGNAL(previousPublicMessagesReceived()), publicPage->twitterWidget, SLOT(enableMoreButton()));
	}
	if (favoritesPage) {
		disconnect(config->accounts[oldAccountId], SIGNAL(favoritesUpdated(const QVector<Message> &, Account *)), 0, 0);
		connect(config->currentAccount(), SIGNAL(favoritesUpdated(const QVector<Message> &, Account *)), favoritesPage, SLOT(updateItems(const QVector<Message> &, Account *)));
		favoritesPage->updateItems(config->currentAccount()->favorites, config->currentAccount());
		disconnect(config->accounts[oldAccountId], SIGNAL(previousFavoritesReceived()), 0, 0);
		connect(config->currentAccount(), SIGNAL(previousFavoritesReceived()), favoritesPage->twitterWidget, SLOT(enableMoreButton()));
	}
	if (inboxPage) {
		disconnect(config->accounts[oldAccountId], SIGNAL(inboxMessagesUpdated(const QVector<Message> &, Account *)), 0, 0);
		connect(config->currentAccount(), SIGNAL(inboxMessagesUpdated(const QVector<Message> &, Account *)), inboxPage, SLOT(updateItems(const QVector<Message> &, Account *)));
		inboxPage->updateItems(config->currentAccount()->inboxMessages, config->currentAccount());
		disconnect(config->accounts[oldAccountId], SIGNAL(previousInboxMessagesReceived()), 0, 0);
		connect(config->currentAccount(), SIGNAL(previousInboxMessagesReceived()), inboxPage->twitterWidget, SLOT(enableMoreButton()));
	}
	if (outboxPage) {
		disconnect(config->accounts[oldAccountId], SIGNAL(outboxMessagesUpdated(const QVector<Message> &, Account *)), 0, 0);
		connect(config->currentAccount(), SIGNAL(outboxMessagesUpdated(const QVector<Message> &, Account *)), outboxPage, SLOT(updateItems(const QVector<Message> &, Account *)));
		outboxPage->updateItems(config->currentAccount()->outboxMessages, config->currentAccount());
		disconnect(config->accounts[oldAccountId], SIGNAL(previousOutboxMessagesReceived()), 0, 0);
		connect(config->currentAccount(), SIGNAL(previousOutboxMessagesReceived()), outboxPage->twitterWidget, SLOT(enableMoreButton()));
	}
	if (searchPage) {
		disconnect(config->accounts[oldAccountId], SIGNAL(searchMessagesUpdated(const QVector<Message> &, Account *)), 0, 0);
		connect(config->currentAccount(), SIGNAL(searchMessagesUpdated(const QVector<Message> &, Account *)), searchPage, SLOT(updateItems(const QVector<Message> &, Account *)));
		searchPage->updateItems(config->currentAccount()->searchMessages, config->currentAccount());
		disconnect(config->accounts[oldAccountId], SIGNAL(previousSearchMessagesReceived()), 0, 0);
		connect(config->currentAccount(), SIGNAL(previousSearchMessagesReceived()), searchPage->twitterWidget, SLOT(enableMoreButton()));
	}
	disconnect(config->accounts[oldAccountId], SIGNAL(lastMessageReceived(const QString &, Account *)), 0, 0);
	connect(config->currentAccount(), SIGNAL(lastMessageReceived(const QString &, Account *)), this, SLOT(updateLastMessage(const QString &, Account *)));
	disconnect(config->accounts[oldAccountId], SIGNAL(remainingRequestsUpdated(int, Account *)), 0, 0);
	connect(config->currentAccount(), SIGNAL(remainingRequestsUpdated(int, Account *)), this, SLOT(updateRemainingRequests(int, Account *)));
    disconnect(messageTextEdit, SIGNAL(messageEntered(const QString &, quint64, quint64)), 0, 0);
    connect(messageTextEdit, SIGNAL(messageEntered(const QString &, quint64, quint64)), config->currentAccount(), SLOT(sendMessage(const QString &, quint64, quint64)));
	messageTextEdit->setEnabled(!config->currentAccount()->sendingMessage);
	for (int i = 0; i < config->accounts.size(); ++i) {
		disconnect(config->accounts[i], 0, this, SLOT(messageSent(const QString &, Account *)));
		disconnect(config->accounts[i], 0, this, SLOT(messageNotSent(Account *)));
	}
	connect(config->currentAccount(), SIGNAL(messageSent(const QString &, Account *)), this, SLOT(messageSent(const QString &, Account *)));
	connect(config->currentAccount(), SIGNAL(messageNotSent(Account *)), this, SLOT(messageNotSent(Account *)));
	updateLastMessage(config->currentAccount()->lastMessage.text, config->currentAccount());
	updateRemainingRequests(config->currentAccount()->remainingRequests, config->currentAccount());
	messageTextEdit->completer->setModel(&config->currentAccount()->usernamesCacheModel);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	qDebug() << ("MainWindow::resizeEvent()");

	for (int i = 0; i < pages.size(); ++i) {
		pages[i]->updateSize();
	}
	event->ignore();
}

void MainWindow::showEvent(QShowEvent *event) {
	qDebug() << ("MainWindow::showEvent()");

	Configuration *config = Configuration::getInstance();
	resize(config->size);
	move(config->position);

	for (int i = 0; i < pages.size(); ++i) {
		pages[i]->updateSize();
	}
	
	messageTextEdit->setFocus(Qt::OtherFocusReason);

	event->accept();
}

void MainWindow::hideEvent(QHideEvent *event) {
	qDebug() << ("MainWindow::hideEvent()");

	saveState();
	event->accept();
}

void MainWindow::setupTrayIcon() {
	qDebug() << ("MainWindow::setupTrayIcon()");

        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setIcon(QIcon(":/images/qwit.png"));

#ifndef Q_OS_MAC //no context menu for trayicon in mac osx
	trayShowhideAction = new QAction(tr("&Show / Hide"), this);
	connect(trayShowhideAction, SIGNAL(triggered()), this, SLOT(showhide()));
	trayQuitAction = new QAction(tr("&Quit"), this);
	connect(trayQuitAction, SIGNAL(triggered()), this, SLOT(quit()));
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(trayShowhideAction);
	trayIconMenu->addAction(trayQuitAction);
	trayIcon->setContextMenu(trayIconMenu);
#endif
	trayIcon->show();
//	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(makeActive()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
	qDebug() << ("MainWindow::iconActivated()");
	trayIcon->setIcon(QIcon(":/images/qwit.png"));
	if (reason == QSystemTrayIcon::Trigger) {
		showhide();
	}
}

void MainWindow::showhide() {
	qDebug() << ("MainWindow::showhide()");

        if (isVisible() && isActiveWindow()) {
		hide();
	} else {
		show();
                raise();
                activateWindow();
//		for (int i = 0; i < TWITTER_TABS; ++i) {
//			twitterTabs[i].twitterWidget->updateItems();
//		}
	}
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
	qDebug() << ("MainWindow::keyPressEvent()");

	if (event->key() == Qt::Key_Escape) {
		showhide();
	} else if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_Q)) {
		acceptClose = true;
		quit();
	} else {
                QMainWindow::keyPressEvent(event);
	}
}

void MainWindow::quit() {
	qDebug() << ("MainWindow::quit()");
	trayIcon->hide();
	acceptClose = true;
	close();
}

void MainWindow::closeEvent(QCloseEvent *event) {
	qDebug() << ("MainWindow::closeEvent()");

	if (acceptClose) {
		saveState();
		event->accept();
	} else {
		hide();
		event->ignore();
	}
}

void MainWindow::refresh() {
	qDebug() << ("MainWindow::refresh()");

	pages[mainTabWidget->currentIndex()]->update();
	Configuration *config = Configuration::getInstance();
	config->currentAccount()->updateLastMessage();
}

void MainWindow::tabChanged(int tabIndex) {
	qDebug() << ("MainWindow::tabChanged()");
	if ((tabIndex >= 0) && (tabIndex < pages.size()) && pages[tabIndex]) {
		pages[tabIndex]->updateSize();
	}
}

void MainWindow::reloadUserpics() {
	qDebug() << ("MainWindow::reloadUserpics()");

	for (int i = 0; i < pages.size(); ++i) {
		pages[i]->reloadUserpics();
	}
}

void MainWindow::updateLastMessage(const QString &message, Account *account) {
	qDebug() << ("MainWindow::updateLastMessage()");
	lastMessageLabel->setText(QwitTools::prepareMessage(message, account));
}

void MainWindow::messageSent(const QString &message, Account *account) {
	qDebug() << ("MainWindow::messageSent()");
	messageTextEdit->setEnabled(true);
	messageTextEdit->clear();
	updateAccount(account);
}

void MainWindow::messageNotSent(Account *account) {
	qDebug() << ("MainWindow::messageNotSent()");
	messageTextEdit->setEnabled(true);
}

void MainWindow::showNewMessages(const QVector<Message> &messages, Account *account) {
	qDebug() << ("MainWindow::showNewMessages()");
	Configuration *config = Configuration::getInstance();
    if (min(messages.size(), config->messagesInPopup) != 0) {
		trayIcon->setIcon(QIcon(":/images/qwitnewmessages.png"));
		if (config->showMessagesInTray) {
            QString title = tr("Qwit: new messages receieved for %1@%2").arg(account->username).arg(account->type);
            if (config->notificationSubsystem == "qt") {
                QString trayMessage = "";
                for (int i = 0; i < min(messages.size(), config->messagesInPopup); ++i) {
                    if (trayMessage.length()) {
                        trayMessage += "----------------------------\n";
                    }
                    trayMessage += messages[i].username + ": " + messages[i].text + " /" + QwitTools::formatDateTime(messages[i].time.toLocalTime()) + "\n";
                }
                trayIcon->showMessage(title, trayMessage);
            } else if (config->notificationSubsystem == "libnotify") {
                for (int i = 0; i < min(messages.size(), config->messagesInPopup); ++i) {
                    QString trayMessage = messages[i].username + ": " + messages[i].text + " /" + QwitTools::formatDateTime(messages[i].time.toLocalTime());
                    QStringList args;
                    args << "-u" << "normal" << "-t" << "30000" << "-i" << "qwit" << "--" << title << trayMessage;
                    QProcess::execute("notify-send", args);
                }
            } else if (config->notificationSubsystem == "kdialog") {
                for (int i = 0; i < min(messages.size(), config->messagesInPopup); ++i) {
                    QString trayMessage = messages[i].username + ": " + messages[i].text + " /" + QwitTools::formatDateTime(messages[i].time.toLocalTime());
                    QStringList args;
                    args << "--title" << title << "--passivepopup" << trayMessage << "30";
                    QProcess::execute("kdialog", args);
                }
            }
        }
	}
}

void MainWindow::redrawPages() {
	for (int i = 0; i < pages.size(); ++i) {
		pages[i]->redraw();
	}
}

void MainWindow::updateAccount(Account *account, bool initial) {
	qDebug() << "MainWindow::updateAccount()";
	for (int i = 0; i < pages.size(); ++i) {
		if (pages[i]->updateAutomatically()) {
			pages[i]->update(account, initial);
		}
	}
	account->updateLastMessage();
}

void MainWindow::updateAll(bool initial) {
	Configuration *config = Configuration::getInstance();
	for (int i = 0; i < config->accounts.size(); ++i) {
		updateAccount(config->accounts[i], initial);
	}
}

void MainWindow::updateRemainingRequests(int remainingRequests, Account *account) {
	if (remainingRequests == -1) {
		stateLabel->setText("");
	} else if (remainingRequests == 0) {
        stateLabel->setText(tr("Rate limit exceeded"));
	} else {
        stateLabel->setText(tr("%n requests left", "", remainingRequests));
	}
}

void MainWindow::directMessage(const Message &message) {
	Configuration *config = Configuration::getInstance();
	directMessageDialog->accountsComboBox->clear();
	for (int i = 0; i < config->accounts.size(); ++i) {
		directMessageDialog->accountsComboBox->addItem(QIcon(":/images/" + config->accounts[i]->type + ".png"), config->accounts[i]->username);
	}
	directMessageDialog->accountsComboBox->setCurrentIndex(message.account->id);
	directMessageDialog->usernameLineEdit->setText(message.username);
	directMessageDialog->messageTextEdit->clear();
	directMessageDialog->messageTextEdit->setFocus();
	directMessageDialog->showNormal();
}

void MainWindow::sendDirectMessage() {
	Configuration *config = Configuration::getInstance();
	config->accounts[directMessageDialog->accountsComboBox->currentIndex()]->sendDirectMessage(directMessageDialog->usernameLineEdit->text(), directMessageDialog->messageTextEdit->toPlainText());
}

void MainWindow::favor(const Message &message) {
	message.account->favorMessage(message);
}

void MainWindow::unfavor(const Message &message) {
	message.account->unfavorMessage(message);
}

void MainWindow::destroy(const Message &message) {
	message.account->destroyMessage(message);
}

void MainWindow::postTwitPic() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Pick an image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.jpeg *.bmp *.PNG *.JPG *.JPEG *.BMP)"));
	if (fileName.isEmpty()) {
		return;
	}
    TwitPicDialog dialog(fileName, this);
//	dialog.setUser(config->currentAccount()->username, config->currentAccount()->password);
	if (dialog.exec() == QDialog::Accepted) {
		messageTextEdit->insertPlainText(dialog.twitPickedUrlString());
		messageTextEdit->insertPlainText(" " + dialog.twitPicCommentString());
	}
}

void MainWindow::ensureThereAreAccounts() {
	Configuration *config = Configuration::getInstance();
	if (config->accounts.size() == 0) {
		QMessageBox::warning(this, "Warning!", "You must configure at least one account before you can use Qwit!");
		mainTabWidget->setEnabled(false);
		refreshToolButton->setEnabled(false);
		messageTextEdit->setEnabled(false);
		twitPicButton->setEnabled(false);
		lastMessageLabel->setText("");
		stateLabel->setText("No accounts configured");
	} else {
		mainTabWidget->setEnabled(true);
		refreshToolButton->setEnabled(true);
		messageTextEdit->setEnabled(true);
		twitPicButton->setEnabled(true);
	}
}

void MainWindow::updateUrlShorteningButtonTooltip(bool enabled) {
	if (enabled)
		urlShorteningEnabledButton->setToolTip(tr("Disable shortening of pasted links"));
	else
		urlShorteningEnabledButton->setToolTip(tr("Enable shortening of pasted links"));
}

#endif
