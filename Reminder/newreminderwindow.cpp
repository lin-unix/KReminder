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
#include "ErrorHandling/errorhandling.h"

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
#include <KDE/KMessageBox>

#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDesktopWidget>
#include <QtGui/QCalendarWidget>
#include <QtGui/QRadioButton>
#include <QtGui/QTimeEdit>

#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QDate>
#include <QtCore/QFileInfo>

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
	ErrorHandling *errorCall;
};

/*
 * Set up the window
 */
NewReminderWindow::NewReminderWindow(QWidget *parent) : KXmlGuiWindow(parent), d(new NewReminderWindowPrivate)
{
	setCaption(i18n("Create a new reminder"));
	d->dayChecked = false;
	d->errorCall = new ErrorHandling(this);

	setupObjects();
	setupGUI(Keys | Save | Create, "KReminderui.rc");

	QRect frameGeo = frameGeometry();
	frameGeo.moveCenter(QDesktopWidget().availableGeometry().center());
	move(frameGeo.topLeft());
}

/*
 * Set up the GUI
 */
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
	buttonBoxWidget->addButton(i18n("Main Menu"), KDialogButtonBox::AcceptRole, this, SLOT(sendToMenu()));
	buttonBoxWidget->addButton(KStandardGuiItem::close(), KDialogButtonBox::AcceptRole, this, SLOT(close()));

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

/*
 * Change the selected date on the calendar based on the radio button that the user selected
 */
void NewReminderWindow::changeDateTime(bool checked)
{
	QDate newDate = QDate::currentDate();

	if(checked) { //24 hours
		if((d->dayRadio->isChecked()) && (!d->customRadio->isChecked()) && (!d->weekRadio->isChecked())) {
			d->dayChecked = true;
			d->calendarWidget->setSelectedDate(newDate.addDays(1));
		}
		else { //1 week
			if((d->weekRadio->isChecked()) && (!d->customRadio->isChecked()) && (!d->dayRadio->isChecked())) {
				if(d->dayChecked) {
					d->calendarWidget->setSelectedDate(newDate.addDays(6));
				}
				else {
					d->calendarWidget->setSelectedDate(newDate.addDays(7));
				}

				d->dayChecked = false;
			}
		}
	}
}

/*
 * Save information in an fcrontab file, and tell fcrontab that a new task has been created
 */
void NewReminderWindow::saveReminder()
{
	KUser currentUser;
	QFile fcrontabFile("/home/" + currentUser.loginName() + "/.KReminter_fcrontab");
	QFile::FileError fileError = QFile::NoError;
	QString *cmdInput = new QString("fcrontab -l >> /home/" + currentUser.loginName() + "/.KReminter_fcrontab");
	KProcess *systemCall = new KProcess(this);

	switch(systemCall->execute(QString("fcrontab"), QStringList("-V"), -1)) {
		case -1: {
			d->errorCall->handleError(ErrorHandling::processCrashed, true); //Process crashed
		}
		case -2: {
			d->errorCall->handleError(ErrorHandling::processNotStarted, true); //Process could not be started
		}
		case 1: {
			d->errorCall->handleError(ErrorHandling::fcronError, true); //fcron error code
		}
	}

	//It is assumed that the function completes successfuly because if there is an error,
	//the ErrorHandling class will be called internally
	checkUserPermissions();

	//Tell fcrontab to pipe the user's current fcrontab file into a temporary file
	switch(system(cmdInput->toLocal8Bit().constData())) {
		case -1: {
			d->errorCall->handleError(ErrorHandling::systemFunction, true); //general error
		}
		case 1: {
			d->errorCall->handleError(ErrorHandling::fcronError, true); //fcron error code
		}
	}

	//Open the temporary copy of the user's fcrontab file
	if(fcrontabFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
		if(((fcrontabFile.readAll()).trimmed()).isEmpty()) {
			if(!writeReminder(&fcrontabFile)) { //write error
				fileError = fcrontabFile.error();
				fcrontabFile.close();
				d->errorCall->handleError(ErrorHandling::writeReminderToFile, true, fileError);
			}

			fcrontabFile.close();
		} else {
			fcrontabFile.close(); //close it so that it can be opened for appending

			if(fcrontabFile.open(QIODevice::Append | QIODevice::Text)) {
				if(!writeReminder(&fcrontabFile)) { //write error
					fileError = fcrontabFile.error();
					fcrontabFile.close();
					d->errorCall->handleError(ErrorHandling::writeReminderToFile, true, fileError);
				}

				fcrontabFile.close();
			} else {
				d->errorCall->handleError(ErrorHandling::fcrontabFileOpen, true, fcrontabFile.error()); //open error
			}
		}
	} else {
		d->errorCall->handleError(ErrorHandling::fcrontabFileOpen, true, fcrontabFile.error()); //open error
	}

	switch(systemCall->execute(QString("fcrontab"), QStringList("/home/" + currentUser.loginName() + "/.KReminter_fcrontab"), -1)) {
		case -1: {
			d->errorCall->handleError(ErrorHandling::processCrashed, true); //Process crashed
		}
		case -2: {
			d->errorCall->handleError(ErrorHandling::processNotStarted, true); //Process could not be started
		}
		case 1: {
			d->errorCall->handleError(ErrorHandling::fcronError, true); //Fcron error code
		}
		default: {
			if(!fcrontabFile.remove()) {
				d->errorCall->handleError(ErrorHandling::fileDelete, true, fcrontabFile.error()); //delete error
			}

			if(!window()->close()) {
				window()->hide();
				d->errorCall->handleError(ErrorHandling::windowClose, true);
			}
		}
	}
}

/*
 * TODO: Implement error value conversion from QVariant
 */
bool NewReminderWindow::checkUserPermissions()
{
	QFile denyFile("/usr/local/etc/fcron.deny"), allowFile("/usr/local/etc/fcron.allow");
	QTextStream inputDenyFile(&denyFile), inputAllowFile(&allowFile);
	//QFile::FileError denyError = QFile::NoError, allowError = QFile::NoError;
	QString line;
	ActionReply reply;
	//bool userFoundInDeny = false, userFoundinAllow = false;
	KUser currentUser;

	if(!checkFilePermissions(&denyFile, &allowFile)) {
		QVariantMap args;
		Action rwFiles("org.kde.auth.kreminder.rwfcrontab");

		rwFiles.setHelperID("org.kde.auth.kreminder");
		args["fcron.deny"] = "/usr/local/etc/fcron.deny";
		args["fcron.allow"] = "/usr/local/etc/fcron.allow";
		rwFiles.setArguments(args);

		reply = rwFiles.execute();

		if(reply.failed()) {
			QVariantMap errorArgs(reply.data());

			if(reply.type() == ActionReply::KAuthError) { //Internal KAuth error
				d->errorCall->handleKAuthError(ErrorHandling::kauthintneralerror, true);
			}
			else if(reply.type() == ActionReply::HelperError) { //Self generated error code
				/*int originalFileError = errorArgs.value("originalFileError").toInt();
				int newFileError = errorArgs.value("newFileError").toInt();
				int streamError = errorArgs.value("streamError").toInt();
				bool stringError = errorArgs.value("stringError").toBool();
				bool fileRemoveError = errorArgs.value("fileRemovalError").toBool();*/

				/*if(errorArgs.value("originalFileError").convert()) {
					if(errorArgs.value("newFileError").convert()) {
						if(errorArgs.value("streamError").convert()) {
							if(errorArgs.value("stringError")) {
								if(errorArgs.value("fileRemovalError")) {
					d->errorCall->handleKAuthError(ErrorHandling::kauthcustomerror, true, errorArgs.value("originalFileError"), errorArgs.value("newFileError"), errorArgs.value("streamError"), errorArgs.value("stringError"), errorArgs.value("fileRemovalError"));
				}
				else {
					;
				}*/

				d->errorCall->handleKAuthError(ErrorHandling::kauthcustomerror, true);
			}

			return false;
		}
	}

	return true;
}

/*
 * Any errors from QFileInfo::ownerID(), groupId() or KUser::uid(), gid() are ignored,
 * and it is assumed that the user does not have read/write access to the needed file(s)
 */
bool NewReminderWindow::checkFilePermissions(QFile* denyFile, QFile* allowFile)
{
	QFileInfo denyFileInfo(*denyFile), allowFileInfo(*allowFile);
	KUser currentUser;

	if(denyFile->exists()) {
		//Note: On Unix, Qt returns whether the owner has read/write access (not the user!)
		if(((denyFileInfo.ownerId() == currentUser.uid()) && (denyFileInfo.isReadable()) && (denyFileInfo.isWritable())) &&
			((denyFileInfo.groupId() == currentUser.gid()) && (denyFileInfo.permission(QFile::ReadGroup | QFile::WriteGroup)))) {
			return true;
		}
	}

	if(allowFile->exists()) {
		//Note: On Unix, Qt returns whether the owner has read/write access (not the user!)
		if(((allowFileInfo.ownerId() == currentUser.uid()) && (allowFileInfo.isReadable()) && (allowFileInfo.isWritable())) &&
			((allowFileInfo.groupId() == currentUser.gid()) && (allowFileInfo.permission(QFile::ReadGroup | QFile::WriteGroup)))) {
			return true;
		}
	}

	//User does not have access to the file(s)
	return false;
}

//TODO: add year
//TODO: repeating reminders
const char *NewReminderWindow::formatReminder()
{
	QString reminder(QString::number(d->timeEdit->time().minute())); //Minute

	//Optional arguments may be added with a '&' prepended
	//ie. run the command after the conditions are successfuly met five times: "&5, <min> <hr> <day-of-month> <month> <day-of-week> <command>"

	reminder.append(" ");
	reminder.append(QString::number(d->timeEdit->time().hour())); //Hour
	reminder.append(" ");

	reminder.append(QString::number(d->calendarWidget->selectedDate().day())); //Day of Month
	reminder.append(" ");

	reminder.append(QString::number(d->calendarWidget->selectedDate().month())); //Month
	reminder.append(" * ");//Day of week

	reminder.append("rekonq\n"); //Command to run
	return reminder.toLocal8Bit().constData();
}

/*
 * Write the reminder to the fcrontab file
 */
bool NewReminderWindow::writeReminder(QFile *fcrontabFile)
{
	if(!fcrontabFile->write(formatReminder())) {
		return false; //write error
	}

	return true;
}

/*
 * Send the user back to the main menu
 */
void NewReminderWindow::sendToMenu()
{
	IntroWindow *mainMenu = new IntroWindow(0);
	mainMenu->show();

	if(!window()->close()) {
		window()->hide();
		d->errorCall->handleError(ErrorHandling::windowClose, false);
	}
}

