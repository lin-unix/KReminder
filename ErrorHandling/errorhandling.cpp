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

#include <QtGui/QLabel>

class ErrorHandlingPrivate
{
public:
	KDialog *errorDialog;
	QWidget *parent;
	bool close;
};

ErrorHandling::ErrorHandling(QWidget *parent) : d(new ErrorHandlingPrivate) {
	d->errorDialog = new KDialog(0);

	d->errorDialog->setButtons(KDialog::User1 | KDialog::User2);
	d->errorDialog->setDefaultButton(KDialog::User1);
	d->errorDialog->setButtonGuiItem(KDialog::User2, KGuiItem(QString(i18n("Quit")), KIcon("application-exit", KIconLoader::global()), QString(i18n("Do not report error")), QString(i18n("Quit KReminder"))));
	d->errorDialog->setButtonGuiItem(KDialog::User1, KGuiItem(QString(i18n("Report Error")), KIcon("tools-report-bug", KIconLoader::global()), QString(i18n("Launch wizard to report error")), QString(i18n("Open Dr. Konqi to report an error"))));

	d->errorDialog->setMainWidget(new QLabel(i18n("Cannot save your reminder.\n\nWould you like to report this error?")));
	d->errorDialog->setCaption(i18n("Internal Error"));

	d->parent = parent;
}

//TODO: Save all errors to log file
//TODO: Let Dr. Konqi report errors and give it the log file
void ErrorHandling::handleError(ErrorHandling::errorNumber error, bool endProgram, QFile::FileError fileError, QTextStream::Status textStreamError, bool isStringNull)
{
	if(!connect(d->errorDialog, SIGNAL(user2Clicked()), d->errorDialog, SLOT(reject()))) {
		//Log to file
		//Add code here to run Dr. Konqi
		exit(EXIT_FAILURE);
	}

    switch(error) {
        case windowClose: {
			if(d->errorDialog->exec()) {
				//Log to file
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case fcrontabFileOpen: {
			if(d->errorDialog->exec()) {
				//Log to file
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

            break;
        }
        case writeReminderToFile: {
			if(d->errorDialog->exec()) {
				//Log to file
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
	switch(error) {
		case kauthintneralerror: {
			if(d->errorDialog->exec()) {
				//Log to file
				exit(EXIT_FAILURE);
			}
			else {
				//Add code here to run Dr. Konqi, then exit program
			}

			break;
		}
		case kauthcustomerror: {
			if(d->errorDialog->exec()) {
				//Log to file
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
				//Why would this be run? Save to log if this happens
				//Add code here to run Dr. Konqi, then exit program
			}
		}
	}
}

ErrorHandling::~ErrorHandling() {
    delete d;
}
