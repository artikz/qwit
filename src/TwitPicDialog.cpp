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


#include "TwitPicDialog.h"
#include <QGridLayout>
#include <QProgressBar>
#include <QDialogButtonBox>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QNetworkRequest>
#include <QImageReader>

TwitPicDialog::TwitPicDialog(const QString &fileName, QWidget *parent)
		: QDialog(parent),
		m_fileName(fileName),
		m_pixmapLabel(0),
		m_postButton(0),
		m_progressBar(0)
{
	setWindowTitle("TwitPic");
	QVBoxLayout *layout = new QVBoxLayout;
	m_pixmapLabel = new QLabel(this);
	layout->addWidget(m_pixmapLabel);
	QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
	m_postButton = buttonBox->addButton("Post image", QDialogButtonBox::AcceptRole);
	m_cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
	layout->addWidget(buttonBox, 0, Qt::AlignCenter);
	m_progressBar = new QProgressBar(this);
	m_progressBar->setMinimum(0);
	m_progressBar->setMaximum(100);
	m_progressBar->setTextVisible(false);
	m_progressBar->hide();
	layout->addWidget(m_progressBar);
	setLayout(layout);
	m_postButton->setDefault(true);
	setTwitPixmap(QPixmap(m_fileName));
	connect(m_postButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void TwitPicDialog::setTwitPixmap(const QPixmap &pixmap) {
	QSize size(600, 200);
	if (pixmap.width() > size.width() || pixmap.height() > size.height()) {
		m_pixmapLabel->setPixmap(pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	} else {
		m_pixmapLabel->setPixmap(pixmap);
	}
}

void TwitPicDialog::accept() {
	m_progressBar->reset();
	m_progressBar->show();
}
