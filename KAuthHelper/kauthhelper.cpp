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
#include <QtCore/QVariantMap>
#include <QtCore/QString>

//Change the contents of fcron.deny
ActionReply KAuthHelper::IODeny(QVariantMap args)
{
	KUser currentUser;
	KProcess *systemCall = new KProcess(this);
	QString contents, line;
	QString originalFilename = args["filename"].toString(), newFilename = "/home/" + currentUser.loginName() + "/.tempKReminderDeny";
	QFile originalFile(originalFilename), newFile(newFilename);
	QTextStream inputStream(&originalFile);

	if(!originalFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return setReturnValue(&originalFile);
	}

	if(!newFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return setReturnValue(NULL, &newFile);
	}

	while(!inputStream.atEnd()) {
		line = inputStream.readLine();

		if((originalFile.error() != QFile::NoError) || (inputStream.status() != QTextStream::Ok) || (line.isNull())) {
			return setReturnValue(&originalFile, &newFile, line.isNull(), inputStream.status());
		}

		if(!line.contains(currentUser.loginName(), Qt::CaseSensitive)) { //if the current user is not in this list
			if(newFile.write(line.toLocal8Bit().constData()) == -1) {
				return setReturnValue(&originalFile, &newFile, line.isNull(), inputStream.status());
			}
		}
	}

	if(!systemCall->execute(QString("mv " + newFilename + " /usr/local/etc/fcron.deny"), QStringList(), -1)) {
		return setReturnValue(&originalFile, &newFile);
	}

	return setReturnValue();
}

//Change the contents of fcron.allow
ActionReply KAuthHelper::IOAllow(QVariantMap args)
{
	KUser currentUser;
	QFile file(args["filename"].toString());

	if(!file.open(QIODevice::Append | QIODevice::Text)) {
		return setReturnValue(&file);
	}

	file.write(currentUser.loginName().toLocal8Bit().constData()); //Any errors will be found and returned in the line below
	return setReturnValue(&file);
}

ActionReply KAuthHelper::setReturnValue(QFile *originalFile, QFile *newFile, bool isStringNull, QTextStream::Status streamStatus)
{
	ActionReply reply = ActionReply::HelperErrorReply;
	QVariantMap returnData;

	returnData["originalFileError"] = originalFile->error();
	returnData["newFileError"] = newFile->error();
	returnData["stringError"] = isStringNull;
	returnData["streamError"] = streamStatus;
	returnData["fileRemovalError"] = newFile->remove();

	originalFile->close();
	reply.setData(returnData);
	return reply;
}

KDE4_AUTH_HELPER_MAIN("org.kde.auth.kreminder", KAuthHelper)