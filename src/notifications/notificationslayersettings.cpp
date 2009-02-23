/*
    NotificationsLayerSettings

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

#include "notificationslayersettings.h"
#include <QSettings>

NotificationsLayerSettings::NotificationsLayerSettings(const QString &profile_name,
    QWidget *parent)
    : QWidget(parent),
    m_profile_name(profile_name)
{
  ui.setupUi(this);
  changed = false;
  loadSettings();
  connect(ui.popupBox, SIGNAL(toggled(bool)),
          this, SLOT(widgetStateChanged()));
  connect(ui.popupBox, SIGNAL(toggled(bool)),
          ui.notifyBox, SLOT(setEnabled(bool)));
  connect(ui.signOnCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(widgetStateChanged()));
  connect(ui.signOffCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(widgetStateChanged()));
  connect(ui.typingCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(widgetStateChanged()));
  connect(ui.changeStatusCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(widgetStateChanged()));
  connect(ui.messageBox, SIGNAL(stateChanged(int)),
          this, SLOT(widgetStateChanged()));
}

NotificationsLayerSettings::~NotificationsLayerSettings()
{

}

void NotificationsLayerSettings::loadSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("notifications");
  ui.popupBox->setChecked(settings.value("popup", true).toBool());
  ui.signOnCheckBox->setChecked(settings.value("signon", true).toBool());
  ui.signOffCheckBox->setChecked(settings.value("signoff", true).toBool());
  ui.typingCheckBox->setChecked(settings.value("typing", true).toBool());
  ui.changeStatusCheckBox->setChecked(settings.value("statuschange", true).toBool());
  ui.messageBox->setChecked(settings.value("message", true).toBool());
  settings.endGroup();
}

void NotificationsLayerSettings::saveSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("notifications");
  settings.setValue("popup", ui.popupBox->isChecked());
  settings.setValue("signon", ui.signOnCheckBox->isChecked());
  settings.setValue("signoff", ui.signOffCheckBox->isChecked());
  settings.setValue("typing", ui.typingCheckBox->isChecked());
  settings.setValue("statuschange", ui.changeStatusCheckBox->isChecked());
  settings.setValue("message", ui.messageBox->isChecked());
  settings.endGroup();
  if ( changed )
    emit settingsSaved();
  changed = false;
}
