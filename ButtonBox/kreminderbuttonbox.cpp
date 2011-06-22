/*
 *  KReminder - A Replacement Of Your Short-Term Memory
 *  Copyright (C) 2011  Steven Sroka
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kreminderbuttonbox.h"
#include "Reminder/newreminderwindow.h"
#include "Introduction/introwindow.h"

#include <KDE/KLocale>
#include <KDE/KGuiItem>
#include <KDE/KPushButton>

#include <QtCore/QString>
#include <QtGui/QPushButton>

class KReminderButtonBoxPrivate
{

public:
    KPushButton *nextButton;
    KPushButton *exitButton;
    KPushButton *closeButton;
    KPushButton *okButton;
    QPushButton *mainMenuButton;

    int optionSelected;
    QString *name;
};

KReminderButtonBox::KReminderButtonBox(QWidget *parent) : KDialogButtonBox(parent), d(new KReminderButtonBoxPrivate)
{
    d->name = new QString(parent->metaObject()->className());
    d->optionSelected = 0;
    setupObjects();
}

void KReminderButtonBox::setupObjects()
{
    if (d->name->compare("IntroWindow", Qt::CaseInsensitive) == 0) {
        d->nextButton = addButton(KStandardGuiItem::forward(), AcceptRole, this, SLOT(next()));
        d->closeButton = addButton(KStandardGuiItem::close(), AcceptRole, parentWidget(), SLOT(close()));

        setAddReminderToolTip(true);
    } else if (d->name->compare("NewReminderWindow", Qt::CaseInsensitive) == 0) {
        d->okButton = addButton(KStandardGuiItem::ok(), AcceptRole, this, SLOT(saveReminder()));
        d->mainMenuButton = addButton("Main Menu", AcceptRole, this, SLOT(sendToMenu()));
        d->closeButton = addButton(KStandardGuiItem::close(), AcceptRole, parentWidget(), SLOT(close()));
    }
}

void KReminderButtonBox::setAddReminderToolTip(bool checked)
{
    if (checked) {
        d->nextButton->setToolTip(i18n("Create yourself a new reminder"));
        d->nextButton->setWhatsThis(i18n("Choose this if you want to setup a new reminder"));

        d->optionSelected = 0;
    }
}

void KReminderButtonBox::setAddNoteToolTip(bool checked)
{
    if (checked) {
        d->nextButton->setToolTip(i18n("Pencil in a new note"));
        d->nextButton->setWhatsThis(i18n("Choose this if you want to create a new note"));

        d->optionSelected = 1;
    }
}

void KReminderButtonBox::setConfigureToolTip(bool checked)
{
    if (checked) {
        d->nextButton->setToolTip(i18n("Configure KReminder"));
        d->nextButton->setWhatsThis(i18n("Change the overall options of KReminder"));

        d->optionSelected = 2;
    }
}

KReminderButtonBox::~KReminderButtonBox()
{
    delete d;
}

void KReminderButtonBox::next()
{
    if (d->optionSelected == 0) {
        NewReminderWindow *optionWindow = new NewReminderWindow(0);
        optionWindow->show();
    } else if (d->optionSelected == 1) {
        //NewNote *optionWindow = new NewNote(parentWidget());
        //optionWindow->show();
    } else if (d->optionSelected == 2) {
        //ConfigureKReminder *optionWindow = new ConfigureKReminder(parentWidget());
        //optionWindow->show();
    } else {
        ; //Error
    }

    window()->close();
}

void KReminderButtonBox::sendToMenu()
{
    //Send the user back to the main menu
    IntroWindow *mainMenu = new IntroWindow(0);

	mainMenu->show();
    window()->close();
}

void KReminderButtonBox::saveReminder()
{
	//Save information and start the reminder daemon
	window()->close();
}