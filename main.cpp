/*
    main.cpp

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "src/qutim.h"
#include <qtopiaapplication.h>
QTOPIA_ADD_APPLICATION(QTOPIA_TARGET, qutIM)
QTOPIA_MAIN

/*#include <QApplication>

#include "src/qutim.h"
#include "themeengine/abstractthemeengine.h"
#include <QString>
#include <QByteArray>

#if defined(STATIC_IMAGE_PLUGINS)
Q_IMPORT_PLUGIN(qjpeg)
Q_IMPORT_PLUGIN(qgif)
#endif

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	if ( QFile::exists(QCoreApplication::applicationDirPath() + "/config"))
		QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationDirPath() + "/config");
	QSettings::setDefaultFormat(QSettings::IniFormat);

//	Example of valid creating of QSettings
//	QSettings settings(QSettings::defaultFormat(),QSettings::UserScope, "qutim", "bla-bla");

//	QTranslator tra;
//	tra.load("qutim_ru");
//	a.installTranslator(&tra);
	a.setQuitOnLastWindowClosed(false);
	qutIM w;


//	QFile file("qss/simple.qss");
//    if(file.open(QFile::ReadOnly))
//    {
//    	qApp->setStyleSheet("");
//	    QString styleSheet = QLatin1String(file.readAll());
//	    a.setStyleSheet(styleSheet);
//	    file.close();
//    }
//    w.show();
	if (w.isShouldRun())
		return a.exec();
	else
		return 0;					// User decide to do not sign in
}*/

#ifdef Q_WS_WIN
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	LPWSTR *arg_w;
	int argc;
	arg_w = CommandLineToArgvW(GetCommandLineW(), &argc);
	char **argv = new char *[argc];
	for(int i=0;i<argc;i++)
	{
		int len=0;
		while(!arg_w[i][len])
			len++;
		argv[i] = QString::fromUtf16((ushort *)arg_w[i],len).toLocal8Bit().data();
	}
	return main(argc, argv);
}
#endif
