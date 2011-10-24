/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

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
		fcrontabFileOpen = 2,
		writeReminderToFile = 3,
		fileDelete = 4,
		processCrashed = 5,
		processNotStarted = 6,
		fcronError = 7,
		systemFunction = 8,
		adminDenyFile = 9,
		adminAllowFile = 10,
		denyFileOpen = 11,
		allowFileOpen = 12,
		inputDenyRead = 13,
		inputAllowRead = 14,
		writeAllowFile = 15,
		writeDenyFile = 16
	};

	void handleError(errorNumber error = noError, QFile::FileError fileError = QFile::NoError, QTextStream::Status textStreamError = QTextStream::Ok);

private:
    ErrorHandlingPrivate *const d;
};

#endif // ERRORHANDLING_H
