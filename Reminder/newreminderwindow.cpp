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

#include "newreminderwindow.h"
#include "Introduction/introwindow.h"

#include <KDE/KDialogButtonBox>
#include <KDE/KLocale>
#include <KDE/KPushButton>
#include <KDE/KIconLoader>
#include <KDE/KGuiItem>
#include <KDE/KSeparator>
#include <KDE/KRichTextWidget>
#include <KDE/KActionCollection>
#include <KDE/KButtonGroup>
#include <KDE/KLineEdit>
#include <KDE/KUser>
#include <KDE/KProcess>
#include <KDE/KStandardGuiItem>

#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDesktopWidget>
#include <QtGui/QCalendarWidget>
#include <QtGui/QRadioButton>
#include <QtGui/QTimeEdit>

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QDate>

class NewReminderWindowPrivate
{

public:
    KRichTextWidget *descriptionWidget;
    QVBoxLayout *vWindowLayout;
    QWidget *mainWidget;
    QCalendarWidget *calendarWidget;
    QTimeEdit *timeEdit;

    QRadioButton *dayRadio;
    QRadioButton *weekRadio;
    QRadioButton *customRadio;

    bool dayChecked;
};

NewReminderWindow::NewReminderWindow(QWidget *parent) : KXmlGuiWindow(parent), d(new NewReminderWindowPrivate)
{
    setCaption(i18n("Create a new reminder"));
    d->dayChecked = false;

    setupObjects();
    setupGUI(Keys | Save | Create, "KReminderui.rc");

    QRect frameGeo = frameGeometry();
    frameGeo.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frameGeo.topLeft());
}

void NewReminderWindow::setupObjects()
{
    d->mainWidget = new QWidget();
    d->vWindowLayout = new QVBoxLayout();

    QVBoxLayout *vCalendarLayout = new QVBoxLayout();
    QVBoxLayout *vOptionsLayout = new QVBoxLayout();
    QVBoxLayout *vOptionsRadioLayout = new QVBoxLayout();
    QHBoxLayout *hMainWidgetLayout = new QHBoxLayout();
    QHBoxLayout *hTimeLayout = new QHBoxLayout();
    KLineEdit *reminderLineEdit = new KLineEdit(this);
    QLabel *descriptionLabel = new QLabel(i18n("Detailed Description (optional):"), this);

    KDialogButtonBox *buttonBoxWidget = new KDialogButtonBox(this);
    KSeparator *hHeaderSeparator = new KSeparator();
    KSeparator *hButtonBoxSeparator = new KSeparator();
    KSeparator *vDescriptionSeparator = new KSeparator();
    KSeparator *vMainWidgetSeparator = new KSeparator(Qt::Vertical);
    KButtonGroup *optionsButtonGroup = new KButtonGroup(this);

    d->calendarWidget = new QCalendarWidget(this);
    d->timeEdit = new QTimeEdit(QTime(12, 00), this);
    d->dayRadio = new QRadioButton(i18n("24 Hours"));
    d->weekRadio = new QRadioButton(i18n("1 Week"));
    d->customRadio = new QRadioButton(i18n("Custom"));

    d->customRadio->setFocus(Qt::ActiveWindowFocusReason);
    d->customRadio->setChecked(true);

    buttonBoxWidget->addButton(KStandardGuiItem::ok(), KDialogButtonBox::AcceptRole, this, SLOT(saveReminder()));
    buttonBoxWidget->addButton("Main Menu", KDialogButtonBox::AcceptRole, this, SLOT(sendToMenu()));
    buttonBoxWidget->addButton(KStandardGuiItem::close(), KDialogButtonBox::AcceptRole, parentWidget(), SLOT(close()));

    connect(d->dayRadio, SIGNAL(toggled(bool)), this, SLOT(changeDateTime(bool)));
    connect(d->weekRadio, SIGNAL(toggled(bool)), this, SLOT(changeDateTime(bool)));

    reminderLineEdit->setClickMessage(i18n("Enter A Reminder Here"));

    d->vWindowLayout->addWidget(reminderLineEdit);
    d->vWindowLayout->addWidget(hHeaderSeparator);

    d->timeEdit->setAlignment(Qt::AlignHCenter);
    d->timeEdit->setTime(QTime::currentTime());

    hTimeLayout->addSpacing(70);
    hTimeLayout->addWidget(d->timeEdit);
    hTimeLayout->addSpacing(70);

    vCalendarLayout->addWidget(d->calendarWidget);
    vCalendarLayout->addLayout(hTimeLayout);

    hMainWidgetLayout->addLayout(vCalendarLayout);
    hMainWidgetLayout->addWidget(vMainWidgetSeparator);

    vOptionsRadioLayout->addWidget(d->dayRadio);
    vOptionsRadioLayout->addWidget(d->weekRadio);
    vOptionsRadioLayout->addWidget(d->customRadio);

    optionsButtonGroup->setFlat(true);
    optionsButtonGroup->setLayout(vOptionsRadioLayout);

    vOptionsLayout->addWidget(optionsButtonGroup);
    hMainWidgetLayout->addLayout(vOptionsLayout);

    d->vWindowLayout->addLayout(hMainWidgetLayout);

    d->descriptionWidget = new KRichTextWidget(this);
    d->descriptionWidget->setRichTextSupport(KRichTextWidget::FullSupport);
    d->descriptionWidget->createActions(actionCollection());

    d->vWindowLayout->addWidget(vDescriptionSeparator);
    d->vWindowLayout->addWidget(descriptionLabel);
    d->vWindowLayout->addWidget(d->descriptionWidget);
    d->vWindowLayout->addWidget(hButtonBoxSeparator);
    d->vWindowLayout->addWidget(buttonBoxWidget);

    d->mainWidget->setLayout(d->vWindowLayout);
    setCentralWidget(d->mainWidget);
}

NewReminderWindow::~NewReminderWindow()
{
    delete d;
}

void NewReminderWindow::changeDateTime(bool checked)
{
    QDate newDate = QDate::currentDate();

    if (checked) {
        if ((d->dayRadio->isChecked()) && (!d->customRadio->isChecked()) && (!d->weekRadio->isChecked())) {
            //24hrs
            d->dayChecked = true;
            d->calendarWidget->setSelectedDate(newDate.addDays(1));
        } else if ((d->weekRadio->isChecked()) && (!d->customRadio->isChecked()) && (!d->dayRadio->isChecked())) {
            //1 week
            if (d->dayChecked)
				d->calendarWidget->setSelectedDate(newDate.addDays(6));
			else
				d->calendarWidget->setSelectedDate(newDate.addDays(7));

			d->dayChecked = false;
        }
        else {
            //Error
        }
    }
}

/*
 * Save information and start the reminder daemon
 */
void NewReminderWindow::saveReminder()
{
    KProcess *SystemCall = new KProcess(this);
    KUser currentUser;
    QFile denyFile("/usr/local/etc/fcron.deny"), allowFile("/usr/local/etc/fcron.allow"), fcrontabFile("/home/" + currentUser.loginName() + "/.KReminter_fcrontab");
    QTextStream inputDenyFile(&denyFile), inputAllowFile(&allowFile);
    QString line;
    bool userDeny = false;

    switch(SystemCall->execute(QString("fcrontab"), QStringList("-V"), -1)) {
      case -1: {
	//Process crashed
	  ;
      }
      case -2: {
	//Process could not be started
	  ;
      }
      case 1: {
	//Fcron error code
	  ;
      }
    }

    if(denyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      while(!inputDenyFile.atEnd()) {
	line = inputDenyFile.readLine();

	if(line.contains(currentUser.loginName(), Qt::CaseSensitive)) //if the current user is in this list
	  ; //error message - need admin rights to remove user from list
      }
    }
    else if(allowFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      while(!inputAllowFile.atEnd()) {
	line = inputAllowFile.readLine();

	if(line.contains(currentUser.loginName(), Qt::CaseSensitive))
	  userDeny = true;
      }

      if(!userDeny) //if the current user is *not* in this list
	; //error message - need admin rights to add user to list
    }

    switch(SystemCall->execute(QString("fcrontab"), QStringList("-l >> /home/" + currentUser.loginName() + "/.KReminter_fcrontab"), -1)) {
      case -1: {
	//Process crashed
	  ;
      }
      case -2: {
	//Process could not be started
	  ;
      }
      case 1: {
	//Fcron error code
	  ;
      }
      default: {
		fcrontabFile.open(QIODevice::WriteOnly | QIODevice::Text);
		fcrontabFile.write("Hello"); //save reminder here
      }
    }

    switch(SystemCall->execute(QString("fcrontab"), QStringList("/home/" + currentUser.loginName() + "/.KReminter_fcrontab"), -1)) {
      case -1: {
	//Process crashed
	  ;
      }
      case -2: {
	//Process could not be started
	  ;
      }
      case 1: {
	//Fcron error code
	  ;
      }
      default: {
	window()->close();
      }
    }
}

/*
 * Send the user back to the main menu
 */
void NewReminderWindow::sendToMenu()
{
    IntroWindow *mainMenu = new IntroWindow(0);

    mainMenu->show();
    window()->close();
}

