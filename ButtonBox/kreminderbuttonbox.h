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

#ifndef KREMINDERBUTTONBOX_H
#define KREMINDERBUTTONBOX_H

#include <KDE/KDialogButtonBox>

class KReminderButtonBoxPrivate;

class KReminderButtonBox : public KDialogButtonBox
{
    Q_OBJECT

public:
    KReminderButtonBox(QWidget *parent = 0);
    virtual ~KReminderButtonBox();

private:
    KReminderButtonBoxPrivate *const d;
    void setupObjects();

private slots:
    void setAddReminderToolTip(bool checked);
    void setAddNoteToolTip(bool checked);
    void setConfigureToolTip(bool checked);
    void next();
    void sendToMenu();
	void saveReminder();
};

#endif // KREMINDERBUTTONBOX_H
