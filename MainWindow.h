#ifndef MainWindow_h
#define MainWindow_h

#include <QDialog>
#include <QTimer>
#include <QMessageBox>
#include <QBuffer>
#include <QtNetwork/QHttp>
#include <QUrl>
#include <QtXml/QDomDocument>
#include <QString>
#include <QFile>
#include <QTime>
#include <QSystemTrayIcon>
#include <QTextStream>
#include <QMenu>
#include <QCloseEvent>
#include <QSettings>
#include <QScrollArea>
#include <QScrollBar>
#include <QQueue>
#include <QPair>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QDir>

#include "UserpicsDownloader.h"
#include "StatusLineEdit.h"
#include "TwitterWidget.h"
#include "Twitter.h"

#include "ui_mainwindow.h"

class MainWindow: public QDialog, public Ui::MainWindow {
	Q_OBJECT

public:
	
	QTimer *timer;
	QString username;
	QString password;
	int interval;
	QAction *quitAction;
	QMenu *trayIconMenu;
	QSystemTrayIcon *trayIcon;
	int lastId;
	bool acceptClose;
	bool useProxy;
	QString proxyUsername;
	QString proxyPassword;
	QString proxyAddress;
	int proxyPort;
	TwitterWidget *twitterWidget;
	QScrollArea *scrollArea;
	UserpicsDownloader userpicsDownloader;
	StatusLineEdit *statusLineEdit;
	Twitter twitter;
	
	MainWindow(QWidget *parent = 0);
	void setupTrayIcon();
	void loadSettings();
	
	QString formatDateTime(const QDateTime &time);

public slots:
	
	void sendStatus();
	void updateHome();
	void saveSettings();
	void resetSettings();
	void homeUpdated(const QByteArray &buffer);
	void statusUpdated();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void quit();

protected:
	
	void closeEvent(QCloseEvent *event);
	void hideEvent(QHideEvent *event);
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);
};

#endif
