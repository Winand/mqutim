/*
    networkSettings

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


#ifndef NETWORKSETTINGS_H
#define NETWORKSETTINGS_H

#include <QtGui/QWidget>
#include <QSettings>
#include "ui_networksettings.h"

class networkSettings : public QWidget
{
    Q_OBJECT

public:
    networkSettings(const QString &profile_name, QWidget *parent = 0);
    ~networkSettings();
    void loadSettings();
    void saveSettings();

private slots:
	void widgetStateChanged() { changed = true; emit settingsChanged(); }
/*	void proxyTypeChanged( int );*/
    
signals:
    void settingsChanged();
    void settingsSaved();
    
private:
    Ui::networkSettingsClass ui;
    bool changed;
    QString m_profile_name;
};

#endif // NETWORKSETTINGS_H
