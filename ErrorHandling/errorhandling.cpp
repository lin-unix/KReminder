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
	QWidget *parent;
	bool close;
};

ErrorHandling::ErrorHandling(QWidget *parent) : d(new ErrorHandlingPrivate) {
    d->parent = parent;
}

//TODO: Save all errors to log file
//TODO: Let Dr. Konqi report errors and give it the log file
bool ErrorHandling::handleError(ErrorHandling::errorNumber error, bool endProgram, QFile::FileError fileError, QTextStream::Status textStreamError)
{
    KDialog *errorDialog = new KDialog(0);

    errorDialog->setButtons(KDialog::User1 | KDialog::User2);
    errorDialog->setDefaultButton(KDialog::User1);
	errorDialog->setButtonGuiItem(KDialog::User2, KGuiItem(QString(i18n("Quit")), KIcon("application-exit", KIconLoader::global()), QString(i18n("Do not report error")), QString(i18n("Quit KReminder"))));
	errorDialog->setButtonGuiItem(KDialog::User1, KGuiItem(QString(i18n("Report Error")), KIcon("tools-report-bug", KIconLoader::global()), QString(i18n("Launch wizard to report error")), QString(i18n("Open Dr. Konqi to report an error"))));

    errorDialog->setMainWidget(new QLabel(i18n("Cannot save your reminder.\n\nWould you like to report this error?")));
    errorDialog->setCaption(i18n("Internal Error"));

    if(!connect(errorDialog, SIGNAL(user2Clicked()), errorDialog, SLOT(reject()))) {
		// Log problem with connecting signal with slot
		return true;
	}

    switch(error) {
        case windowClose: {
			if(errorDialog->exec()) // When using exec(), window is modal
				return true; // If exec() returns 0 (which is a rejection to run Dr. Konqi because the user clicked "Quit"), then end the program
			else {
				return false;
				//Add code here to run Dr. Konqi
			}

            break;
        }
        case fcrontabFileOpen: {
            break;
        }
        case writeReminderToFile: {
            break;
        }
        case fileDelete: {
            break;
        }
        case processCrashed: {
            break;
        }
        case processNotStarted: {
            break;
        }
        case fcronError: {
            break;
        }
        case systemFunction: {
            break;
        }
        case adminDenyFile: {
            break; //Ask for root access
        }
        case adminAllowFile: {
			QVariantMap args;
			Action writeAllow("org.kde.auth.kreminder.rwallow");

			writeAllow.setHelperID("org.kde.auth.kreminder");
			args["filename"] = "/usr/local/etc/fcron.allow";
			writeAllow.setArguments(args);

			ActionReply reply = writeAllow.execute();

			if(reply.failed()) {
				if(reply.type() == ActionReply::KAuthError) {
					; //Internal KAuth error
				}
				else if(reply.type() == ActionReply::HelperError) {
					; //Self generated error code provided
				}

				return true; //Terminate program
			}

			return false;
            break;
        }
        case denyFileOpen: {
            break;
        }
        case allowFileOpen: {
            break;
        }
        case inputDenyRead: {
            break;
        }
        case inputAllowRead: {
            break;
        }
        default: {
            return true; //why would this be run? Save to log if this happens
        }
    }

    return true;
}

ErrorHandling::~ErrorHandling() {
    delete d;
}
