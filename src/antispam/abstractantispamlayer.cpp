/*
    AbstractAntiSpamLayer

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

#include "abstractantispamlayer.h"
#include <QSettings>
#include <QDebug>
#include <QObject>
#include "../pluginsystem.h"
#include "../notifications/abstractnotificationlayer.h"

AbstractAntiSpamLayer::AbstractAntiSpamLayer()
{
}

AbstractAntiSpamLayer::~AbstractAntiSpamLayer()
{
}

AbstractAntiSpamLayer &AbstractAntiSpamLayer::instance()
{
  static AbstractAntiSpamLayer aasl;
  return aasl;
}

void AbstractAntiSpamLayer::loadProfile(const QString &profile_name)
{
  m_profile_name = profile_name;
  loadSettings();
}

void AbstractAntiSpamLayer::loadSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("antispam");
  m_accept_from_cl_only = settings.value("fromcl", false).toBool();
  m_notify_on_block = settings.value("notify", true).toBool();
  m_block_authorization_from_nil = settings.value("authorization", false).toBool();
  m_block_nil_urls = settings.value("urls", false).toBool();
  m_anti_spam_bot_enabled = settings.value("botenabled", false).toBool();
  m_bot_question = settings.value("question", "").toString();
  m_bot_answer = settings.value("answer", "").toString();
  m_bot_after_answer = settings.value("afteranswer", "").toString();
  m_do_not_answer_if_invisible = settings.value("oninvisible", false).toBool();
  settings.endGroup();
}

bool AbstractAntiSpamLayer::checkForMessageValidation(const TreeModelItem &item,
    const QString &message,int message_type,bool special_status)
{
  if ( m_accept_from_cl_only )
  {
    notifyAboutBlock(item, message);
    return false;
  }
  if ( m_block_authorization_from_nil && message_type == 1 )
  {
    notifyAboutBlock(item, QObject::tr("Authorization blocked"));
    return false;
  }
  if ( m_do_not_answer_if_invisible && special_status )
  {
    notifyAboutBlock(item, message);
    return false;
  }
  if ( m_block_nil_urls && checkForUrls(message) )
  {
    notifyAboutBlock(item, message);
    return false;
  }
  if ( m_anti_spam_bot_enabled )
  {
    if ( message != m_bot_answer )
    {
      QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                               .arg(item.m_account_name).arg(item.m_item_name);
      if ( !m_banned_contacts.contains(identification) )
      {
        notifyAboutBlock(item, message);
        answerToContact(item, m_bot_question);
        m_banned_contacts<<identification;
      }
      return false;
    }
    else
    {
      QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                               .arg(item.m_account_name).arg(item.m_item_name);
      answerToContact(item, m_bot_after_answer);
      m_banned_contacts.removeAll(identification);
      return true;
    }
  }

  return true;
}

void AbstractAntiSpamLayer::notifyAboutBlock(const TreeModelItem &item, const QString &message)
{
  if ( m_notify_on_block )
  {
    AbstractNotificationLayer::instance().userMessage(item, message, NotifyBlockedMessage);
  }
}

void AbstractAntiSpamLayer::answerToContact(const TreeModelItem &item, QString &message)
{
  PluginSystem &ps = PluginSystem::instance();
  ps.sendMessageToContact(item, message, 0);
}

bool AbstractAntiSpamLayer::checkForUrls(const QString &message)
{
  bool containsURLs = false;
  containsURLs = message.contains("http:", Qt::CaseInsensitive) ? true : containsURLs;
  containsURLs = message.contains("ftp:", Qt::CaseInsensitive) ? true : containsURLs;
  containsURLs = message.contains("www.", Qt::CaseInsensitive) ? true : containsURLs;
  return containsURLs;
}
