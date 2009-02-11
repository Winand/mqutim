/*
    ChatWindowSettings

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

#ifndef CHATWINDOWSETTINGS_H
#define CHATWINDOWSETTINGS_H

#include <QtGui/QWidget>
#include "ui_chatwindowsettings.h"

class ChatWindowSettings : public QWidget
{
    Q_OBJECT

public:
    ChatWindowSettings(const QString &profile_name, QWidget *parent = 0);
    ~ChatWindowSettings();
    void loadSettings();
    void saveSettings();

private slots:
	void widgetStateChanged() { changed = true; emit settingsChanged(); }
    
signals:
  	void settingsChanged();
    void settingsSaved();
    
private:
    Ui::ChatWindowSettingsClass ui;
    bool changed;
    QString m_profile_name;
};

#endif // CHATWINDOWSETTINGS_H
