/*
    statusSettings

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


#ifndef STATUSSETTINGS_H
#define STATUSSETTINGS_H

#include <QtGui>
#include "ui_statussettings.h"

class statusSettings : public QWidget
{
    Q_OBJECT

public:
    statusSettings(const QString &profile_name,QWidget *parent = 0);
    ~statusSettings();
    void loadSettings();
    void saveSettings();

private slots:
	void widgetStateChanged() { changed = true; emit settingsChanged(); }
	void statusEditChanged(int);
	
signals:
    void settingsChanged();
    void settingsSaved();
    
private:
    Ui::statusSettingsClass ui;
    bool changed;
    
    bool awayDontShow;
    bool lunchDontShow;
    bool evilDontShow;
    bool depressionDontShow;
    bool atHomeDontShow;
    bool atWorkDontShow;
    bool naDontShow;
    bool occupiedDontShow;
    bool dndDontShow;

    QString awayMessage;
    QString lunchMessage;
    QString evilMessage;
    QString depressionMessage;
    QString atHomeMessage;
    QString atWorkMessage;
    QString naMessage;
    QString occupiedMessage;
    QString dndMessage;
    
    int currentStatusIndex;
    QString m_profile_name;
    
};

#endif // STATUSSETTINGS_H
