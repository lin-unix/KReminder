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

#include <QtCore/QFile>
#include <QtCore/QTextStream>

//Change the contents of fcron.deny
ActionReply KReminderHelper::IODeny(QVariantMap args)
{
	ActionReply reply;
	QString filename = args["filename"].toString();
	QFile file(filename);
	QTextStream stream(&file);
	QString contents;

	if(!file.open(QIODevice::ReadOnly)) {
		reply = ActionReply::HelperErrorReply;
		reply.setErrorCode(file.error());

		return reply;
	}

	//Add code here
	stream >> contents;
	reply.data()["contents"] = contents;

	return reply;
}

//Change the contents of fcron.allow
ActionReply KReminderHelper::IOAllow(QVariantMap args)
{
	ActionReply reply;
	QString filename = args["filename"].toString();
	QFile file(filename);
	QTextStream stream(&file);
	QString contents;

	if(!file.open(QIODevice::ReadOnly)) {
		reply = ActionReply::HelperErrorReply;
		reply.setErrorCode(file.error());

		return reply;
	}

	//Add code here
	stream >> contents;
	reply.data()["contents"] = contents;

	return reply;
}

KDE4_AUTH_HELPER_MAIN("org.kde.auth.kreminder", KReminderHelper)