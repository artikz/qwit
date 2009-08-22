/*!
 *  $Id: AboutDialog.cpp 188 2009-07-11 16:46:38Z artem.iglikov $
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
 *  AboutDialog class implementation
 *
 */

#ifndef AboutDialog_cpp
#define AboutDialog_cpp

#include "QwitHeaders.h"

#include "AboutDialog.h"

#ifndef VERSION
#define VERSION "UNKNOWN"
#endif

#ifndef REVISION
#define REVISION "UNKNOWN"
#endif

AboutDialog::AboutDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);
	QString version = VERSION;
	QString revision = REVISION;
	QString currentNumber = "", lastNumber = "";
	for (int i = 0; i < revision.length(); ++i) {
		if (!revision[i].isDigit()) {
			if (currentNumber != "") {
				lastNumber = currentNumber;
				currentNumber = "";
			}
		} else {
			currentNumber += revision[i];
		}
	}
	if (currentNumber != "") {
		lastNumber = currentNumber;
	}
	if (lastNumber != "") {
		revision = lastNumber;
	}
	applicationTitleLabel->setText("Qwit - " + version + " (revision " + revision + ")");
}

#endif
