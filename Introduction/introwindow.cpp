/*
 *  <one line to give the program's name and a brief idea of what it does.>
 *  Copyright (C) <year>  <name of author>
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

#include "introwindow.h"
#include "Reminder/newreminderwindow.h"

#include <KDE/KLocale>
#include <KDE/KSeparator>
#include <KDE/KIconLoader>
#include <KDE/KDialogButtonBox>
#include <KDE/KStandardGuiItem>
#include <KDE/KPushButton>

#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QDesktopWidget>

class IntroWindowPrivate
{
public:
    KPushButton *nextButton;
    KPushButton *closeButton;
    
    int optionSelected;
};

IntroWindow::IntroWindow(QWidget *parent) : KXmlGuiWindow(parent), d(new IntroWindowPrivate)
{
    KIconLoader *loader = KIconLoader::global();
    setWindowIcon(loader->loadIcon("view-calendar-tasks", KIconLoader::Small));

    setupObjects();
    setupGUI(Keys | Save | Create, "KReminderui.rc");

    adjustSize();

    QRect frameGeo = frameGeometry();
    frameGeo.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frameGeo.topLeft());
}

void IntroWindow::setupObjects()
{
    QWidget *mainWidget = new QWidget();

    QRadioButton *addReminderRadio = new QRadioButton(i18n("Add New Reminder"));
    QRadioButton *addNoteRadio = new QRadioButton(i18n("Add New Note"));

    addReminderRadio->setChecked(true);

    KDialogButtonBox *buttonBoxWidget = new KDialogButtonBox(this);
    KSeparator *hSeparator = new KSeparator();

    QVBoxLayout *vWindowLayout = new QVBoxLayout;
    QHBoxLayout *hMainWidgetsLayout = new QHBoxLayout;
    QVBoxLayout *vButtonGroupLayout = new QVBoxLayout;
    QVBoxLayout *vPixmapLayout = new QVBoxLayout;

    d->nextButton = buttonBoxWidget->addButton(KStandardGuiItem::forward(), KDialogButtonBox::AcceptRole, this, SLOT(next()));
    d->closeButton = buttonBoxWidget->addButton(KStandardGuiItem::close(), KDialogButtonBox::AcceptRole, parentWidget(), SLOT(close()));
    
    setAddReminderToolTip(true);
    
    vWindowLayout->insertSpacing(0, 5);
    vButtonGroupLayout->addWidget(addReminderRadio);
    vButtonGroupLayout->addWidget(addNoteRadio);

    hMainWidgetsLayout->insertStretch(0, 1);
    hMainWidgetsLayout->addLayout(vButtonGroupLayout);

    KIconLoader *loader = KIconLoader::global();
    QLabel *pictureLabel = new QLabel();

    pictureLabel->setPixmap(loader->loadIcon("help-hint", KIconLoader::Desktop));

    vPixmapLayout->setAlignment(Qt::AlignHCenter);
    vPixmapLayout->addWidget(pictureLabel);
    vPixmapLayout->insertStretch(1, 1);
    hMainWidgetsLayout->insertStretch(3, 1);
    hMainWidgetsLayout->addLayout(vPixmapLayout);
    hMainWidgetsLayout->insertStretch(4, 1);
    vWindowLayout->addLayout(hMainWidgetsLayout);

    vWindowLayout->insertSpacing(2, 5);
    vWindowLayout->addWidget(hSeparator);
    vWindowLayout->addWidget(buttonBoxWidget);

    connect(addReminderRadio, SIGNAL(clicked(bool)), this, SLOT(setAddReminderToolTip(bool)));
    connect(addNoteRadio, SIGNAL(clicked(bool)), this, SLOT(setAddNoteToolTip(bool)));
    
    connect(d->closeButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    mainWidget->setLayout(vWindowLayout);
    setCentralWidget(mainWidget);
}

IntroWindow::~IntroWindow()
{
    delete d;
}

void IntroWindow::setAddReminderToolTip(bool checked)
{
    if (checked) {
        d->nextButton->setToolTip(i18n("Create a new reminder"));
        d->nextButton->setWhatsThis(i18n("Choose this if you want to setup a new reminder"));
	
	d->optionSelected = 0;
    }
}

void IntroWindow::setAddNoteToolTip(bool checked)
{
    if (checked) {
        d->nextButton->setToolTip(i18n("Pencil in a new note"));
        d->nextButton->setWhatsThis(i18n("Choose this if you want to create a new note"));
	
	d->optionSelected = 1;
    }
}

void IntroWindow::next()
{
    if (d->optionSelected == 0) {
        NewReminderWindow *optionWindow = new NewReminderWindow(0);
        optionWindow->show();
    } else if (d->optionSelected == 1) {
        //NewNote *optionWindow = new NewNote(parentWidget());
        //optionWindow->show();
    } else {
        ; //Error
    }

    window()->close();
}
