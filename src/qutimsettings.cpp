/*
    qutimSettings

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


#include <QDebug>
#include <QtopiaApplication>
#include <QKeyEvent>
#include <QSoftMenuBar>
#include <QTabWidget>

#include "qutimsettings.h"
#include "pluginsettings.h"
#include "pluginsystem.h"
#include "abstractchatlayer.h"
#include "abstracthistorylayer.h"
#include "abstractsoundlayer.h"
#include "antispam/abstractantispamlayer.h"
#include "notifications/abstractnotificationlayer.h"
#include "abstractcontactlist.h"
#include "globalsettings/abstractglobalsettings.h"

qutimSettings::qutimSettings(const QString &profile_name,
                             QWidget *parent) :
    QStackedWidget(parent),
    m_iconManager(IconManager::instance()),
    m_profile_name(profile_name),
    justShown(true)
{
  settingsSelector = new QWidget(this);
  ui.setupUi(settingsSelector);
  ui.settingsTree->header()->hide();
  addWidget(settingsSelector);

  createSettingsWidget();
  connect(ui.protocolBox, SIGNAL(currentIndexChanged(int)), SLOT(changeProtocolSettings(int)));
  fillProtocolComboBox();

  QSoftMenuBar::setLabel(this, Qt::Key_Back, QSoftMenuBar::Back);
  QMenu *menu = QSoftMenuBar::menuFor(this);
  menu->clear();
  menu->addAction(tr("Cancel"), this, SLOT(cancel()));
}
qutimSettings::~qutimSettings()
{
  delete msettings;
  delete m_account_management_widget;
  delete m_chat_window_settings;
  delete m_anti_spam_settings;
  delete m_notification_settings;
/*  delete m_sound_settings;*/
  delete m_history_settings;
/*  delete m_proxy_settings;*/
  deleteCurrentProtocolSettings();
}

void qutimSettings::fillProtocolComboBox()
{
  PluginSystem &ps = PluginSystem::instance();
  PluginInfoList protocol_list = ps.getPluginsByType("protocol");
  foreach(PluginInfo information_about_plugin, protocol_list)
  {
    ui.protocolBox->addItem(information_about_plugin.icon, information_about_plugin.name);
  }
}

void qutimSettings::createSettingsWidget()
{
  m_account_management_item = new QTreeWidgetItem(ui.settingsTree);
  m_account_management_item->setText(0, tr("Accounts"));
  m_account_management_item->setIcon(0, m_iconManager.getIcon("switch_user"));

  general = new QTreeWidgetItem(ui.settingsTree);
  general->setText(0, tr("General"));
  general->setIcon(0, m_iconManager.getIcon("mainsettings"));

  m_contact_list_settings_item = new QTreeWidgetItem(ui.settingsTree);
  m_contact_list_settings_item->setText(0, tr("Contact list"));
  m_contact_list_settings_item->setIcon(0, m_iconManager.getIcon("contactlist"));

  m_chat_settings_item = new QTreeWidgetItem(ui.settingsTree);
  m_chat_settings_item->setText(0, tr("Chat window"));
  m_chat_settings_item->setIcon(0, m_iconManager.getIcon("messaging"));

  m_history_settings_item = new QTreeWidgetItem(ui.settingsTree);
  m_history_settings_item->setText(0, tr("History"));
  m_history_settings_item->setIcon(0, m_iconManager.getIcon("history"));

  m_notifications_settings_item = new QTreeWidgetItem(ui.settingsTree);
  m_notifications_settings_item ->setText(0, tr("Notifications"));
  m_notifications_settings_item ->setIcon(0, m_iconManager.getIcon("events"));

  m_anti_spam_settings_item = new QTreeWidgetItem(ui.settingsTree);
  m_anti_spam_settings_item->setText(0, tr("Anti-spam"));
  m_anti_spam_settings_item->setIcon(0, m_iconManager.getIcon("antispam"));

/*  m_sound_settings_item = new QTreeWidgetItem(ui.settingsTree);
  m_sound_settings_item->setText(0, tr("Sound"));
  m_sound_settings_item->setIcon(0, m_iconManager.getIcon("soundsett"));


  m_global_proxy_item = new QTreeWidgetItem(ui.settingsTree);
  m_global_proxy_item->setText(0, tr("Global proxy"));
  m_global_proxy_item->setIcon(0, m_iconManager.getIcon("proxy"));*/

  //ui.settingsTree->resizeColumnToContents(0);
  createWidgetsStack();

  connect(ui.settingsTree,
          SIGNAL(itemActivated ( QTreeWidgetItem *, int)),
          this, SLOT(showSettings(QTreeWidgetItem *, int)));
  //ui.settingsTree->setCurrentItem(m_account_management_item);
}

void qutimSettings::createWidgetsStack()
{
  m_account_management_widget = new AccountManagement;
  addWidget(m_account_management_widget);

  msettings = new mainSettings(m_profile_name);
  connect(msettings, SIGNAL(settingsSaved()),
          this, SLOT(mainSettingsChanged()));
  connect(m_account_management_widget, SIGNAL(updateAccountComboBoxFromMainSettings()),
          msettings, SLOT(updateAccountComboBox()));
  addWidget(msettings);

  m_contact_list_settings = new ContactListSettings(m_profile_name);
  addWidget(m_contact_list_settings);
  connect(m_contact_list_settings, SIGNAL(settingsSaved()),
          this, SLOT(contactListSettingsChanged()));

  m_chat_window_settings = new ChatWindowSettings(m_profile_name);
  addWidget(m_chat_window_settings);
  connect(m_chat_window_settings, SIGNAL(settingsSaved()),
          this, SLOT(chatWindowSettingsChanged()));

  m_history_settings = new HistorySettings(m_profile_name);
  addWidget(m_history_settings);
  connect(m_history_settings, SIGNAL(settingsSaved()),
          this, SLOT(historySettingsChanged()));

  m_notification_settings = new NotificationsLayerSettings(m_profile_name);
  addWidget(m_notification_settings);
  connect(m_notification_settings, SIGNAL(settingsSaved()),
          this, SLOT(notificationsSettingsChanged()));

  m_anti_spam_settings = new AntiSpamLayerSettings(m_profile_name);
  addWidget(m_anti_spam_settings);
  connect(m_anti_spam_settings, SIGNAL(settingsSaved()),
          this, SLOT(antiSpamSettingsChanged()));

/*  m_sound_settings = new SoundLayerSettings(m_profile_name);
  addWidget(m_sound_settings);
  connect(m_sound_settings, SIGNAL(settingsSaved()),
          this, SLOT(soundSettingsChanged()));

  m_proxy_settings = new GlobalProxySettings(m_profile_name);
  addWidget(m_proxy_settings);
        connect(m_proxy_settings, SIGNAL(settingsSaved()),
                  this, SLOT(globalProxySettingsChanged()));*/
}

void qutimSettings::showSettings(QTreeWidgetItem *current, int)
{
  setCurrentIndex(ui.settingsTree->indexOfTopLevelItem(current)+1); // the first one is selector
}

void qutimSettings::cancel()
{
  deleteLater();
}

void qutimSettings::accept()
{
  if ( ui.protocolBox->count() )
    signalForSettingsSaving(ui.protocolBox->currentText());
  else
    signalForSettingsSaving();
  deleteLater();
}

void qutimSettings::saveAllSettings()
{
  msettings->saveSettings();
  m_contact_list_settings->saveSettings();
  m_chat_window_settings->saveSettings();
  m_anti_spam_settings->saveSettings();
  m_notification_settings->saveSettings();
/*  m_sound_settings->saveSettings();*/
  m_history_settings->saveSettings();
/*  m_proxy_settings->saveSettings();*/
}

void qutimSettings::changeProtocolSettings(int index)
{
  if (!m_current_account_name.isEmpty())
  {
    QMessageBox msgBox(QMessageBox::NoIcon, tr("Save settings"),
                       tr("Save %1 settings?").arg(m_current_account_name), QMessageBox::Yes | QMessageBox::No, this);
    switch ( QtopiaApplication::execDialog(&msgBox) )
    {
    case QMessageBox::Yes:
      signalForSettingsSaving(m_current_account_name);
      break;

    case QMessageBox::No:
      break;

    default:
      break;
    }
    deleteCurrentProtocolSettings();
  }

  m_current_account_name = ui.protocolBox->itemText(index);
  if ( !m_current_account_name.isEmpty() )
  {
    PluginSystem &ps = PluginSystem::instance();
    QList<SettingsStructure> settings_list = ps.getSettingsByName(m_current_account_name);
    if ( settings_list.count() > 0 )
    {
      foreach( SettingsStructure settings_structure, settings_list )
      {
        ui.settingsTree->addTopLevelItem(settings_structure.settings_item);
        QWidget *settings_widget = settings_structure.settings_widget;
        addWidget(settings_widget);
      }
    }
  }
}

void qutimSettings::onUpdateTranslation()
{
  general->setText(0, tr("General"));
}

void qutimSettings::signalForSettingsSaving(const QString &protocol_name)
{
  saveAllSettings();
  if (protocol_name.isEmpty())
    return;
  PluginSystem &ps = PluginSystem::instance();
  ps.applySettingsPressed(protocol_name);
}

void qutimSettings::deleteCurrentProtocolSettings()
{
  if ( !m_current_account_name.isEmpty() )
  {
    PluginSystem &ps = PluginSystem::instance();
    ps.removeProtocolSettingsByName(m_current_account_name);
  }
}

void qutimSettings::mainSettingsChanged()
{
  AbstractLayer &as = AbstractLayer::instance();
  as.reloadGeneralSettings();
}

void qutimSettings::chatWindowSettingsChanged()
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.loadSettings();
}

void qutimSettings::antiSpamSettingsChanged()
{
  AbstractAntiSpamLayer &aasl = AbstractAntiSpamLayer::instance();
  aasl.loadSettings();
}

void qutimSettings::notificationsSettingsChanged()
{
  AbstractNotificationLayer &anl = AbstractNotificationLayer::instance();
  anl.loadSettings();
}

void qutimSettings::soundSettingsChanged()
{
  AbstractSoundLayer::instance().loadSettings();
}

void qutimSettings::contactListSettingsChanged()
{
  AbstractContactList::instance().loadSettings();
}

void qutimSettings::historySettingsChanged()
{
  AbstractHistoryLayer::instance().loadSettings();
}

void qutimSettings::showSelector()
{
  setCurrentWidget(settingsSelector);
}

void qutimSettings::keyPressEvent(QKeyEvent *ev)
{
  if (ev->key()==Qt::Key_Back)
  {
    if (currentWidget()==settingsSelector) // Exit settings
    {
      qDebug() << "Accept Settings";
      accept();
    }
    else // go back to selector
    {
      qDebug() << "Back to Selector";
      showSelector();
    }
  }
}
