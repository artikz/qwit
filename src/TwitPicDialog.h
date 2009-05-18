/*  This file is part of Qwit.

    Copyright (C) 2009 Roopesh Chander <roop@forwardbias.in>

    Qwit is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Qwit is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Qwit.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef TWITPICDIALOG_H
#define TWITPICDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>

class TwitPicDialog : public QDialog
{
public:
	TwitPicDialog(const QString &fileName, QWidget *parent = 0);
	void setTwitPixmap(const QPixmap &pixmap);
	QProgressBar *progressBar() const;
	void setUser(const QString &username, const QString &password);

public slots:
	void accept();

private:
	QString m_fileName;
	QLabel *m_pixmapLabel;
	QPushButton *m_postButton, *m_cancelButton;
	QProgressBar *m_progressBar;
};

#endif // TWITPICDIALOG_H
