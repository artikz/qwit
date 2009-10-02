/*! 
 *  $Id: UserpicsDownloader.h 186 2009-07-11 14:38:30Z artem.iglikov $
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
 *  UserpicsDownloader class declaration
 */

#ifndef UserpicsDownloader_h
#define UserpicsDownloader_h

#include "QwitHeaders.h"

class UserpicsDownloader: public QObject {
	Q_OBJECT
	
	QFile file;
	QString url;
	QHttp *http;
	int requestId;
	QQueue<QPair<QString, QString> > queue;
	QHash<QString, QString> userImageMap;
	static UserpicsDownloader* instance;
	UserpicsDownloader();
	
public:

	static UserpicsDownloader* getInstance();
	void startDownload();
	void download(const QString &url, const QString &filename);
	
	// user image file name: maps the username to her profile image stored in the local cache
	void setUserImageFileName(QString serviceBaseUrl, QString user, QString imageFileName, bool replace = true);
	QString userImageFileName(QString serviceBaseUrl, QString user) const;

public slots:

	void requestStarted(int id);
//	void requestFinished(int id, bool error);
	void httpDone(bool error);
	
signals:
	
//	void stateChanged(const QString &);
	void userpicDownloaded();
};

#endif
