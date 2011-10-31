/*
    KReminder - A Replacement Of Your Short-Term Memory
    Copyright (C) 2011  Steven Sroka

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

#include <kauth.h>

#include <QtGui/QWidget>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QObject>

using namespace KAuth;

class ErrorHandlingPrivate;

class ErrorHandling : public QObject
{
  Q_OBJECT

public:
    ErrorHandling(QWidget *parent = 0);
    virtual ~ErrorHandling();

    enum errorNumber {
		noError = 0,
		windowClose = 1,
		windowCloseQuit = 2,
		selectedOption = 3,
		fcrontabFileOpen = 4,
		writeReminderToFile = 5,
		fileDelete = 6,
		processCrashed = 7,
		processNotStarted = 8,
		fcronError = 9,
		systemFunction = 10,
		adminDenyFile = 11,
		adminAllowFile = 12,
		denyFileOpen = 13,
		allowFileOpen = 14,
		inputDenyRead = 15,
		inputAllowRead = 16,
		writeAllowFile = 17,
		writeDenyFile = 18,
	};

	bool handleError(errorNumber error = noError, bool endProgram = true, QFile::FileError fileError = QFile::NoError, QTextStream::Status textStreamError = QTextStream::Ok);

private:
    ErrorHandlingPrivate *const d;
};

#endif //ERRORHANDLING_H
