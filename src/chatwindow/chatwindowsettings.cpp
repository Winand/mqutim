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

#include "chatwindowsettings.h"
#include <QSettings>

ChatWindowSettings::ChatWindowSettings(const QString &profile_name, QWidget *parent)
    : QWidget(parent)
    , m_profile_name(profile_name)
{
  ui.setupUi(this);
  changed = false;
  loadSettings();
  connect(ui.openBox, SIGNAL(stateChanged(int)),
          this, SLOT(widgetStateChanged()));
  connect(ui.eventsBox, SIGNAL(stateChanged(int)),
          this, SLOT(widgetStateChanged()));
  connect(ui.typingBox, SIGNAL(stateChanged(int)),
          this, SLOT(widgetStateChanged()));
  connect(ui.removeBox, SIGNAL(stateChanged(int)),
          this, SLOT(widgetStateChanged()));
  connect(ui.namesBox, SIGNAL(stateChanged(int)),
          this, SLOT(widgetStateChanged()));
  connect(ui.removeSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(widgetStateChanged()));
  connect(ui.timestampComboBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(widgetStateChanged()));
  connect(ui.contentBox, SIGNAL(stateChanged(int)),
          this, SLOT(widgetStateChanged()));
}

ChatWindowSettings::~ChatWindowSettings()
{

}

void ChatWindowSettings::loadSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("chatwindow");
  ui.openBox->setChecked(settings.value("open", false).toBool());
  ui.eventsBox->setChecked(settings.value("events", false).toBool());
  ui.typingBox->setChecked(settings.value("typing", true).toBool());
  ui.removeBox->setChecked(settings.value("remove", true).toBool());
  ui.namesBox->setChecked(settings.value("names", true).toBool());
  ui.removeSpinBox->setValue(settings.value("removecount", 60).toUInt());
  ui.timestampComboBox->setCurrentIndex(settings.value("timestamp", 1).toUInt());
  ui.contentBox->setChecked(settings.value("content", false).toBool());
  settings.endGroup();
}

void ChatWindowSettings::saveSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("chatwindow");
  settings.setValue("open", ui.openBox->isChecked());
  settings.setValue("events", ui.eventsBox->isChecked());
  settings.setValue("typing", ui.typingBox->isChecked());
  settings.setValue("remove", ui.removeBox->isChecked());
  settings.setValue("names", ui.namesBox->isChecked());
  settings.setValue("removecount", ui.removeSpinBox->value());
  settings.setValue("timestamp", ui.timestampComboBox->currentIndex());
  settings.setValue("content", ui.contentBox->isChecked());
  settings.endGroup();
  if ( changed )
    emit settingsSaved();
  changed = false;
}
