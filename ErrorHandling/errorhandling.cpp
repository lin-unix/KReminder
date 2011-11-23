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

#include "errorhandling.h"

#include <KDE/KDialog>
#include <KDE/KLocale>
#include <KDE/KApplication>
#include <KDE/KUser>

#include <QtGui/QLabel>
#include <QtCore/QFile>

class ErrorHandlingPrivate
{
public:
	KDialog *errorDialog;
	QWidget *parent;

	ErrorHandling::errorNumber error;
	QTextStream::Status textStreamError;
	QFile::FileError fileError;
	QFile::FileError originalFileError;
	QFile::FileError newFileError;
	bool endProgram;
	bool isStringNull;
	bool fileRemovalError;
	bool connectError;
};

ErrorHandling::ErrorHandling(QWidget *parent) : d(new ErrorHandlingPrivate) {
	d->errorDialog = new KDialog(0);

	d->errorDialog->setButtons(KDialog::User2 | KDialog::User1);
	d->errorDialog->setDefaultButton(KDialog::User1);
	d->errorDialog->setButtonGuiItem(KDialog::User1, KGuiItem(QString(i18n("Report Error")), KIcon("tools-report-bug", KIconLoader::global()), QString(i18n("Launch wizard to report error")), QString(i18n("Open Dr. Konqi to report an error"))));
	d->errorDialog->setButtonGuiItem(KDialog::User2, KGuiItem(QString(i18n("Quit")), KIcon("application-exit", KIconLoader::global()), QString(i18n("Do not report error")), QString(i18n("Quit KReminder"))));

	d->errorDialog->setMainWidget(new QLabel(i18n("Cannot save your reminder.\nWould you like to report this error?")));
	d->errorDialog->setCaption(i18n("Internal Error"));

	if(!connect(d->errorDialog, SIGNAL(user2Clicked()), d->errorDialog, SLOT(reject()))) {
		handleConnectError();
	}

	d->parent = parent;
}

//TODO: Save all errors to log file
//TODO: Let Dr. Konqi report errors and give it the log file
void ErrorHandling::handleError(ErrorHandling::errorNumber error, bool endProgram, QFile::FileError fileError, QTextStream::Status textStreamError, bool isStringNull)
{
	setErrors(error, endProgram, fileError, textStreamError, isStringNull);

	if(error != ErrorHandling::userDirectoryNotExistent) {
		writeToLog();
	}

    switch(error) {
        case windowClose: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case fcrontabFileOpen: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case writeReminderToFile: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case fileDelete: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case processCrashed: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case processNotStarted: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case fcronError: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case systemFunction: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case denyFileOpen: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case allowFileOpen: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case inputDenyRead: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case inputAllowRead: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        default: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Why would this be run? Save to log if this happens
				//Add code here to run Dr. Konqi, then exit program
			}
        }
    }
}

void ErrorHandling::handleKAuthError(ErrorHandling::errorNumber error, bool endProgram, QFile::FileError originalFileError, QFile::FileError newFileError, QTextStream::Status textStreamError, bool isStringNull, bool fileRemovalError)
{
	setKAuthErrors(error, endProgram, originalFileError, newFileError, textStreamError, isStringNull, fileRemovalError);
	writeToLog();

	switch((int)error) {
		case kauthintneralerror: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

			break;
		}
		case kauthcustomerror: {
			if(d->errorDialog->exec()) {
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

			break;
		}
		default: {
			if(d->errorDialog->exec()) { // When using exec(), window is modal
				exit(EXIT_FAILURE); // If exec() returns 0 (which is a rejection to run Dr. Konqi because the user clicked "Quit"), then end the program
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}
		}
	}
}

void ErrorHandling::handleConnectError()
{
	d->connectError = true;
	writeToLog();

	exit(EXIT_FAILURE);
}

void ErrorHandling::setErrors(ErrorHandling::errorNumber error, bool endProgram, QFile::FileError fileError, QTextStream::Status textStreamError, bool isStringNull)
{
	d->error = error;
	d->endProgram = endProgram;
	d->fileError = fileError;
	d->textStreamError = textStreamError;
	d->isStringNull = isStringNull;
}

void ErrorHandling::setKAuthErrors(ErrorHandling::errorNumber error, bool endProgram, QFile::FileError originalFileError, QFile::FileError newFileError, QTextStream::Status textStreamError, bool isStringNull, bool fileRemovalError)
{
	d->error = error;
	d->endProgram = endProgram;
	d->originalFileError = originalFileError;
	d->newFileError = newFileError;
	d->textStreamError = textStreamError;
	d->isStringNull = isStringNull;
	d->fileRemovalError = fileRemovalError;
}

void ErrorHandling::writeToLog()
{
	KUser currentUser;
	QFile logFile(currentUser.homeDir() + "/.KReminder.log");

	if(logFile.exists()) {
		if(!logFile.open(QIODevice::Append | QIODevice::Text)) {
			exit(EXIT_FAILURE);
		}
	}
	else {
		if(!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			exit(EXIT_FAILURE);
		}
	}

	QString errors("#####\n");
	errors.append("Error Number: " + QString::number((int)d->error) + "\n");

	if(d->endProgram) {
		errors.append("End Program: true\n");
	}
	else {
		errors.append("End Program: false\n");
	}

	errors.append("File Error: " + QString::number((int)d->fileError) + "\n");
	errors.append("Original File Error: " + QString::number((int)d->originalFileError) + "\n");
	errors.append("New File Error: " + QString::number((int)d->newFileError) + "\n");
	errors.append("Text Stream Error: " + QString::number((int)d->textStreamError) + "\n");

	if(d->isStringNull) {
		errors.append("Is String Null: true\n");
	}
	else {
		errors.append("Is String Null: false\n");
	}

	if(d->fileRemovalError) {
		errors.append("File Removal Error: true\n");
	}
	else {
		errors.append("File Removal Error: false\n");
	}

	if(d->connectError) {
		errors.append("Connect Error: true\n");
	}

	errors.append("#####\n\n");

	logFile.write(errors.toLocal8Bit().constData());
	logFile.close();
}

ErrorHandling::~ErrorHandling() {
    delete d;
}
