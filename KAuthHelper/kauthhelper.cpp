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

#include "kauthhelper.h"

#include <KDE/KUser>
#include <KDE/KProcess>

#include <QtCore/QStringList>
#include <QtCore/QString>

/*
 * Notes about the priorities of fcron.deny and fcron.allow:
 * If fcron.deny does not exist but fcron.allow exists, then only those users in fcron.allow are allowed to use fcron.
 * If fcron.deny exists but fcron.allow does not exist, then only those not in fcron.deny are allowed to use fcron.
 * If neither fcron.deny and fcron.allow exist, then everyone is allowed to use fcron.
 * If both fcron.deny and fcron.allow exist, then fcron.deny overrides fcron.allow.
 *
 * Note: After this class has checked whether or not fcron.deny and fcron.allow exists,
 * this class assumes that the file(s) will continue to exist throughout the execution of this class
 * because there is no guarantee that the file(s) have not been deleted (even with file locks). Good ol' OS!
 *
 *
 * Read fcron.deny and check if the user is listed in the file.
 * If the user is in fcron.deny, then remove them.
 *
 * Read fcron.allow and check if the user is listed in the file.
 * If the user is not in fcron.allow, then add them.
 */

ActionReply KAuthHelper::rwfcrontab(QVariantMap args)
{
	ActionReply reply = readDeny(args);

	if(reply.failed()) {
		return reply;
	}

	return readAllow(args);
}

ActionReply KAuthHelper::readDeny(QVariantMap args)
{
	KUser currentUser;
	bool userFoundInDeny = false;
	QString denyFilename = args["fcron.deny"].toString(), line;
	QFile denyFile(denyFilename);
	QTextStream inputDenyFile(&denyFile);
	ActionReply reply;

	if(denyFile.exists()) {
		if(denyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			while(!inputDenyFile.atEnd()) {
				line = inputDenyFile.readLine();

				if((line.isNull()) || (inputDenyFile.status() != QTextStream::Ok) || (denyFile.error() != QFile::NoError)) {
					return setReturnValue(true, &denyFile, NULL, line.isNull(), inputDenyFile.status());
				}

				if(line.contains(currentUser.loginName(), Qt::CaseSensitive)) { //if the current user is in this list
					denyFile.close();
					userFoundInDeny = true;
					break;
				}
			}

			denyFile.close();
		}
		else {
			return setReturnValue(true, &denyFile);
		}
	}

	if(userFoundInDeny) {
		reply = editDeny(args);
	}

	return reply;
}

ActionReply KAuthHelper::readAllow(QVariantMap args)
{
	KUser currentUser;
	QString line;
	bool userFoundInAllow = false;
	QString allowFilename = args["fcron.allow"].toString();
	QFile allowFile(allowFilename);
	QTextStream inputAllowFile(&allowFile);
	ActionReply reply;

	if(allowFile.exists()) {
		if(allowFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			while(!inputAllowFile.atEnd()) {
				line = inputAllowFile.readLine();

				if((line.isNull()) || (inputAllowFile.status() != QTextStream::Ok) || (allowFile.error() != QFile::NoError)) {
					return setReturnValue(true, &allowFile, NULL, line.isNull(), inputAllowFile.status());
				}
				else if((line.contains(currentUser.loginName(), Qt::CaseSensitive)) || (line.contains(QString("all"), Qt::CaseInsensitive))) {
					allowFile.close();
					userFoundInAllow = true;
					break; //Found the user's username
				}
			}

			allowFile.close();
		}
		else {
			return setReturnValue(true, &allowFile);
		}
	}

	if(!userFoundInAllow) {
		reply = editAllow(args);
	}

	return reply;
}

//Change the contents of fcron.deny
ActionReply KAuthHelper::editDeny(QVariantMap args)
{
	KUser currentUser;
	KProcess *systemCall = new KProcess(this);
	QString contents, line;
	QString originalFilename = args["filename"].toString(), newFilename = currentUser.homeDir() + "/.tempKReminderDeny";
	QFile originalFile(originalFilename), newFile(newFilename);
	QTextStream inputStream(&originalFile);

	if(!originalFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return setReturnValue(true, &originalFile);
	}

	if(!newFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return setReturnValue(true, NULL, &newFile);
	}

	while(!inputStream.atEnd()) {
		line = inputStream.readLine();

		if((originalFile.error() != QFile::NoError) || (inputStream.status() != QTextStream::Ok) || (line.isNull())) {
			return setReturnValue(true, &originalFile, &newFile, line.isNull(), inputStream.status());
		}

		if(!line.contains(currentUser.loginName(), Qt::CaseSensitive)) { //if the current user is not in this list
			if(newFile.write(line.toLocal8Bit().constData()) == -1) {
				return setReturnValue(true, &originalFile, &newFile, line.isNull(), inputStream.status());
			}
		}
	}

	if(!systemCall->execute(QString("mv " + newFilename + " /usr/local/etc/fcron.deny"), QStringList(), -1)) {
		return setReturnValue(true, &originalFile, &newFile);
	}

	return setReturnValue(false);
}

//Change the contents of fcron.allow
ActionReply KAuthHelper::editAllow(QVariantMap args)
{
	KUser currentUser;
	QFile file(args["filename"].toString());

	if(!file.open(QIODevice::Append | QIODevice::Text)) {
		return setReturnValue(true, &file);
	}

	if(!file.write(currentUser.loginName().toLocal8Bit().constData())) {
		return setReturnValue(true, &file);
	}
	else {
		return setReturnValue(false, &file);
	}
}

ActionReply KAuthHelper::setReturnValue(bool isError, QFile *fileOne, QFile *fileTwo, bool isStringNull, QTextStream::Status streamStatus)
{
	ActionReply reply = ActionReply::HelperErrorReply;
	QVariantMap returnData;

	returnData["isError"] = isError;
	returnData["originalFileError"] = fileOne->error();
	returnData["newFileError"] = fileTwo->error();
	returnData["stringError"] = isStringNull;
	returnData["streamError"] = streamStatus;
	returnData["fileRemovalError"] = fileTwo->remove();

	fileOne->close();
	fileTwo->close();

	reply.setData(returnData);
	return reply;
}

KDE4_AUTH_HELPER_MAIN("org.kde.auth.kreminder", KAuthHelper)