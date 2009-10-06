/*! 
 *  $Id: MainWindow.h 207 2009-07-24 18:05:05Z artem.iglikov $
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
 *  MainWindow class declaration
 */

#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"

#include "QwitHeaders.h"

#include "AbstractPage.h"
#include "MessageTextEdit.h"
#include "OptionsDialog.h"
#include "AboutDialog.h"
#include "DirectMessageDialog.h"
#include "HomePage.h"
#include "RepliesPage.h"
#include "PublicPage.h"
#include "FavoritesPage.h"
#include "InboxPage.h"
#include "OutboxPage.h"
#include "SearchPage.h"
#include "Message.h"

class MainWindow: public QDialog, public Ui::MainWindow {
	Q_OBJECT

private:

	OptionsDialog *optionsDialog;
	AboutDialog *aboutDialog;
	DirectMessageDialog *directMessageDialog;
	QVector<QToolButton*> accountsButtons;
	QHBoxLayout *accountsLayout;
	QButtonGroup accountsButtonGroup;
	QVector<AbstractPage*> pages;
	QSystemTrayIcon *trayIcon;
	QMenu *trayIconMenu;
	QAction *trayQuitAction;
	QAction *trayShowhideAction;
	HomePage *homePage;
	RepliesPage *repliesPage;
	PublicPage *publicPage;
	FavoritesPage *favoritesPage;
	InboxPage *inboxPage;
	OutboxPage *outboxPage;
	SearchPage *searchPage;
	QTimer *redrawTimer;
	QTimer *updateTimer;
	MessageTextEdit *messageTextEdit;
	QLabel *greetingMessageLabel;
	QLabel *leftCharactersNumberLabel;

	
	bool acceptClose;
	
	static MainWindow* instance;
	MainWindow(QWidget *parent = 0);
	
public:

	static MainWindow* getInstance();
	void addAccountButton(Account *account);
	void deleteAccountButton(Account *account);
	void updateAccountButton(Account *account);
	void setupTrayIcon();
	void updateCurrentAccount(int id);
	
public slots:

	void quit();
	void leftCharsNumberChanged(int length);
	void saveState();
	void loadState();
	void updateState();
	void saveOptions();
	void resetOptionsDialog();
	void showOptionsDialog();
	void accountButtonClicked(int id);
	void showhide();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void refresh();
	void tabChanged(int tabIndex);
	void reloadUserpics();
	void updateLastMessage(const QString &message, Account *account);
	void messageSent(const QString &message, Account *account);
	void messageNotSent(Account *account);
	void showNewMessages(const QVector<Message> &messages, Account *account);
	void redrawPages();
	void updateAccount(Account *account);
	void updateAll();
	void updateRemainingRequests(int remainingRequests, Account *account);
	void directMessage(const Message &message);
	void favor(const Message &message);
	void unfavor(const Message &message);
	void destroy(const Message &message);
	void sendDirectMessage();
	void postTwitPic();
	void ensureThereAreAccounts();

private slots:
	void updateUrlShorteningButtonTooltip(bool);

signals:

	void retweet(const Message &);
	void reply(const Message &);
	
protected:

	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *event);
};

#endif
