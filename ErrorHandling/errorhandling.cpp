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


#include "errorhandling.h"

#include <KDE/KDialog>
#include <KDE/KLocale>

#include <QLabel>

class ErrorHandlingPrivate
{
public:
	QWidget *parent;
};

ErrorHandling::ErrorHandling(QWidget *parent) : d(new ErrorHandlingPrivate) {
    d->parent = parent;
}

//TODO: Save all errors to log file
//TODO: Let Dr.Konqi report errors and give it the log file
void ErrorHandling::handleError(ErrorHandling::errorNumber error, QFile::FileError fileError, QTextStream::Status textStreamError)
{
    KDialog *errorDialog = new KDialog(0);

    errorDialog->setButtons(KDialog::User1 | KDialog::User2);
    errorDialog->setDefaultButton(KDialog::User1);
    errorDialog->setButtonGuiItem(KDialog::User2, KGuiItem(QString("Quit"), KIcon("application-exit", KIconLoader::global()), QString("Tooltip"), QString("What's this")));
    errorDialog->setButtonGuiItem(KDialog::User1, KGuiItem(QString("Report Error"), KIcon("tools-report-bug", KIconLoader::global()), QString("Tooltip"), QString("What's this")));

    errorDialog->setMainWidget(new QLabel(i18n("Cannot save your reminder.\n\nWould you like to report this error?")));
    errorDialog->setCaption(i18n("Internal Error"));
    errorDialog->setModal(true);

    connect(errorDialog, SIGNAL(user2Clicked()), this, SLOT(close()));

    switch (error) {
        case windowClose: {
			errorDialog->show();
            d->parent->window()->hide();

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
			KAuth::Action writeAllow("org.kde.auth.kreminder.rwallow");

			writeAllow.setHelperID("org.kde.auth.kreminder");
			args["filename"] = "/usr/local/etc/fcron.allow";
			writeAllow.setArguments(args);

			KAuth::ActionReply reply = writeAllow.execute();

			if(reply.failed()) {
				if(reply.type() == ActionReply::KAuthError) {
					; //Could not complete task
				}
			}

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
            ; //why would this be run? Save to log if this happens
        }
    }
}

ErrorHandling::~ErrorHandling() {
    delete d;
}
