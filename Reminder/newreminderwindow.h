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

#ifndef NEWREMINDERWINDOW_H
#define NEWREMINDERWINDOW_H

#include <KDE/KXmlGuiWindow>

class NewReminderWindowPrivate;

class NewReminderWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    NewReminderWindow(QWidget *parent = 0);
    virtual ~NewReminderWindow();

private:
    NewReminderWindowPrivate *const d;

    void setupObjects();

private slots:
    void changeDateTime(bool checked);
    void saveReminder();
    void sendToMenu();
};

#endif // NEWREMINDERWINDOW_H
