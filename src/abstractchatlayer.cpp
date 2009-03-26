/*
    AbstractChatLayer

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
#include "abstractchatlayer.h"
#include "abstractlayer.h"
#include "abstracthistorylayer.h"
#include "abstractcontactlist.h"
#include "abstractsoundlayer.h"
#include "notifications/abstractnotificationlayer.h"
#include "chatwindow/separateconference.h"
#include "abstractcontextlayer.h"
#include "qutim.h"

#include "chatsession_qtopia.h"

AbstractChatLayer::AbstractChatLayer():
    m_plugin_system(PluginSystem::instance()),
    dispatcher(qApp)
{
  dispatcher.setChatSessionFactory(new QtopiaChatSessionFactory(&dispatcher));
}

AbstractChatLayer::~AbstractChatLayer()
{
}

AbstractChatLayer &AbstractChatLayer::instance()
{
  static AbstractChatLayer acl;
  return acl;
}

void AbstractChatLayer::createChat(const TreeModelItem &item, bool new_chat)
{
  ChatSession *session = dispatcher.sessionFor(item, ChatSession::Private);
  if (session)
    session->activate();
}

void AbstractChatLayer::addMessage(const TreeModelItem &item, const QString &message, bool in,
                                   QDateTime message_date, bool save_history, bool unreaded_message, bool history,
                                   const QString &not_modified)
{
  if (in)
  {
    Message msg;
    msg.setType(Message::IncomingDialog);
    msg.setAuthor(item);
    msg.setTime(message_date);
    msg.setText(message);
    msg.reg();
    dispatcher.dispatch(msg);
  }
  else
    qWarning() << "Message OUT";
}

void AbstractChatLayer::sendMessageTo(const QString &protocol_name, const QString &account_name,
                                      const QString &contact_name, const QString &message, int message_icon_position, bool from_plugin)
{

}

void AbstractChatLayer::addServiceMessage(const TreeModelItem &item, const QString &message)
{

}

void AbstractChatLayer::sendTypingNotification(const QString &protocol_name, const QString &account_name, const QString &contact_name,
    int notification_type)
{
  TreeModelItem contact_item;
  contact_item.m_protocol_name = protocol_name;
  contact_item.m_account_name = account_name;
  contact_item.m_item_name = contact_name;
  contact_item.m_parent_name = "";
  contact_item.m_item_type = TreeModelItem::Buddy;
  
  m_plugin_system.sendTypingNotification(contact_item, notification_type);
}

void AbstractChatLayer::contactTyping(const TreeModelItem &item, bool typing)
{
  Message msg;
  msg.setAuthor(item);
  msg.setType(typing?Message::TypingStarted:Message::TypingStopped);
  msg.reg();
  dispatcher.dispatch(msg);
}

void AbstractChatLayer::messageDelievered(const TreeModelItem &item, int message_position)
{
}

void AbstractChatLayer::windowActivated(const QString &protocol_name, const QString &account_name,
                                        const QString &item_name)
{
  TreeModelItem contact_item;
  contact_item.m_protocol_name = protocol_name;
  contact_item.m_account_name = account_name;
  contact_item.m_item_name = item_name;
  contact_item.m_parent_name = "";
  contact_item.m_item_type = TreeModelItem::Buddy;
  ChatSession *session = dispatcher.sessionFor(contact_item, ChatSession::Private);
  if (session)
    session->activate();
}

void AbstractChatLayer::askForContactMenu(const QString &protocol_name,
    const QString &account_name,
    const QString &item_name,
    const QPoint &menu_point)
{
  TreeModelItem contact_item;
  contact_item.m_protocol_name = protocol_name;
  contact_item.m_account_name = account_name;
  contact_item.m_item_name = item_name;
  contact_item.m_parent_name = "";
  contact_item.m_item_type = TreeModelItem::Buddy;

  AbstractContextLayer::instance().itemContextMenu(contact_item, menu_point);
}

bool AbstractChatLayer::createConference(const QString &protocol_name,
    const QString &conference_name, const QString &account_name)
{
  return false;
}

void AbstractChatLayer::addMessageToConference(const QString &protocol_name,
    const QString &conference_name, const QString &account_name,
    const QString &from, const QString &message, const QDateTime &date, bool history)
{
}

void AbstractChatLayer::sendMessageToConference(
  const QString &protocol_name,
  const QString &conference_name, const QString &account_name,
  const QString &message)
{
}

void AbstractChatLayer::changeOwnConferenceNickName(const QString &protocol_name,
    const QString &conference_name,
    const QString &account_name,
    const QString &nickname)
{
}


void AbstractChatLayer::setConferenceTopic(const QString &protocol_name, const QString &conference_name,
    const QString &account_name, const QString &topic)
{
}

void AbstractChatLayer::addSystemMessageToConference(const QString &protocol_name,
    const QString &conference_name, const QString &account_name,
    const QString &message, const QDateTime &date, bool history)
{
}

bool AbstractChatLayer::addConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname)
{
  return false;
}

bool AbstractChatLayer::removeConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname)
{
  return false;
}

bool AbstractChatLayer::renameConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QString &new_nickname)
{
  return false;
}

bool AbstractChatLayer::setConferenceItemStatus(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, const QString &status, int mass)
{
  return false;
}

bool AbstractChatLayer::setConferenceItemIcon(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname,
    const QIcon &icon, int position)
{
  return true;
}

bool AbstractChatLayer::setConferenceItemRole(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, const QString &role, int mass)
{
  return true;
}

void AbstractChatLayer::addSeparator(const TreeModelItem &item)
{
}

void AbstractChatLayer::sendMessageFromPlugin(const TreeModelItem &item, const QString &message)
{
}

void AbstractChatLayer::showConferenceTopicSettings(const QString &protocol_name, const QString &account_name, const QString &conference)
{
}

QStringList AbstractChatLayer::getConferenceItemsList(const QString &protocol_name, const QString &conference_name, const QString &account_name)
{
  return QStringList();
}

bool AbstractChatLayer::changeChatWindowID(const TreeModelItem &item, const QString &id)
{
  return false;
}
