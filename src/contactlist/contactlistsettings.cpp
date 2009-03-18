#include "contactlistsettings.h"
#include <QDebug>
#include <string.h>

ContactListSettings::ContactListSettings(const QString &profile_name, QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);
  m_profile_name = profile_name;
  m_gui_changed=false;
  loadSettings();
  connect (ui.showAccountsBox, SIGNAL(stateChanged(int)), this, SLOT(widgetSettingsChanged()));
  connect (ui.showGroupsBox, SIGNAL(stateChanged(int)), this, SLOT(widgetSettingsChanged()));
  connect (ui.showAccountsBox, SIGNAL(stateChanged(int)), this, SLOT(widgetSettingsChanged()));
  connect (ui.hideEmptyBox, SIGNAL(stateChanged(int)), this, SLOT(widgetSettingsChanged()));
  connect (ui.offlineBox, SIGNAL(stateChanged(int)), this, SLOT(widgetSettingsChanged()));
  connect (ui.separatorBox, SIGNAL(stateChanged(int)), this, SLOT(widgetSettingsChanged()));
  connect (ui.sortstatusCheckBox, SIGNAL(stateChanged(int)), this, SLOT(widgetSettingsChanged()));
  connect (ui.alternatingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(widgetSettingsChanged()));
  connect (ui.clientIconCheckBox, SIGNAL(stateChanged(int)), this, SLOT(widgetSettingsChanged()));
}

ContactListSettings::~ContactListSettings()
{
}

void ContactListSettings::loadSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("contactlist");
  ui.alternatingCheckBox->setChecked(settings.value("alternatingrc",false).toBool());
  ui.clientIconCheckBox->setChecked(settings.value("showicon12",false).toBool());
  ui.showAccountsBox->setChecked((settings.value("modeltype",0).toInt()&1)==0);
  ui.showGroupsBox->setChecked((settings.value("modeltype",0).toInt()&2)==0);
  ui.offlineBox->setChecked(!settings.value("showoffline",true).toBool());
  ui.hideEmptyBox->setChecked(!settings.value("showempty",true).toBool());
  ui.sortstatusCheckBox->setChecked(settings.value("sortstatus",false).toBool());
  ui.separatorBox->setChecked(!settings.value("showseparator",true).toBool());

  settings.endGroup();
}
void ContactListSettings::saveSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("contactlist");
  settings.setValue("alternatingrc",ui.alternatingCheckBox->isChecked());
  settings.setValue("showicon12",ui.clientIconCheckBox->isChecked());
  int model_type = ui.showAccountsBox->isChecked()?0:1;
  model_type += ui.showGroupsBox->isChecked()?0:2;
  settings.setValue("modeltype",model_type);
  settings.setValue("showoffline",!ui.offlineBox->isChecked());
  settings.setValue("showempty",!ui.hideEmptyBox->isChecked());
  settings.setValue("sortstatus",ui.sortstatusCheckBox->isChecked());
  settings.setValue("showseparator",!ui.separatorBox->isChecked());
  settings.endGroup();
  if (m_gui_changed)
    AbstractContactList::instance().loadGuiSettings();
  emit settingsSaved();
}
void ContactListSettings::widgetSettingsChanged()
{
  emit settingsChanged();
}
void ContactListSettings::widgetGuiSettingsChanged()
{
  m_gui_changed=true;
  emit settingsChanged();
}
