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
#include "iostream"
#include "abstractchatlayer.h"
#include "abstractlayer.h"
#include "chatwindow/separatechatwindow.h"
#include "abstracthistorylayer.h"
#include "abstractcontactlist.h"
#include <QtXml>
#include "abstractsoundlayer.h"
#include "notifications/abstractnotificationlayer.h"
#include "chatwindow/separateconference.h"
#include "abstractcontextlayer.h"
#include "qutim.h"

AbstractChatLayer::AbstractChatLayer():
    m_plugin_system(PluginSystem::instance())
{
  m_remove_messages_after = false;
  m_remove_count = 0;
  m_open_on_new = false;
  m_close_after_send = false;
  m_dont_show_tray_event = false;
  m_dont_show_content_in_notifications = false;
}

AbstractChatLayer::~AbstractChatLayer()
{
  saveOpenedChatWindows();
  saveUnreadedMessages();
}

AbstractChatLayer &AbstractChatLayer::instance()
{
  static AbstractChatLayer acl;
  return acl;
}

void AbstractChatLayer::loadProfile(const QString &profile_name)
{
  m_profile_name = profile_name;
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  m_tmp_image_path = settings.fileName().section('/', 0, -2) + "/messageimages/";
  QDir image_dir(m_tmp_image_path);
  if ( !image_dir.exists() )
  {
    image_dir.mkdir(m_tmp_image_path);
  }
  loadSettings();
}
void AbstractChatLayer::createChat(const TreeModelItem &item, bool new_chat)
{
  if ( item.m_item_type==TreeModelItem::Buddy )
  {
    QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                             .arg(item.m_account_name).arg(item.m_item_name);
    qDebug("createChat:!m_tabbed_mode");
    if ( !m_separate_window_list.contains(identification) )
    {
      qDebug("======= message ========");
      qDebug() << "Protocol:" << item.m_protocol_name;
      qDebug() << "From:" << item.m_item_name;
      qDebug() << "To:" << item.m_account_name;
      qDebug("===== end message ======");
      qutIM *qutim = qutIM::getInstance();
      SeparateChatWindow *win = new SeparateChatWindow(item.m_protocol_name, item.m_account_name, item.m_item_name, qutim);
      m_separate_window_list.insert(identification, win);
      win->setOptions(m_remove_messages_after, m_remove_count, m_close_after_send,
                      m_show_names, m_send_on_enter, m_send_typing_notifications);
      AbstractContactList &acl = AbstractContactList::instance();
      QIcon icon = acl.getItemStatusIcon(item);
      if ( !icon.isNull() )
        win->setWindowIcon(icon);
      PluginSystem::instance().chatWindowOpened(item);
      qutim->addTab(win, item.m_item_name);
      qutim->setCurrentWidget(win);
      QDateTime last_time = QDateTime::currentDateTime();
      foreach(UnreadedMessage unreaded_message, m_unreaded_messages_list)
      {
        if ( item.m_protocol_name == unreaded_message.m_protocol_name &&
             item.m_account_name == unreaded_message.m_account_name &&
             item.m_item_name == unreaded_message.m_contact_name)
        {
          if (unreaded_message.m_message_time<last_time)
            last_time = unreaded_message.m_message_time;
        }
      }
      AbstractHistoryLayer &ahl = AbstractHistoryLayer::instance();
      ahl.loadHistoryMessage(item,last_time);
      PluginSystem::instance().chatWindowAboutToBeOpened(item);
    }
    else if ( !new_chat )
    {
      SeparateChatWindow *win = m_separate_window_list.value(identification);
      win->windowActivatedByUser();
    }
    AbstractContactList &acl = AbstractContactList::instance();
    if ( acl.getItemHasUnviewedContent(item) ||
         !acl.itemExists(item))
    {
      readUnreaded(item);
    }
  }
}

void AbstractChatLayer::removeSeparateWindowFromList(const QString &identification)
{
  m_separate_window_list.remove(identification);
}

void AbstractChatLayer::removeSeparateConferenceFromList(const QString &identification)
{
  m_separate_conference_list.remove(identification);
}

void AbstractChatLayer::addMessage(const TreeModelItem &item, const QString &message, bool in,
                                   QDateTime message_date, bool save_history, bool unreaded_message, bool history,
                                   const QString &not_modified)
{
  if ( item.m_item_type==TreeModelItem::Buddy )
  {
    if (save_history)
    {
      HistoryItem history_item;
      history_item.m_message = message;
      history_item.m_time = message_date;
      history_item.m_user = item;
      history_item.m_in = in;
      history_item.m_type = 1;
      AbstractHistoryLayer &ahl = AbstractHistoryLayer::instance();
      ahl.saveHistoryMessage(history_item);
    }

    if ( m_open_on_new )
    {
      createChat(item, true);
    }
    bool can_not_read_it_now = false;
    QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                             .arg(item.m_account_name).arg(item.m_item_name);
    if ( m_separate_window_list.contains(identification) )
    {
      SeparateChatWindow *win = m_separate_window_list.value(identification);
      win->addMessage(message, in, message_date, history);
      if ( !win->isActiveWindow() || win->isMinimized())
      {
        m_waiting_for_activation_list.append(item);
        if ( !m_dont_show_tray_event && in && !unreaded_message)
        {

          if ( m_dont_show_content_in_notifications )
            AbstractNotificationLayer::instance().userMessage(item,
                QObject::tr("Incoming message"), NotifyMessageGet);
          else
            AbstractNotificationLayer::instance().userMessage(item,
                not_modified.isEmpty()?message:not_modified, NotifyMessageGet);
        }
        AbstractContactList::instance().setItemHasUnviewedContent(item ,true);
      }
      if ( in && !unreaded_message && AbstractSoundLayer::instance().isActiveWindowIncomeSound() )
      {
        AbstractSoundLayer::instance().playSound(NotifyMessageGet);
      }
    }
    else
      can_not_read_it_now = true;

    if ( can_not_read_it_now && in && !unreaded_message)
    {
      UnreadedMessage new_unreaded_message;
      new_unreaded_message.m_protocol_name = item.m_protocol_name;
      new_unreaded_message.m_account_name = item.m_account_name;
      new_unreaded_message.m_contact_name = item.m_item_name;
      new_unreaded_message.m_message = message;
      new_unreaded_message.m_message_time = message_date;
      m_unreaded_messages_list.append(new_unreaded_message);
      if ( !m_unreaded_contacts.contains(identification) )
        m_unreaded_contacts.insert(identification, item);
      if ( m_dont_show_content_in_notifications )
        AbstractNotificationLayer::instance().userMessage(item,
            QObject::tr("Incoming message"), NotifyMessageGet);
      else
        AbstractNotificationLayer::instance().userMessage(item,
            not_modified.isEmpty()?message:not_modified, NotifyMessageGet);
      AbstractContactList::instance().setItemHasUnviewedContent(item, true);
      AbstractSoundLayer::instance().playSound(NotifyMessageGet);
    }
  }
}

void AbstractChatLayer::sendMessageTo(const QString &protocol_name, const QString &account_name,
                                      const QString &contact_name, const QString &message, int message_icon_position, bool from_plugin)
{
  TreeModelItem contact_item;
  contact_item.m_protocol_name = protocol_name;
  contact_item.m_account_name = account_name;
  contact_item.m_item_name = contact_name;
  contact_item.m_parent_name = "";
  contact_item.m_item_type = TreeModelItem::Buddy;

  QString tmp_message = message;

  m_plugin_system.sendMessageBeforeShowing(contact_item, tmp_message);
  m_plugin_system.sendMessageToContact(contact_item, tmp_message, message_icon_position);
  QString identification = QString("%1.%2.%3").arg(protocol_name)
                           .arg(account_name).arg(contact_name);

  if (m_separate_window_list.contains(identification))
  {
    addModifiedMessage(contact_item, tmp_message, false, QDateTime::currentDateTime());
    AbstractSoundLayer::instance().playSound(NotifyMessageSend);
  }
  else if (from_plugin)
  {

    HistoryItem history_item;
    history_item.m_message = tmp_message;
    history_item.m_time = QDateTime::currentDateTime();
    history_item.m_user = contact_item;
    history_item.m_in = false;
    history_item.m_type = 1;
    AbstractHistoryLayer::instance().saveHistoryMessage(history_item);
  }
}

void AbstractChatLayer::addServiceMessage(const TreeModelItem &item, const QString &message)
{
  if ( item.m_item_type==TreeModelItem::Buddy )
  {
    QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                             .arg(item.m_account_name).arg(item.m_item_name);
    if ( m_separate_window_list.contains(identification) )
    {
      SeparateChatWindow *win = m_separate_window_list.value(identification);
      win->addServiceMessage(message);
    }
  }
}

void AbstractChatLayer::readAllUnreadedMessages()
{
  if ( m_open_all_after_click_on_tray )
  {
    foreach(UnreadedMessage unreaded_message, m_unreaded_messages_list)
    {
      TreeModelItem tmp_model_item;
      tmp_model_item.m_protocol_name = unreaded_message.m_protocol_name;
      tmp_model_item.m_account_name = unreaded_message.m_account_name;
      tmp_model_item.m_item_name = unreaded_message.m_contact_name;
      tmp_model_item.m_item_type = TreeModelItem::Buddy;
      createChat(tmp_model_item);
      //		addMessage(tmp_model_item, unreaded_message.m_message, true,
      //				unreaded_message.m_message_time, false, true);
    }
    foreach(TreeModelItem item, m_waiting_for_activation_list)
    {
      createChat(item);
      AbstractContactList::instance().setItemHasUnviewedContent(item, false);
    }
    m_waiting_for_activation_list.clear();
    m_unreaded_messages_list.clear();
    m_unreaded_contacts.clear();
  }
  else
  {
    if ( m_unreaded_messages_list.count() > 0 && m_unreaded_contacts.count() > 0)
    {
      UnreadedMessage unr_msg = m_unreaded_messages_list.at(0);
      QString identification = QString("%1.%2.%3").arg(unr_msg.m_protocol_name)
                               .arg(unr_msg.m_account_name).arg(unr_msg.m_contact_name);
      TreeModelItem item = m_unreaded_contacts.value(identification);
      createChat(item);
      readUnreaded(item);
    }
    else if ( m_waiting_for_activation_list.count() )
    {
      TreeModelItem item = m_waiting_for_activation_list.at(0);
      createChat(item);
      AbstractContactList::instance().setItemHasUnviewedContent(item, false);
      if ( !m_waiting_for_activation_list.count() &&
           !m_unreaded_messages_list.count() &&
           !m_unreaded_contacts.count() )
      {
      }
    }
  }
}

void AbstractChatLayer::readUnreaded(const TreeModelItem &item)
{
  int index = 0;
  foreach(UnreadedMessage unreaded_message, m_unreaded_messages_list)
  {
    if ( item.m_protocol_name == unreaded_message.m_protocol_name &&
         item.m_account_name == unreaded_message.m_account_name &&
         item.m_item_name == unreaded_message.m_contact_name)
    {
      AbstractContactList::instance().setItemHasUnviewedContent(item , false);
      addMessage(item, unreaded_message.m_message, true,
                 unreaded_message.m_message_time, false, true);
      m_unreaded_messages_list.removeAt(index);
      index--;
    }
    index++;
  }

  index = 0;
  foreach(TreeModelItem act_item, m_waiting_for_activation_list)
  {
    if ( item.m_protocol_name == act_item.m_protocol_name &&
         item.m_account_name == act_item.m_account_name &&
         item.m_item_name == act_item.m_item_name)
    {
      m_waiting_for_activation_list.removeAt(index);
      index--;
    }
    index++;
  }

  QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                           .arg(item.m_account_name).arg(item.m_item_name);
  m_unreaded_contacts.remove(identification);

  if ( !m_waiting_for_activation_list.count() &&
       !m_unreaded_messages_list.count() &&
       !m_unreaded_contacts.count() )
  {
  }
}

void AbstractChatLayer::showContactInformation(const QString &protocol_name, const QString &account_name, const QString &contact_name)
{
  TreeModelItem contact_item;
  contact_item.m_protocol_name = protocol_name;
  contact_item.m_account_name = account_name;
  contact_item.m_item_name = contact_name;
  contact_item.m_parent_name = "";
  contact_item.m_item_type = TreeModelItem::Buddy;
  m_plugin_system.showContactInformation(contact_item);
}

void AbstractChatLayer::sendImageTo(const QString &protocol_name, const QString &account_name, const QString &item_name,
                                    const QByteArray &image_raw)
{
  /*	TreeModelItem contact_item;
  	contact_item.m_protocol_name = protocol_name;
  	contact_item.m_account_name = account_name;
  	contact_item.m_item_name = item_name;
  	contact_item.m_parent_name = "";
  	contact_item.m_item_type = TreeModelItem::Buddy;
  	m_plugin_system.sendImageTo(contact_item, image_raw);
          addImage(contact_item, image_raw, false);*/
}

void AbstractChatLayer::addImage(const TreeModelItem &item, const QByteArray &image_raw, bool in)
{
  QCryptographicHash hash(QCryptographicHash::Md5);
  hash.addData(image_raw);
  QString path_to_new_picture = m_tmp_image_path + hash.result().toHex();
  if ( !QFile::exists(path_to_new_picture) )
  {
    QFile pic(path_to_new_picture);
    if ( pic.open(QIODevice::WriteOnly) )
    {
      pic.write(image_raw);
      pic.close();
    }
  }
  addMessage(item,QString("<img src='%1'>").arg(path_to_new_picture),in, QDateTime::currentDateTime());
}

void AbstractChatLayer::sendFileTo(const QString &protocol_name, const QString &account_name, const QString &contact_name)
{
  /*      QFileDialog dialog(0,QObject::tr("Open File"),"",QObject::tr("All files (*)"));
          dialog.setFileMode(QFileDialog::ExistingFiles);
          dialog.setAttribute(Qt::WA_QuitOnClose, false);
          QStringList file_names;
          if ( !dialog.exec() )
                  return;
          file_names = dialog.selectedFiles();

          TreeModelItem contact_item;
  	contact_item.m_protocol_name = protocol_name;
  	contact_item.m_account_name = account_name;
  	contact_item.m_item_name = contact_name;
  	contact_item.m_parent_name = "";
  	contact_item.m_item_type = TreeModelItem::Buddy;
          m_plugin_system.sendFileTo(contact_item, file_names);*/
}

void AbstractChatLayer::loadSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("chatwindow");
  m_remember_tabs_on_closing= settings.value("remember", false).toBool();
  m_open_all_after_click_on_tray = settings.value("openall", false).toBool();
  m_close_after_send = settings.value("close", false).toBool();
  m_open_on_new = settings.value("open", false).toBool();
  /*	m_webkit_mode = settings.value("webkit", false).toBool();*/
  m_do_not_show_if_open = settings.value("events", false).toBool();
  m_send_on_enter = settings.value("onenter", false).toBool();
  m_send_typing_notifications = settings.value("typing", true).toBool();
  m_dont_show_tray_event = settings.value("trayevents", false).toBool();
  m_remove_messages_after = settings.value("remove", true).toBool();
  m_show_names = settings.value("names", true).toBool();
  m_remove_count = settings.value("removecount", 60).toUInt();
  m_timestamp_type = settings.value("timestamp", 1).toUInt();
  m_dont_show_content_in_notifications = settings.value("content", false).toBool();
  settings.endGroup();
  settings.beginGroup("gui");
  m_emoticons_path = settings.value("emoticons","").toString();
  m_chat_form_path = settings.value("chat","").toString();

  settings.endGroup();
}

QString AbstractChatLayer::getTimeStamp(const QDateTime &message_date)
{
  switch ( m_timestamp_type )
  {
  case 0:
    return message_date.toString("hh:mm:ss dd/MM/yyyy");
  case 1:
    return message_date.toString("hh:mm:ss");
  case 2:
    return message_date.toString("hh:mm ddd/MMM/yyyy");
  default:
    return message_date.toString("hh:mm:ss");
  }
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
  if ( item.m_item_type==TreeModelItem::Buddy )
  {
    QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                             .arg(item.m_account_name).arg(item.m_item_name);
    if ( m_separate_window_list.contains(identification) )
    {
      SeparateChatWindow *win = m_separate_window_list.value(identification);
      win->contactTyping(typing);
    }
    else if ( typing )
      AbstractNotificationLayer::instance().userMessage(item, "", NotifyTyping);
  }
}

void AbstractChatLayer::messageDelievered(const TreeModelItem &item, int message_position)
{
  if ( item.m_item_type==TreeModelItem::Buddy )
  {
    QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                             .arg(item.m_account_name).arg(item.m_item_name);
    if ( m_separate_window_list.contains(identification) )
    {
      SeparateChatWindow *win = m_separate_window_list.value(identification);
      win->messageDelievered(message_position);
    }
  }
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
//	m_plugin_system.sendTypingNotification(contact_item, notification_type);
  int i = 0;
  foreach( TreeModelItem item, m_waiting_for_activation_list )
  {
    if ( item.m_protocol_name == protocol_name
         && item.m_account_name == account_name
         && item.m_item_name == item_name )
    {
      m_waiting_for_activation_list.removeAt(i);
      AbstractContactList::instance().setItemHasUnviewedContent(item, false);
      i--;
    }
    i++;
  }
  if ( !m_waiting_for_activation_list.count() &&
       !m_unreaded_messages_list.count() )
  {
    AbstractLayer &al = AbstractLayer::instance();
  }
}

void AbstractChatLayer::saveOpenedChatWindows()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  QString windows_file = settings.fileName().section('/', 0, -2) + "/owindows.list";
  QFile w_file(windows_file);

  if ( w_file.open(QIODevice::WriteOnly) )
  {
    QDataStream out(&w_file);
    foreach( SeparateChatWindow *win, m_separate_window_list.values())
    {
      out<<win->getProtocolName()
      <<win->getAccountName()
      <<win->getItemName();
    }
    w_file.close();
  }
}

void AbstractChatLayer::restoreOpenedWindows()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  QString windows_file = settings.fileName().section('/', 0, -2) + "/owindows.list";
  QFile w_file(windows_file);
  if ( w_file.open(QIODevice::ReadOnly) )
  {
    QDataStream in(&w_file);
    while ( !in.atEnd() )
    {
      QString p,a,i;
      in>>p>>a>>i;
      TreeModelItem contact_item;
      contact_item.m_protocol_name = p;
      contact_item.m_account_name = a;
      contact_item.m_item_name = i;
      contact_item.m_parent_name = "";
      contact_item.m_item_type = TreeModelItem::Buddy;
      createChat(contact_item);
    }
    w_file.close();
    w_file.remove();
  }
}

void AbstractChatLayer::restoreData()
{
  if ( !m_remember_tabs_on_closing )
    restoreOpenedWindows();
  restoreUnreadedMessages();
}

void AbstractChatLayer::saveUnreadedMessages()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  QString unread_file = settings.fileName().section('/', 0, -2) + "/unreaded.list";
  QFile u_file(unread_file);

  if ( u_file.open(QIODevice::WriteOnly) )
  {
    QDataStream out(&u_file);
    foreach(UnreadedMessage unreaded_message, m_unreaded_messages_list)
    {
      out<<unreaded_message.m_protocol_name
      <<unreaded_message.m_account_name
      <<unreaded_message.m_contact_name
      <<unreaded_message.m_message
      <<unreaded_message.m_message_time;
    }
    u_file.close();
  }
}

void AbstractChatLayer::restoreUnreadedMessages()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  QString unread_file = settings.fileName().section('/', 0, -2) + "/unreaded.list";
  QFile u_file(unread_file);

  if ( u_file.open(QIODevice::ReadOnly) )
  {
    QDataStream in(&u_file);
    while ( !in.atEnd() )
    {
      QString p,a,i,m;
      QDateTime time;
      in>>p>>a>>i>>m>>time;
      TreeModelItem contact_item;
      contact_item.m_protocol_name = p;
      contact_item.m_account_name = a;
      contact_item.m_item_name = i;
      contact_item.m_parent_name = "";
      contact_item.m_item_type = TreeModelItem::Buddy;
      addMessage(contact_item, m, true, time);
    }
    u_file.close();
    u_file.remove();
  }
}

void AbstractChatLayer::reloadContent()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("gui");
  m_emoticons_path = settings.value("emoticons","").toString();
  m_chat_form_path = settings.value("chat","").toString();
  settings.endGroup();
}

void AbstractChatLayer::addItemToActivationList(const QString &protocol_name, const QString &account_name,
    const QString &item_name,const QString &message)
{
  TreeModelItem contact_item;
  contact_item.m_protocol_name = protocol_name;
  contact_item.m_account_name = account_name;
  contact_item.m_item_name = item_name;
  contact_item.m_parent_name = "";
  contact_item.m_item_type = TreeModelItem::Buddy;
  m_waiting_for_activation_list.append(contact_item);
  AbstractContactList::instance().setItemHasUnviewedContent(contact_item, true);
  if ( !m_do_not_show_if_open )
  {
    AbstractNotificationLayer::instance().userMessage(contact_item,message, NotifyMessageGet);
    AbstractSoundLayer::instance().playSound(NotifyMessageGet);
  }
}

void AbstractChatLayer::contactChageStatusIcon(const TreeModelItem &item, const QIcon &icon)
{
  if ( item.m_item_type==TreeModelItem::Buddy )
  {
    QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                             .arg(item.m_account_name).arg(item.m_item_name);
    if ( m_separate_window_list.contains(identification) )
    {
      m_separate_window_list.value(identification)->setWindowIcon(icon);
    }
  }
}

void AbstractChatLayer::contactChangeCLientIcon(const TreeModelItem &item)
{
  if ( item.m_item_type==TreeModelItem::Buddy )
  {
    QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                             .arg(item.m_account_name).arg(item.m_item_name);
    if ( m_separate_window_list.contains(identification) )
    {
      m_separate_window_list.value(identification)->setContactClientIcon();
    }
  }
}

QString AbstractChatLayer::findUrls(const QString &sourceHTML)
{
  QString html = sourceHTML;
  QRegExp linkRegExp("((https?|ftp://|www\\.)[^\\s]+)");
  linkRegExp.setCaseSensitivity(Qt::CaseInsensitive);
  int pos = 0;
  while (((pos = linkRegExp.indexIn(html, pos)) != -1))
  {
    QString link = linkRegExp.cap(0);
    int linkCount = link.count();
    QString tmplink = link;
    if (tmplink.toLower().startsWith("www."))
    {
      tmplink = "http://" + tmplink;
    }
    link = "<a href='" + tmplink + "' target='_blank'>" + link + "</a>";
    html.replace(pos, linkCount, link);
    pos += link.count();
  }
  QRegExp mailRegExp("([a-zA-Z0-9\\-\\_\\.]+@([a-zA-Z0-9\\-\\_]+\\.)+[a-zA-Z]+)");
  pos = 0;
  while (((pos = mailRegExp.indexIn(html, pos)) != -1))
  {
    QString mailAddr = mailRegExp.cap(0);
    int mailAddrCount = mailAddr.count();
    mailAddr = "<a href='mailto:" + mailAddr + "'>" + mailAddr + "</a>";
    if ((pos == 0) || ((pos != 0) && (html[pos - 1] != ':')))
    {
      html.replace(pos, mailAddrCount, mailAddr);
    }
    pos += mailAddr.count();
  }
  return html;
}

void AbstractChatLayer::addModifiedMessage(const TreeModelItem &item, const QString &message, bool in,
    QDateTime message_date, bool save_history,
    bool unreaded_message)
{

  QString modified_message;
  modified_message= findUrls(Qt::escape(message)).replace("\n", "<br>");
  if ( in )
    PluginSystem::instance().receivingMessageBeforeShowing(item, modified_message);

  addMessage(item, modified_message,in,message_date,save_history, unreaded_message, false, message);
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
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
  if ( !m_separate_conference_list.contains(identification) )
  {
    SeparateConference *conf = new SeparateConference(protocol_name,
        conference_name,
        account_name,
        m_emoticons_path);
    m_separate_conference_list.insert(identification, conf);
//    conf->setOptions(m_remove_messages_after, m_remove_count, m_close_after_send,
//        m_show_names, m_send_on_enter);
    conf->setWindowTitle(conference_name);
    conf->setWindowIcon(QIcon(IconManager::instance().getIcon("chat")));
    qutIM *mw = qutIM::getInstance();
    mw->addTab(conf, conference_name);
    mw->setCurrentWidget(conf);
  }
  else
  {
//			SeparateChatWindow *win = m_separate_window_list.value(identification);
//			win->windowActivatedByUser();
  }
  return true;
}

void AbstractChatLayer::addMessageToConference(const QString &protocol_name,
    const QString &conference_name, const QString &account_name,
    const QString &from, const QString &message, const QDateTime &date, bool history)
{
  /*if ( message.indexOf("/me ") == 0)
  {
  	QString serv_message = message;
  	addSystemMessageToConference(protocol_name,conference_name,account_name,from+" "+serv_message.section(' ',1), date, history);
  	return;
  }*/
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
  QString modified_message;
  modified_message= findUrls(Qt::escape(message)).replace("\n", "<br>");
  if ( m_separate_conference_list.contains(identification) )
  {
    m_separate_conference_list.value(identification)->addMessage(from, modified_message,
        getTimeStamp(date.isValid()?date:QDateTime::currentDateTime()), history);
  }
}

void AbstractChatLayer::sendMessageToConference(
  const QString &protocol_name,
  const QString &conference_name, const QString &account_name,
  const QString &message)
{
  PluginSystem::instance().sendMessageToConference(protocol_name, conference_name,
      account_name, message);
}

void AbstractChatLayer::changeOwnConferenceNickName(const QString &protocol_name,
    const QString &conference_name,
    const QString &account_name,
    const QString &nickname)
{
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
  if ( m_separate_conference_list.contains(identification) )
  {
    m_separate_conference_list.value(identification)->setOwnerNickName(nickname);
  }
}


void AbstractChatLayer::setConferenceTopic(const QString &protocol_name, const QString &conference_name,
    const QString &account_name, const QString &topic)
{
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
  if ( m_separate_conference_list.contains(identification) )
  {
//    m_separate_conference_list.value(identification)->setConferenceTopic(topic);
  }
}

void AbstractChatLayer::addSystemMessageToConference(const QString &protocol_name,
    const QString &conference_name, const QString &account_name,
    const QString &message, const QDateTime &date, bool history)
{
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
  if ( m_separate_conference_list.contains(identification) )
  {
    m_separate_conference_list.value(identification)->addServiceMessage(message,
        getTimeStamp(date.isValid()?date:QDateTime::currentDateTime()));
  }
}

bool AbstractChatLayer::addConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname)
{
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
qDebug("AbstractChatLayer::addConferenceItem0");  if ( m_separate_conference_list.contains(identification) )
  {
qDebug("AbstractChatLayer::addConferenceItem1");    m_separate_conference_list.value(identification)->addConferenceItem(nickname);
  }
  return true;
}

bool AbstractChatLayer::removeConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname)
{
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
  if ( m_separate_conference_list.contains(identification) )
  {
    m_separate_conference_list.value(identification)->removeConferenceItem(nickname);
  }
  return true;
}

bool AbstractChatLayer::renameConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QString &new_nickname)
{
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
  if ( m_separate_conference_list.contains(identification) )
  {
    m_separate_conference_list.value(identification)->renameConferenceItem(nickname, new_nickname);
  }
  return true;
}

bool AbstractChatLayer::setConferenceItemStatus(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, const QString &status, int mass)
{
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
  if ( m_separate_conference_list.contains(identification) )
  {
    m_separate_conference_list.value(identification)->setConferenceItemStatus(nickname,icon,status,mass);
  }
  return true;
}

bool AbstractChatLayer::setConferenceItemIcon(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname,
    const QIcon &icon, int position)
{
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
  if ( m_separate_conference_list.contains(identification) )
  {
    m_separate_conference_list.value(identification)->setConferenceItemIcon(nickname, icon, position);
  }
  return true;
}

bool AbstractChatLayer::setConferenceItemRole(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, const QString &role, int mass)
{
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
  if ( m_separate_conference_list.contains(identification) )
  {
    m_separate_conference_list.value(identification)->setConferenceItemRole(nickname,icon,role,mass);
  }
  return true;
}

void AbstractChatLayer::addSeparator(const TreeModelItem &item)
{
  QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                           .arg(item.m_account_name).arg(item.m_item_name);
  if ( m_separate_window_list.contains(identification) )
  {
    SeparateChatWindow *win = m_separate_window_list.value(identification);
    win->addSeparator();
  }
}

void AbstractChatLayer::sendMessageFromPlugin(const TreeModelItem &item, const QString &message)
{
  int cursor_position = 0;
  QString identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                           .arg(item.m_account_name).arg(item.m_item_name);
  if ( m_separate_window_list.contains(identification) )
  {
    cursor_position = m_separate_window_list.value(identification)->getCursorPosition();
  }
  sendMessageTo(item.m_protocol_name, item.m_account_name, item.m_item_name,
                message,cursor_position, true);
}

void AbstractChatLayer::showConferenceTopicSettings(const QString &protocol_name, const QString &account_name, const QString &conference)
{
  PluginSystem::instance().showTopicConfig(protocol_name, account_name, conference);
}

QStringList AbstractChatLayer::getConferenceItemsList(const QString &protocol_name, const QString &conference_name, const QString &account_name)
{
  QString identification = QString("%1.%2.%3")
                           .arg(protocol_name)
                           .arg(conference_name)
                           .arg(account_name);
  if ( m_separate_conference_list.contains(identification) )
  {
    return m_separate_conference_list.value(identification)->getItemsList();
  }
  return QStringList();
}

bool AbstractChatLayer::changeChatWindowID(const TreeModelItem &item, const QString &id)
{
  QString old_identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                               .arg(item.m_account_name).arg(item.m_item_name);
  QString new_identification = QString("%1.%2.%3").arg(item.m_protocol_name)
                               .arg(item.m_account_name).arg(id);
  if ( m_separate_window_list.contains(old_identification) && !m_separate_window_list.contains(new_identification) )
  {
    SeparateChatWindow *window = m_separate_window_list.take(old_identification);
    window->setID(id);
    m_separate_window_list.insert(new_identification, window);
    return true;
  }
  return false;
}
