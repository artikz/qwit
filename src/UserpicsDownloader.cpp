/*!
 *  $Id: UserpicsDownloader.cpp 186 2009-07-11 14:38:30Z artem.iglikov $
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
 *  UserpicsDownloader class implementation
 */

#ifndef UserpicsDownloader_cpp
#define UserpicsDownloader_cpp

#include "QwitHeaders.h"

#include "Configuration.h"
#include "UserpicsDownloader.h"

UserpicsDownloader* UserpicsDownloader::instance = 0;

UserpicsDownloader* UserpicsDownloader::getInstance() {
	if (!instance) {
		instance = new UserpicsDownloader();
	}
	return instance;
}

UserpicsDownloader::UserpicsDownloader() {
    http = new QHttp(this);
    connect(http, SIGNAL(requestStarted(int)), this, SLOT(requestStarted(int)));
//	connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)));
	connect(http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
	requestId = -1;
    cleanCache();
}

void UserpicsDownloader::startDownload() {
	if ((queue.size() == 0) || (requestId != -1)) return;
	Configuration *config = Configuration::getInstance();
    if (config->useProxy) {
        http->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, config->proxyAddress, config->proxyPort, config->proxyUsername, config->proxyPassword));
    } else {
        http->setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
    }
    while (queue.size() > 0) {
		QPair<QString, QString> item = queue.dequeue();
		QFileInfo fi = QFileInfo(item.second);
		if (fi.exists() && (fi.size() > 0)) {
			qDebug() << "UserpicsDownloader::startDownload() already downloaded " << item.second;
			continue;
		}
		file.setFileName(item.second);
		if (!file.open(QIODevice::WriteOnly)) {
			qDebug() << "UserpicsDownloader::startDownload() error opening file " << item.second;
			continue;
		}
		qDebug() << "UserpicsDownloader::startDownload() " << item.first << " -> " << item.second;
		QUrl url(item.first);
        http->setHost(url.host(), url.port(80));
		requestId = http->get(QUrl::toPercentEncoding(url.path(), "/"), &file);
		this->url = item.first;
		break;
	}
}

void UserpicsDownloader::cleanCache() {
    Configuration *config = Configuration::getInstance();
    QDir cacheDir(Configuration::CacheDirectory);
    QStringList files = cacheDir.entryList(QDir::Files);
    QVector<QPair<quint32, QString> > v;
    quint64 size = 0;
    for (QStringList::iterator it = files.begin(); it != files.end(); ++it) {
        if (*it != "messages") {
            QFileInfo fi(Configuration::CacheDirectory + *it);
            v.push_back(qMakePair(fi.lastModified().toTime_t(), *it));
            size += fi.size();
        }
    }

    if (size >> 20 >= config->cacheSize) {
        qSort(files.begin(), files.end());
        while (v.size() && (size >> 20 >= config->cacheSize)) {
            QFile f(Configuration::CacheDirectory + v[0].second);
            quint64 fileSize = f.size();
            if (f.remove()) {
                size -= fileSize;
            }
            v.remove(0);
        }
    }
}

void UserpicsDownloader::download(const QString &imageUrl, const QString &filename) {
	if (imageUrl == "") return;
	QFileInfo fi = QFileInfo(filename);
	if (!fi.exists() || (fi.size() == 0)) {
		qDebug() << "UserpicsDownloader::download() " << imageUrl << " " << filename;
		queue.enqueue(qMakePair(imageUrl, filename));
    } else {
        QFile f(filename);
        f.open(QIODevice::ReadWrite);
        f.close();
    }
	startDownload();
}

void UserpicsDownloader::httpDone(bool error) {
	file.close();
	if (error) {
        qWarning() << "UserpicsDownloader::httpDone() error " + url + " -> " + file.fileName() << ", " << qPrintable(http->errorString());
	} else {
        cleanCache();
		qDebug() << "UserpicsDownloader::httpDone() " + url + " -> " + file.fileName();
		emit userpicDownloaded();
	}
	requestId = -1;
	startDownload();
}

void UserpicsDownloader::requestStarted(int id) {
	if (id == requestId) {
		qDebug() << "UserpicsDownloader::requestStarted() " + QString::number(id) + " " + url + " -> " + file.fileName();
	}
}

void UserpicsDownloader::setUserImageFileName(QString serviceBaseUrl, QString user, QString imageFileName, bool replace) {
	if (!replace && userImageMap.contains(serviceBaseUrl + " " + user)) {
		return;
	}
	userImageMap[serviceBaseUrl + " " + user] = imageFileName;
}

QString UserpicsDownloader::userImageFileName(QString serviceBaseUrl, QString user) const {
	return userImageMap[serviceBaseUrl + " " + user];
}

#endif
