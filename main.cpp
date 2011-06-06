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

#include "Introduction/introwindow.h"

#include <KDE/KApplication>
#include <KDE/KAboutData>
#include <KDE/KCmdLineArgs>
#include <KDE/KLocale>
#include <KDE/KIconLoader>

#include <QtGui/QDesktopWidget>

int main(int argc, char **argv)
{
    KAboutData aboutData("KReminder",
                         0,
                         ki18n("KReminder"),
                         "1.0",
                         ki18n("A Replacement For Your Short-Term Memory"),
                         KAboutData::License_GPL_V3,
                         ki18n("Copyright (c) 2010 Steven Sroka"),
                         ki18n(""),
                         "",
                         "Sroka.Steven@gmail.com");

    aboutData.setProgramIconName("view-calendar-tasks");

    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication app;

    KIconLoader *loader = KIconLoader::global();
    app.setWindowIcon(loader->loadIcon("view-calendar-tasks", KIconLoader::Small));

    IntroWindow *mainWindow = new IntroWindow(0);
    mainWindow->show();

    return app.exec();
}
