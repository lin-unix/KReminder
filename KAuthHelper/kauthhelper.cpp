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

#include "kauthhelper.h"

#include <KDE/KUser>
#include <KDE/KProcess>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtCore/QVariantMap>
#include <QtCore/QString>

//Change the contents of fcron.deny
ActionReply KAuthHelper::IODeny(QVariantMap args)
{
	ActionReply reply = ActionReply::HelperErrorReply;
	QVariantMap returnData;
	KUser currentUser;
	KProcess *systemCall = new KProcess(this);
	QString contents, line;
	QString originalFilename = args["filename"].toString(), newFilename = "/home/" + currentUser.loginName() + "/.tempKReminderDeny";
	QFile originalFile(originalFilename), newFile(newFilename);
	QTextStream inputStream(&originalFile);

	if(!originalFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		returnData["stringError"] = 0;
		returnData["fileError"] = originalFile.error();
		returnData["streamError"] = 0;

		reply.setData(returnData);
		return reply;
	}

	if(!newFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		returnData["stringError"] = 0;
		returnData["fileError"] = newFile.error();
		returnData["streamError"] = 0;

		reply.setData(returnData);
		return reply;
	}

	while(!inputStream.atEnd()) {
		line = inputStream.readLine();

		if((originalFile.error() != QFile::NoError) || (inputStream.status() != QTextStream::Ok) || (line.isNull())) {
			originalFile.close();
			newFile.close();

			returnData["stringError"] = line.isNull();
			returnData["fileError"] = originalFile.error();
			returnData["streamError"] = inputStream.status();

			reply.setData(returnData);
			return reply;
		}

		if(!line.contains(currentUser.loginName(), Qt::CaseSensitive)) { //if the current user is not in this list
			if(newFile.write(line.toLocal8Bit().constData()) == -1) {
				originalFile.close();
				newFile.close();

				returnData["stringError"] = 0;
				returnData["fileError"] = newFile.error();
				returnData["streamError"] = 0;

				reply.setData(returnData);
				return reply;
			}
		}
	}

	if(!systemCall->execute(QString("mv " + newFilename + " /usr/local/etc/fcron.deny"), QStringList(), -1)) {
		originalFile.close();
		newFile.close();

		returnData["stringError"] = 0;
		returnData["fileError"] = 0;
		returnData["streamError"] = 0;

		reply.setData(returnData);
		return reply;
	}

	originalFile.close();
	newFile.close();

	returnData["stringError"] = 0;
	returnData["fileError"] = 0;
	returnData["streamError"] = 0;

	reply.setData(returnData);
	return reply;
}

//Change the contents of fcron.allow
ActionReply KAuthHelper::IOAllow(QVariantMap args)
{
	ActionReply reply;
	KUser currentUser;
	QFile file(args["filename"].toString());

	if(!file.open(QIODevice::Append | QIODevice::Text)) {
		reply = ActionReply::HelperErrorReply;
		reply.data()["stringError"] = 0;
		reply.data()["fileError"] = file.error();
		reply.data()["streamError"] = 0;

		return reply;
	}

	if(file.write(currentUser.loginName().toLocal8Bit().constData()) == -1) {
		file.close();

		reply = ActionReply::HelperErrorReply;
		reply.data()["stringError"] = 0;
		reply.data()["fileError"] = file.error();
		reply.data()["streamError"] = 0;

		return reply;
	}

	file.close();
	return reply;
}

void KAuthHelper::setError()
{
	;
}

KDE4_AUTH_HELPER_MAIN("org.kde.auth.kreminder", KAuthHelper)