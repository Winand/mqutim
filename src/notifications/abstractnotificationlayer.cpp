/*
    AbstractNotificationLayer

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

#include "abstractnotificationlayer.h"
#include "../abstractlayer.h"
#include "../pluginsystem.h"
#include "../abstractsoundlayer.h"
#include <QSettings>
#include <QFile>
#include <QDebug>

AbstractNotificationLayer::AbstractNotificationLayer()
{
}

AbstractNotificationLayer::~AbstractNotificationLayer()
{
}

AbstractNotificationLayer &AbstractNotificationLayer::instance()
{
  static AbstractNotificationLayer anl;
  return anl;
}

void AbstractNotificationLayer::loadProfile(const QString &profile_name)
{
  m_profile_name = profile_name;
  loadSettings();
}

void AbstractNotificationLayer::loadSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("notifications");
  m_show_popup = settings.value("popup", true).toBool();

  m_show_signon = settings.value("signon", true).toBool();
  m_show_signoff = settings.value("signoff", true).toBool();
  m_show_typing = settings.value("typing", true).toBool();
  m_show_change_status = settings.value("statuschange", true).toBool();
  m_show_message = settings.value("message", true).toBool();

  settings.endGroup();
}

void AbstractNotificationLayer::systemMessage(const TreeModelItem &item, const QString &message)
{
  if (!m_show_popup)
    return;
  qDebug() << "System popup from %username%:" << message;
}

void AbstractNotificationLayer::userMessage(const TreeModelItem &item, const QString &message, NotificationType type)
{
  if (!m_show_popup)
    return;

  PluginSystem::instance().userNotification(item, message, type);
  QString trayMessageMsg;
  QString msg;
  //read contact and account info
  PluginSystem &ps = PluginSystem::instance();

  QStringList contact_info = ps.getAdditionalInfoAboutContact(item);
  TreeModelItem account_item = item;

  account_item.m_item_name = item.m_account_name;
  QStringList account_info = ps.getAdditionalInfoAboutContact(account_item);

  QString contact_nick = item.m_item_name;
  QString contact_avatar;
  QString account_nick = item.m_account_name;
  QString account_avatar;

  if ( contact_info.count() > 0)
  {
    contact_nick = contact_info.at(0);
  }
  if ( contact_info.count() > 1 )
  {
    contact_avatar= contact_info.at(1);
  }

  if ( account_info.count() > 0)
  {
    account_nick = account_info.at(0);
  }
  if ( account_info.count() > 1 )
  {
    account_avatar = account_info.at(1);
  }

  bool show_message = false;
  switch ( type )
  {
  case NotifyStatusChange:
    msg = contact_nick + " " + message;
    trayMessageMsg =  message;
    show_message = m_show_change_status;
    break;
  case NotifyMessageGet:
    msg = QObject::tr("Message from %1:\n%2").arg(contact_nick).arg(message);
    trayMessageMsg = message;
    show_message = m_show_message;
    break;
  case NotifyTyping:
    msg = QObject::tr("%1 is typing").arg(contact_nick);
    trayMessageMsg = QObject::tr("typing");
    show_message = m_show_typing;
    break;
  case NotifyBlockedMessage:
    msg = QObject::tr("Blocked message from %1:\n%2").arg(contact_nick).arg(message);
    trayMessageMsg = QObject::tr("(BLOCKED)\n") + message;
    show_message = true;
    break;
  case NotifyBirthday:
    msg = QObject::tr("%1 has birthday today!!").arg(contact_nick);
    trayMessageMsg = QObject::tr("has birthday today!!");
    show_message = true;
    break;
  case NotifyCustom:
    msg = contact_nick + "(" + item.m_item_name + ") " + message;
    trayMessageMsg =  message;
    show_message = true;
    break;
  case NotifyOnline:
    msg = contact_nick + " " + message;
    trayMessageMsg =  message;
    show_message = m_show_signon;
    break;
  case NotifyOffline:
    msg = contact_nick + " " + message;
    trayMessageMsg =  message;
    show_message = m_show_signoff;
    break;
  default:
    msg = contact_nick + " " + message;
    trayMessageMsg =  message;
    show_message = true;
  }

  if ( show_message ) // Show popup
  {
    qDebug() << "User popup from %username%:" << msg << message;
  }
}

