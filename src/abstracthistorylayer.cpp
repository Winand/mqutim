/*
    Abstract History Layer

    Copyright (c) 2008 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "abstracthistorylayer.h"
#include "abstractchatlayer.h"
#include <QSettings>

AbstractHistoryLayer::AbstractHistoryLayer()
{
  m_engine = new HistoryEngine();
  m_engine->setHistoryPath(QString("history"));
}

AbstractHistoryLayer::~AbstractHistoryLayer()
{
  delete m_engine;
}
AbstractHistoryLayer &AbstractHistoryLayer::instance()
{
  static AbstractHistoryLayer ahl;
  return ahl;
}
void AbstractHistoryLayer::loadProfile(const QString &profile_name)
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+profile_name, "profilesettings");
  m_engine->setHistoryPath(QFileInfo(settings.fileName()).absolutePath()+"/history/");
  m_profile_name = profile_name;
  loadSettings();
}

void AbstractHistoryLayer::loadSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("history");
  m_save_history = settings.value("save", true).toBool();
  m_show_recent_messages = settings.value("showrecent", true).toBool();
  m_recent_messages_count = settings.value("recentcount", 4).toUInt();
  settings.endGroup();
}

bool AbstractHistoryLayer::saveHistoryMessage(const HistoryItem &item)
{
  if ( m_save_history )
  {
    QString identification = QString("%1.%2.%3").arg(item.m_user.m_protocol_name)
                             .arg(item.m_user.m_account_name).arg(item.m_user.m_item_name);
    if (m_contact_history.contains(identification))
    {
      HistoryItem new_item = item;
      new_item.m_user.m_item_name = m_contact_history[identification];
      return m_engine->saveHistoryMessage(new_item);
    }
    else
      return m_engine->saveHistoryMessage(item);
  }
  return false;
}
bool AbstractHistoryLayer::saveSystemMessage(const HistoryItem &item)
{
  return m_engine->saveSystemMessage(item);
}
void AbstractHistoryLayer::loadHistoryMessage(const TreeModelItem &item, const QDateTime &date_last)
{
  if ( m_show_recent_messages )
  {
    QList<HistoryItem> list;
    QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                             .arg(item.m_account_name).arg(item.m_item_name);
    if (m_contact_history.contains(identification))
    {
      TreeModelItem new_item = item;
      new_item.m_item_name = m_contact_history[identification];
      list = m_engine->loadHistoryMessage(new_item,m_recent_messages_count, date_last);
    }
    else
    {
      list = m_engine->loadHistoryMessage(item,m_recent_messages_count, date_last);
    }
    AbstractChatLayer &acl = AbstractChatLayer::instance();
    foreach(HistoryItem history, list)
    acl.addMessage(item, history.m_message, history.m_in, history.m_time, false, true, true);
    /*if ( list.count() )
    	acl.addSeparator(item);*/
  }
}

void AbstractHistoryLayer::closeHistoyWindow(const QString &identification)
{
  m_user_history_list.remove(identification);
}

void AbstractHistoryLayer::openHistoryWindow(const TreeModelItem &item)
{
  QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                           .arg(item.m_account_name).arg(item.m_item_name);
  if ( !m_user_history_list.contains(identification) )
  {
    HistoryWindow *win = new HistoryWindow(item.m_protocol_name,
                                           item.m_account_name,
                                           item.m_item_name,
                                           m_profile_name);
    m_user_history_list.insert(identification, win);
    win->show();
  }
}

void AbstractHistoryLayer::setContactHistoryName(const TreeModelItem &item)
{
  QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                           .arg(item.m_account_name).arg(item.m_item_name);
  if (item.m_item_history.isEmpty())
    m_contact_history.remove(identification);
  else
    m_contact_history.insert(identification,item.m_item_history);
}

QString AbstractHistoryLayer::getContactHistoryName(const TreeModelItem &item)
{
  QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                           .arg(item.m_account_name).arg(item.m_item_name);
  return m_contact_history.value(identification, item.m_item_name);

}
