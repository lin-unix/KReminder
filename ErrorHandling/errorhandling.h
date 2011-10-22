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
		windowClose = 0,
		fcrontabFileOpen = 1,
		writeReminderToFile = 2,
		fileDelete = 3,
		processCrashed = 4,
		processNotStarted = 5,
		fcronError = 6,
		systemFunction = 7,
		adminDenyFile = 8,
		adminAllowFile = 9,
		denyFileOpen = 10,
		allowFileOpen = 11,
		inputDenyRead = 12,
		inputAllowRead = 13,
		writeAllowFile = 14,
		writeDenyFile = 15
	};

	void handleError(errorNumber error, QFile::FileError fileError = QFile::NoError, QTextStream::Status textStreamError = QTextStream::Ok);

private:
    ErrorHandlingPrivate *const d;
};

#endif // ERRORHANDLING_H