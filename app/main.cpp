/*
    Copyright (C) 2004-2011 Martin Jolicoeur (snmpb1@gmail.com) 

    This file is part of the SnmpB project 
    (http://sourceforge.net/projects/snmpb)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <qapplication.h>
#include <qmainwindow.h>
#include <QTranslator>
#include <QLibraryInfo>
#include "snmpb.h"
#include "mibeditor.h"
#include "snmpbapp.h"

QString file_to_open;

int main( int argc, char ** argv )
{
    Snmpb snmpb;
    SnmpBApplication app( argc, argv );

    // Qt translations
    QTranslator l10n_qt;
    l10n_qt.load("qt_" + QLocale::system().name(),
                 QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&l10n_qt);

    // SnmpB translations
    QTranslator l10n_app;
    l10n_app.load(":/i18n/snmpb." + QLocale::system().name());
    app.installTranslator(&l10n_app);

    QMainWindow mw;
    snmpb.BindToGUI(&mw);
    mw.show();
    app.connect(&app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ));

    // Load a file specified as argument in the Mib Editor
    if (!file_to_open.isEmpty() || QCoreApplication::arguments().count() > 1)
    {
        snmpb.MibEditorObj()->MibFileOpen(file_to_open.isEmpty()?QCoreApplication::arguments().at(1):file_to_open);
        snmpb.MainUI()->TabW->setCurrentIndex(2); // Select the Editor Tab
    }

    return app.exec();
}
