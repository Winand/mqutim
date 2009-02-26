/*
    AbstractLayer

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

#include <QtopiaApplication>

#include "abstractlayer.h"
#include "addaccountwizard.h"
#include "qutimsettings.h"
#include "qutim.h"
#include "iconmanager.h"
#include "abstractstatus.h"
#include "abstractchatlayer.h"
#include "antispam/abstractantispamlayer.h"
#include "notifications/abstractnotificationlayer.h"
#include "abstracthistorylayer.h"
#include "abstractsoundlayer.h"
#include "globalsettings/abstractglobalsettings.h"


AbstractLayer::AbstractLayer() : m_plugin_system(PluginSystem::instance()),
    m_contact_list_management(AbstractContactList::instance()),
    m_icon_manager(IconManager::instance()),
    m_show_account_menus(false),
    m_is_new_profile(false)
{

}

AbstractLayer::~AbstractLayer()
{

}

AbstractLayer &AbstractLayer::instance()
{
  static AbstractLayer al;
  return al;
}

void AbstractLayer::setPointers(qutIM *parent)
{
  m_parent = parent;
}

void AbstractLayer::setupProfile()
{
  m_current_profile = "main";
  
  m_plugin_system.loadProfile(m_current_profile);
  AbstractStatusLayer &asl = AbstractStatusLayer::instance();
  asl.setProfileName(m_current_profile);
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.loadProfile(m_current_profile);
  AbstractAntiSpamLayer &aasl = AbstractAntiSpamLayer::instance();
  aasl.loadProfile(m_current_profile);
  AbstractNotificationLayer &anl = AbstractNotificationLayer::instance();
  anl.loadProfile(m_current_profile);
  AbstractSoundLayer &asndl = AbstractSoundLayer::instance();
  asndl.loadProfile(m_current_profile);
  AbstractHistoryLayer &ahl = AbstractHistoryLayer::instance();
  ahl.loadProfile(m_current_profile);
  AbstractContactList &acll = AbstractContactList::instance();
  acll.loadProfile(m_current_profile);
  asndl.playSound(NotifyStartup);
  m_icon_manager.loadProfile(m_current_profile);
  AbstractGlobalSettings::instance().setProfileName(m_current_profile);
}

void AbstractLayer::createNewAccount()
{
  AddAccountWizard wizard;
  wizard.addProtocolsToWizardList(m_plugin_system.getPluginsByType("protocol"));
  if ( QtopiaApplication::execDialog(&wizard) )
  {
    QString protocol_name = wizard.getChosenProtocol();
    if ( !protocol_name.isEmpty() )
    {
      m_plugin_system.saveLoginDataByName(protocol_name);
      m_plugin_system.removeLoginWidgetByName(protocol_name);
    }
    updateStausMenusInTrayMenu();
  }
  else
  {
    QString protocol_name = wizard.getChosenProtocol();
    if ( !protocol_name.isEmpty() )
    {
      m_plugin_system.removeLoginWidgetByName(protocol_name);
    }
  }
}

void AbstractLayer::openSettingsDialog()
{
  qutimSettings *settingsDialog = new qutimSettings(m_current_profile, m_parent);
  m_parent->addTab(settingsDialog, m_parent->tr("Settings"));
//  m_parent->addTab(settingsDialog, QIcon("icons/crystal_project/settings.png"),"");
  m_parent->setCurrentWidget(settingsDialog);

  QObject::connect ( settingsDialog, SIGNAL(destroyed()),
                     m_parent, SLOT(destroySettings()) );
  QObject::connect(m_parent, SIGNAL(updateTranslation()),
                   settingsDialog, SLOT(onUpdateTranslation()));
}

void AbstractLayer::initializePointers(QTreeView *contact_list_view, QHBoxLayout *account_button_layout,
                                       QMenu *tray_menu, QAction *action_before)
{
  m_contact_list_management.setTreeView(contact_list_view);
  m_plugin_system.addAccountButtonsToLayout(account_button_layout);
  m_tray_menu = tray_menu;
  m_action_tray_menu_before = action_before;
  if ( m_is_new_profile )
    createNewAccount();
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.restoreData();
  m_plugin_system.pointersAreInitialized();
}

void AbstractLayer::clearMenuFromStatuses()
{
  if ( m_account_status_menu_list.count() )
  {
    foreach(QMenu *account_status_menu, m_account_status_menu_list)
    {
      m_tray_menu->removeAction(account_status_menu->menuAction());
    }
  }
}

void AbstractLayer::addStatusesToMenu()
{
  clearMenuFromStatuses();
  m_account_status_menu_list = m_plugin_system.addStatusMenuToTrayMenu();
  if ( m_account_status_menu_list.count() )
  {
    foreach(QMenu *account_status_menu, m_account_status_menu_list)
    {
      if ( m_action_tray_menu_before && account_status_menu )
        m_tray_menu->insertMenu(m_action_tray_menu_before, account_status_menu);
    }
  }
}

void AbstractLayer::reloadGeneralSettings()
{
  m_parent->reloadGeneralSettings();
}

void AbstractLayer::addAccountMenusToTrayMenu(bool add)
{
  if ( add )
  {
    addStatusesToMenu();
  }
  else
  {
    clearMenuFromStatuses();
  }
  m_show_account_menus = add;
}

void AbstractLayer::updateStausMenusInTrayMenu()
{
  m_account_status_menu_list.clear();
  addAccountMenusToTrayMenu(m_show_account_menus);
}

void AbstractLayer::setAutoAway()
{
  m_plugin_system.setAutoAway();
}

void AbstractLayer::setStatusAfterAutoAway()
{
  m_plugin_system.setStatusAfterAutoAway();
}

qutIM *AbstractLayer::getParent()
{
  return m_parent;
}

void AbstractLayer::addActionToMainMenu(QAction *action)
{
  m_parent->addActionToList(action);
}
