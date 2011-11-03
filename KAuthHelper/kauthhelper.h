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


#ifndef KAUTHHELPER_H
#define KAUTHHELPER_H

#include <kauth.h>

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QVariantMap>

using namespace KAuth;

class KAuthHelper : public QObject
{
private:
	ActionReply setReturnValue(bool isError = false, QFile *fileOne = NULL, QFile *fileTwo = NULL, bool isStringNull = false, QTextStream::Status streamStatus = QTextStream::Ok);

private slots:
	ActionReply rwfcrontab(QVariantMap args);
	ActionReply readDeny(QVariantMap args);
	ActionReply readAllow(QVariantMap args);
	ActionReply editDeny(QVariantMap args);
	ActionReply editAllow(QVariantMap args);
};

#endif //KAUTHHELPER_H
