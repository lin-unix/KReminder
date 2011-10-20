/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Steven Sroka <email>

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

#include "kreminderhelper.h"

#include <KDE/KUser>

#include <QtCore/QFile>
#include <QtCore/QTextStream>

//Change the contents of fcron.deny
ActionReply KReminderHelper::IODeny(QVariantMap args)
{
	ActionReply reply;
	KUser currentUser;
	QString originalFilename = args["filename"].toString(), newFilename = "/home/" + currentUser.loginName() + "/.tempKReminderDeny";
	QFile originalFile(originalFilename), newFile(newFilename);
	QTextStream originalInputStream(&originalFile), newOutputStream(&newFile);
	QString contents, line;

	if((!originalFile.open(QIODevice::ReadOnly | QIODevice::Text)) || (!newFile.open(QIODevice::WriteOnly | QIODevice::Text))) {
		reply = ActionReply::HelperErrorReply;
		reply.setErrorCode(originalFile.error());

		return reply;
	}
	
	while (!originalInputStream.atEnd()) {
		line = originalInputStream.readLine();

		if((line.isNull()) || (originalInputStream.status() != QTextStream::Ok) || (originalFile.error() != QFile::NoError)) {
			originalFile.close();
			newFile.close();
			//handleError(originalInputStream, originalFile.error(), originalInputStream.status());
		}

		if (!line.contains(currentUser.loginName(), Qt::CaseSensitive)) //if the current user is not in this list
			newFile.write(line.toLocal8Bit().constData());
	}

	originalFile.close();
	newFile.close();

	return reply;
}

//Change the contents of fcron.allow
ActionReply KReminderHelper::IOAllow(QVariantMap args)
{
	ActionReply reply;
	KUser currentUser;
	QFile file(args["filename"].toString());

	if(!file.open(QIODevice::Append | QIODevice::Text)) {
		reply = ActionReply::HelperErrorReply;
		reply.setErrorCode(file.error());

		return reply;
	}

	file.write(currentUser.loginName().toLocal8Bit().constData());
	file.close();
	
	return reply;
}

KDE4_AUTH_HELPER_MAIN("org.kde.auth.kreminder", KReminderHelper)