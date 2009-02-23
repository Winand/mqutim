/*****************************************************************************
    Plugin System

    Copyright (c) 2008 by m0rph <m0rph.mailbox@gmail.com>
							Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#define QUTIM_BUILD_VERSION_MAJOR				0
#define QUTIM_BUILD_VERSION_MINOR				1
#define QUTIM_BUILD_VERSION_SECMINOR				99

#define QUTIM_SVN_REVISION                                    140

//#include <assert.h>
#include <QtAlgorithms>
#include <QDir>
#include <QStringList>
#include <QtGlobal>
#include <QDebug>
#include <QDomDocument>
#include "pluginsystem.h"
#include "abstractsoundlayer.h"
#include "abstractlayer.h"
#include "abstractchatlayer.h"
#include "antispam/abstractantispamlayer.h"
#include "notifications/abstractnotificationlayer.h"
#include "abstractcontextlayer.h"
#include "console.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iostream>

int debug_type;
std::ofstream debug_out;
Console *debug_console;
enum DebugTypeFlag
{
  DebugConsole=1,
  DebugFile=2,
  DebugWindow=4
};

void globalHandleDebug(QtMsgType type, const char *msg)
{
  PluginSystem::instance().handleDebug(type, msg);
}

void PluginSystem::handleDebug(QtMsgType type, const char *msg)
{
  QString message = QString::fromLocal8Bit(msg);
  if (debug_type&DebugWindow && debug_console)
    debug_console->appendMsg(message, type);
  switch (type)
  {
  case QtDebugMsg:
    if (debug_type&DebugConsole)
      printf("Debug: %s\n",msg);
    if (debug_type&DebugFile)
      debug_out << "Debug: " << msg << std::endl;
    break;
  case QtWarningMsg:
    if (debug_type&DebugConsole)
      printf("Warning: %s\n",msg);
    if (debug_type&DebugFile)
      debug_out << "Warning: " << msg << std::endl;
    break;
  case QtCriticalMsg:
    if (debug_type&DebugConsole)
      printf("Critical: %s\n",msg);
    if (debug_type&DebugFile)
      debug_out << "Critical: " << msg << std::endl;
    break;
  case QtFatalMsg:
    if (debug_type&DebugConsole)
      printf("Fatal: %s\n",msg);
    if (debug_type&DebugFile)
      debug_out << "Fatal: " << msg << std::endl;
    break;
  }
  if (debug_type&DebugFile)
    debug_out.flush();
}
QSettings::SettingsMap parseDict(const QDomNode & root_element)
{
  QSettings::SettingsMap style_hash;
  if (root_element.isNull())
    return style_hash;
  QDomNode subelement =root_element;//.firstChild();
  QString key="";
  for (QDomNode node = subelement.firstChild(); !node.isNull(); node = node.nextSibling())
  {
    QDomElement element = node.toElement();
    if (element.nodeName()=="key")
      key=element.text();
    else
    {
      QVariant value;
      if (element.nodeName()=="true")
        value=QVariant(true);
      else if (element.nodeName()=="false")
        value=QVariant(false);
      else if (element.nodeName()=="real")
        value=QVariant(element.text().toDouble());
      else if (element.nodeName()=="string")
        value=QVariant(element.text());
      else if (element.nodeName()=="integer")
        value=QVariant(element.text().toInt());
      else if (element.nodeName()=="dict")
        value = parseDict(node);
      style_hash.insert(key,value);
    }
  }
  return style_hash;
}
bool plistReadFunc(QIODevice &device, QSettings::SettingsMap &map)
{
  QDomDocument theme_dom;

  if (theme_dom.setContent(&device))
  {
    QDomElement root_element = theme_dom.documentElement();
    if (root_element.isNull())
      return false;
    map = parseDict(root_element.firstChild());
  }
  return true;
}
bool plistWriteFunc(QIODevice &device, const QSettings::SettingsMap &map)
{
  return false;
}

PluginSystem::PluginSystem() :
    m_abstract_contact_list(AbstractContactList::instance())
    , m_abstract_status_layer(AbstractStatusLayer::instance())
{
  QCoreApplication::setApplicationName("qutIM");
  m_plist_format = QSettings::registerFormat("plist",plistReadFunc,plistWriteFunc);
//	QCoreApplication::setApplicationVersion("%1.%2.%3");
}


PluginSystem::~PluginSystem()
{
  debug_out.close();
  delete debug_console;
  foreach(ProtocolInterface *protocol, m_protocols)
  {
    if (protocol) protocol->release();
  }
  qDeleteAll(m_loaders);
  m_loaders.clear();

}


PluginSystem &PluginSystem::instance()
{
  static PluginSystem ps;
  return ps;
}


PluginInterface *PluginSystem::loadPlugin(const QString &path)
{
  QPluginLoader *loader = new QPluginLoader(path);
  if (!loader->load())
  {
    return 0;
  }

  PluginInterface *plugin = qobject_cast<PluginInterface *>(loader->instance());
  if (!plugin)
  {
    delete loader;
    return 0;
  }

  if (!plugin->init(this))
  {
    delete loader;
    return 0;
  }
  plugin->setProfileName(m_profile_name);


  if ( plugin->type() == "protocol" )
  {
//#if defined(BSD)
    if (ProtocolInterface *protocol = (ProtocolInterface *)(plugin))
      /*#else
      		if (ProtocolInterface *protocol = dynamic_cast<ProtocolInterface *>(plugin))
      #endif*/
    {
      m_protocols.insert (protocol->name(), protocol);
      qDebug() << "Protocol " << protocol->name() << " was loaded\n";
    }
  }
  else
  {
    m_loaders.append(loader);
  }
  return plugin;
}


bool PluginSystem::unloadPlugin(const QString &name)
{
  for (int i = 0; i < m_loaders.count(); ++i)
  {
    QPluginLoader *loader = m_loaders[i];
    PluginInterface *plugin = qobject_cast<PluginInterface *>(loader->instance());
    if (plugin->name() == name)
    {
      plugin->release();
      delete loader;
      m_loaders.removeAt(i);
      return true;
    }
  }

  return false;
}


bool PluginSystem::unloadPlugin(PluginInterface *object)
{
  for (int i = 0; i < m_loaders.count(); ++i)
  {
    QPluginLoader *loader = m_loaders[i];
    PluginInterface *plugin = qobject_cast<PluginInterface *>(loader->instance());
    if (plugin == object)
    {
      plugin->release();
      delete loader;
      m_loaders.removeAt(i);
      return true;
    }
  }

  return false;
}


int PluginSystem::pluginsCount()
{
  return m_loaders.count();
}


SimplePluginInterface *PluginSystem::getPluginByIndex(int index)
{
  //assert (!m_loaders.empty());
  //assert (0 <= index && index < m_loaders.count());
  return static_cast<SimplePluginInterface*>(qobject_cast<PluginInterface *>(m_loaders[index]->instance()));
}


void PluginSystem::registerEventHandler(const EventType &type, SimplePluginInterface *plugin)
{
  m_registered_events_plugins.insert(type, plugin);
}


void PluginSystem::releaseEventHandler(const QString &event_id, PluginInterface *plugin)
{
//	PluginEventLists::iterator it;
//	it = m_event_lists.find(event_id);
//	if(it == m_event_lists.end())
//		return;
//
//	PluginList *handlers_list = &it.value();
//	int i = handlers_list->indexOf(plugin);
//	if(i == -1)
//		return;
//
//	handlers_list->removeAt(i);
//	if(handlers_list->isEmpty())
//		m_event_lists.erase(it);
}


void PluginSystem::processEvent(PluginEvent &event)
{
//	PluginEventLists::iterator it;
//	it = m_event_lists.find(event.id);
//	if(it == m_event_lists.end())
//		return;
//
//	PluginList *handlers_list = &m_event_lists[event.id];
//
//	foreach(SimplePluginInterdace *plugin, (*handlers_list))
//	{
//		plugin->processEvent(event);
//	}
}


QStringList PluginSystem::findPlugins(const QString &path)
{
  QStringList result;
  QDir plugins_dir(path);

  // We're don't need to process symlinks, only files
  QStringList files = plugins_dir.entryList(QDir::Files);
  for (int i = 0; i < files.count(); ++i)
  {
    QString filename = files[i].prepend(path);
    QPluginLoader loader(filename);
    PluginInterface *plugin = qobject_cast<PluginInterface *>(loader.instance());
    if (plugin)
      result << filename;
    else
      qDebug() << loader.errorString();
  }
  return result;
}


void PluginSystem::loadPlugins()
{
  /*        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim", "qutimsettings");*/

  QStringList files;
  /*	QString place = QCoreApplication::applicationDirPath()+"/../lib/qutim/";*/
  QString place = QCoreApplication::applicationDirPath()+"/../lib/plugins/";
  QStringList result = findPlugins(place);
  files << result;

  /*	place = settings.fileName().section('/', 0, -2) + "/plugins/";
  	result = findPlugins(place);
          files << result;*/

  place = QCoreApplication::applicationDirPath()+"/plugins/";
  result = findPlugins(place);
  files << result;
  foreach(const QString &filename, files)
  loadPlugin(filename);
}


PluginInfoList PluginSystem::getPluginsByType(const QString &type)
{
  PluginInfoList temp;
  if ( type != "protocol")
  {
    for (int i = 0; i < pluginsCount(); ++i)
    {
      PluginInterface *plugin = getPluginByIndex(i);
      if (plugin->type() == type)
      {
        PluginInfo info;
        info.name = plugin->name();
        info.description = plugin->description();
        info.type = plugin->type();
        if (plugin->icon())
          info.icon = *plugin->icon();

        temp.append(info);
      }
    }
  }
  else
  {
    foreach(ProtocolInterface *plugin, m_protocols.values() )
    {
      PluginInfo info;
      info.name = plugin->name();
      info.description = plugin->description();
      info.type = plugin->type();
      if (plugin->icon())
        info.icon = *plugin->icon();
      temp.append(info);
    }
  }

  return temp;
}

bool PluginSystem::setLayerInterface( LayerType type, LayerInterface *interface)
{
  return false;
}

QWidget *PluginSystem::getLoginWidget(const QString &protocol_name)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return 0;

  return protocol->loginWidget();
}

void PluginSystem::removeLoginWidgetByName(const QString &protocol_name)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;

  protocol->removeLoginWidget();
}

void PluginSystem::removeAccount(const QString &protocol_name, const QString &account_name)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;

  protocol->removeAccount(account_name);
}

void PluginSystem::loadProfile(const QString &profile_name)
{
  m_profile_name = profile_name;
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
#if defined(Q_OS_WIN)
  debug_type = settings.value("debug/type", 0).toInt();
#else
  debug_type = settings.value("debug/type", DebugConsole).toInt();
#endif
  if (debug_type & DebugFile)
  {
    QString file_name = QFileInfo(settings.fileName()).absoluteDir().filePath(settings.value("debug/file","system.log").toString());
    debug_out.open(file_name.toLocal8Bit(), std::ios_base::app);
  }
  if (debug_type & DebugWindow)
  {
    debug_console = new Console();
  }
  else
    debug_console = 0;
  qInstallMsgHandler(globalHandleDebug);
  loadPlugins();
}

void PluginSystem::applySettingsPressed(const QString &protocol_name)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;

  protocol->applySettingsPressed();
}

QList<QMenu*> PluginSystem::addStatusMenuToTrayMenu()
{
  QList<QMenu*> status_menu_list;
  foreach(ProtocolInterface *protocol, m_protocols)
  {
    if (protocol)
    {
      foreach(QMenu *account_menu, protocol->getAccountStatusMenu())
      {
        status_menu_list.append(account_menu);
      }
    }
  }
  return status_menu_list;
}

void PluginSystem::addAccountButtonsToLayout(QHBoxLayout *account_buttons_layout)
{
  foreach(ProtocolInterface *protocol, m_protocols)
  {
    if (protocol)
    {
      protocol->addAccountButtonsToLayout(account_buttons_layout);
    }
  }
}

void PluginSystem::saveLoginDataByName(const QString &protocol_name)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;
  protocol->saveLoginDataFromLoginWidget();
}

QList<SettingsStructure> PluginSystem::getSettingsByName(const QString &protocol_name) const
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
  {
    QList<SettingsStructure> list;
    return list;
  }
  return protocol->getSettingsList();
}

void PluginSystem::removeProtocolSettingsByName(const QString &protocol_name)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;
  protocol->removeProtocolSettings();
}

QList<AccountStructure> PluginSystem::getAccountsList() const
{
  QList<AccountStructure> full_account_list;
  foreach(ProtocolInterface *protocol, m_protocols)
  {

    if (protocol)
    {
      foreach(AccountStructure account_structure, protocol->getAccountList())
      {
        full_account_list.append(account_structure);
      }
    }
  }
  return full_account_list;
}

QList<AccountStructure> PluginSystem::getAccountsStatusesList() const
{
  QList<AccountStructure> full_account_list;
  foreach(ProtocolInterface *protocol, m_protocols)
  {

    if (protocol)
    {

      foreach(AccountStructure account_structure, protocol->getAccountStatuses())
      {
        full_account_list.append(account_structure);
      }
    }
  }
  return full_account_list;
}

void PluginSystem::updateStatusIcons()
{
  AbstractLayer &as = AbstractLayer::instance();
  as.updateTrayIcon();
}

void PluginSystem::setAutoAway()
{
  foreach(ProtocolInterface *protocol, m_protocols)
  {

    if (protocol)
    {
      protocol->setAutoAway();
    }
  }
}

void PluginSystem::setStatusAfterAutoAway()
{
  foreach(ProtocolInterface *protocol, m_protocols)
  {

    if (protocol)
    {
      protocol->setStatusAfterAutoAway();
    }
  }
}

bool PluginSystem::addItemToContactList(TreeModelItem item, QString name)
{
  bool result = false;
  if (item.m_item_type < TreeModelItem::ConferenceItem)
    result = m_abstract_contact_list.addItem(item, name);
  else if (item.m_item_type == TreeModelItem::ConferenceItem)
    result = AbstractChatLayer::instance().addConferenceItem(item.m_protocol_name, item.m_parent_name, item.m_account_name, item.m_item_name);
  else if (item.m_item_type == TreeModelItem::Conference)
    result = AbstractChatLayer::instance().createConference(item.m_protocol_name, item.m_item_name, item.m_account_name);
  if (result)
  {
    foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(ItemAddedAction))
    {
      PluginEvent event;
      event.system_event_type = ItemAddedAction;
      event.args.append(&item);
      event.args.append(&name);
      plugin->processEvent(event);
    }
  }
  return result;
}

bool PluginSystem::removeItemFromContactList(TreeModelItem item)
{
  bool result = false;
  if (item.m_item_type < TreeModelItem::ConferenceItem)
    result = m_abstract_contact_list.removeItem(item);
  else if (item.m_item_type == TreeModelItem::ConferenceItem)
    result = AbstractChatLayer::instance().removeConferenceItem(item.m_protocol_name, item.m_parent_name, item.m_account_name, item.m_item_name);
  else if (item.m_item_type == TreeModelItem::Conference)
    result = AbstractChatLayer::instance().createConference(item.m_protocol_name, item.m_item_name, item.m_account_name);
  if (result)
  {
    foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(ItemRemovedAction))
    {
      PluginEvent event;
      event.system_event_type = ItemRemovedAction;
      event.args.append(&item);
      plugin->processEvent(event);
    }
  }
  return result;
}

bool PluginSystem::moveItemInContactList(TreeModelItem old_item, TreeModelItem new_item)
{
  bool result = m_abstract_contact_list.moveItem(old_item, new_item);
  if (result)
  {
    foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(ItemMovedAction))
    {
      PluginEvent event;
      event.system_event_type = ItemMovedAction;
      event.args.append(&old_item);
      event.args.append(&new_item);
      plugin->processEvent(event);
    }
  }
  return result;
}

bool PluginSystem::setContactItemName(TreeModelItem item, QString name)
{
  bool result = false;
  if (item.m_item_type < TreeModelItem::ConferenceItem)
    result = m_abstract_contact_list.setItemName(item, name);
  else if (item.m_item_type == TreeModelItem::ConferenceItem)
    result = AbstractChatLayer::instance().renameConferenceItem(item.m_protocol_name, item.m_parent_name, item.m_account_name, item.m_item_name, name);
  if (result)
  {
    foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(ItemChangedNameAction))
    {
      PluginEvent event;
      event.system_event_type = ItemChangedNameAction;
      event.args.append(&item);
      event.args.append(&name);
      plugin->processEvent(event);
    }
  }
  return result;
}

bool PluginSystem::setContactItemIcon(TreeModelItem item, QIcon icon, int position)
{
  bool result = false;
  if (item.m_item_type < TreeModelItem::ConferenceItem)
    result = m_abstract_contact_list.setItemIcon(item, icon, position);
  else if (item.m_item_type == TreeModelItem::ConferenceItem)
    result = AbstractChatLayer::instance().setConferenceItemIcon(item.m_protocol_name, item.m_parent_name, item.m_account_name, item.m_item_name, icon, position);
  return result;
}

bool PluginSystem::setContactItemRow(TreeModelItem item, QList<QVariant> row, int position)
{
  bool result = false;
  if (item.m_item_type < TreeModelItem::ConferenceItem)
    result = m_abstract_contact_list.setItemRow(item, row, position);
  return result;
}

bool PluginSystem::setContactItemStatus(TreeModelItem item, QIcon icon, QString text, int mass)
{
  bool result = false;
  if (item.m_item_type < TreeModelItem::ConferenceItem)
    result = m_abstract_contact_list.setItemStatus(item, icon, text, mass);
  else if (item.m_item_type == TreeModelItem::ConferenceItem)
    result = AbstractChatLayer::instance().setConferenceItemStatus(item.m_protocol_name, item.m_parent_name, item.m_account_name, item.m_item_name, icon, text, mass);
  if (result)
  {
    foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(ItemChangedStatusAction))
    {
      PluginEvent event;
      event.system_event_type = ItemChangedStatusAction;
      event.args.append(&item);
      event.args.append(&icon);
      event.args.append(&text);
      event.args.append(&mass);
      plugin->processEvent(event);
    }
  }
  return result;
}

void PluginSystem::itemActivated(const TreeModelItem &item)
{
  ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  if (!protocol)
    return;
  protocol->itemActivated(item.m_account_name, item.m_item_name);
}

void PluginSystem::itemContextMenu(const QList<QAction*> &action_list,
                                   const TreeModelItem &item, const QPoint &menu_point)
{
  setCurrentContextItemToPlugins(item);
  ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  if (!protocol)
    return;
  protocol->itemContextMenu(action_list,item.m_account_name, item.m_item_name, item.m_item_type, menu_point);
}

bool PluginSystem::setStatusMessage(QString &status_message, bool &dshow)
{
  return m_abstract_status_layer.setStatusMessage(status_message, dshow);
}

void PluginSystem::sendMessageToContact(const TreeModelItem &item, QString &message, int message_icon_position)
{

  TreeModelItem tmp_item = item;
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(SendingMessageAfterShowing))
  {
    PluginEvent event;
    event.system_event_type = SendingMessageAfterShowing;
    QList<void *> params;
    params.append(&tmp_item);
    params.append(&message);
    event.args = params;
    plugin->processEvent(event);
  }
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(SengingMessageAfterShowingLastOutput))
  {
    PluginEvent event;
    event.system_event_type = SengingMessageAfterShowingLastOutput;
    QList<void *> params;
    params.append(&tmp_item);
    params.append(&message);
    event.args = params;
    plugin->processEvent(event);
  }
  ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  if (!protocol)
    return;
  protocol->sendMessageTo(item.m_account_name, item.m_item_name, item.m_item_type, message, message_icon_position);
}

void PluginSystem::addMessageFromContact(const TreeModelItem &item, const QString &message
    , const QDateTime &message_date)
{
  qDebug("PluginSystem::addMessageFromContact\n");
  QString tmp_message = message;
  bool stop_on_this_level = false;
  TreeModelItem tmp_item = item;
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(ReceivingMessageFirstLevel))
  {
    PluginEvent event;
    event.system_event_type = ReceivingMessageFirstLevel;
    QList<void *> params;
    params.append(&tmp_item);
    params.append(&tmp_message);
    event.args = params;
    plugin->processEvent(event);
  }
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(ReceivingMessageSecondLevel))
  {
    PluginEvent event;
    event.system_event_type = ReceivingMessageSecondLevel;
    QList<void *> params;
    params.append(&tmp_item);
    params.append(&tmp_message);
    params.append(&stop_on_this_level);
    event.args = params;
    plugin->processEvent(event);
    if ( stop_on_this_level )
      return;
  }

  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.addModifiedMessage(item, tmp_message, true, message_date);
}

void PluginSystem::addServiceMessage(const TreeModelItem &item, const QString &message)
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.addServiceMessage(item, message);
}

QStringList PluginSystem::getAdditionalInfoAboutContact(const TreeModelItem &item) const
{
  ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  if (!protocol)
    return QStringList();
  return protocol->getAdditionalInfoAboutContact(item.m_account_name, item.m_item_name, item.m_item_type);
}

void PluginSystem::showContactInformation(const TreeModelItem &item)
{
  ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  if (!protocol)
    return;
  return protocol->showContactInformation(item.m_account_name, item.m_item_name, item.m_item_type);
}

void PluginSystem::sendImageTo(const TreeModelItem &item, const QByteArray &image_raw)
{
  /*		ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  		if(!protocol)
  			return;
                  return protocol->sendImageTo(item.m_account_name, item.m_item_name, item.m_item_type, image_raw);*/
}

void PluginSystem::addImage(const TreeModelItem &item, const QByteArray &image_raw)
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.addImage(item, image_raw, true);
}

void PluginSystem::sendFileTo(const TreeModelItem &item, const QStringList &file_names)
{
  /*		ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
                  if(!protocol)
                          return;
                  return protocol->sendFileTo(item.m_account_name, item.m_item_name, item.m_item_type, file_names);*/
}

void PluginSystem::sendTypingNotification(const TreeModelItem &item, int notification_type)
{

  ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  if (!protocol)
    return;
  return protocol->sendTypingNotification(item.m_account_name, item.m_item_name, item.m_item_type, notification_type);
}

void PluginSystem::contactTyping(const TreeModelItem &item, bool typing)
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.contactTyping(item, typing);
  AbstractContactList &ac = AbstractContactList::instance();
  ac.setItemIsTyping(item,typing);
}

void PluginSystem::messageDelievered(const TreeModelItem &item, int message_position)
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.messageDelievered(item, message_position);
}

bool PluginSystem::checkForMessageValidation(const TreeModelItem &item, const QString &message,
    int message_type, bool special_status)
{
  AbstractAntiSpamLayer &aasl = AbstractAntiSpamLayer::instance();
  return aasl.checkForMessageValidation(item, message, message_type, special_status);
}

void PluginSystem::notifyAboutBirthDay(const TreeModelItem &item)
{
  AbstractNotificationLayer::instance().userMessage(item, "", NotifyBirthday);
  AbstractSoundLayer::instance().playSound(NotifyBirthday);
}

void PluginSystem::systemNotifiacation(const TreeModelItem &item, const QString &message)
{
  AbstractNotificationLayer::instance().systemMessage(item, message);
  AbstractSoundLayer::instance().playSound(NotifySystem);
  TreeModelItem tmp_item = item;
  QString tmp_message = message;
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(SystemNotification))
  {
    PluginEvent event;
    event.system_event_type = SystemNotification;
    event.args.append(&tmp_item);
    event.args.append(&tmp_message);
    plugin->processEvent(event);
  }
}

void PluginSystem::userNotification(TreeModelItem item, QString message, int type)
{
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(UserNotification))
  {
    PluginEvent event;
    event.system_event_type = UserNotification;
    event.args.append(&item);
    event.args.append(&message);
    event.args.append(&type);
    plugin->processEvent(event);
  }
}

void PluginSystem::customNotifiacation(const TreeModelItem &item, const QString &message)
{
  AbstractNotificationLayer::instance().userMessage(item, message, NotifyCustom);
}
QString PluginSystem::getIconFileName(const QString & icon_name)
{
  return IconManager::instance().getIconFileName(icon_name);
}
QIcon PluginSystem::getIcon(const QString & icon_name)
{
  return IconManager::instance().getIcon(icon_name);
}
QString PluginSystem::getStatusIconFileName(const QString & icon_name, const QString & default_path)
{
  return IconManager::instance().getStatusIconFileName(icon_name, default_path);
}
QIcon PluginSystem::getStatusIcon(const QString & icon_name, const QString & default_path)
{
  return IconManager::instance().getStatusIcon(icon_name, default_path);
}

void PluginSystem::moveItemSignalFromCL(const TreeModelItem &old_item, const TreeModelItem &new_item)
{
  ProtocolInterface *protocol = m_protocols.value(old_item.m_protocol_name);
  if (!protocol)
    return;

  protocol->moveItemSignalFromCL(old_item, new_item);
}

QString PluginSystem::getItemToolTip(const TreeModelItem &item)
{
  ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  if (!protocol)
    return item.m_item_name;

  return protocol->getItemToolTip(item.m_account_name, item.m_item_name);
}

void PluginSystem::deleteItemSignalFromCL(const TreeModelItem& item)
{
  ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  if (!protocol)
    return;

  protocol->deleteItemSignalFromCL(item.m_account_name,
                                   item.m_item_name, item.m_item_type);
}

void PluginSystem::setAccountIsOnline(const TreeModelItem &item, bool online)
{
  AbstractContactList::instance().setAccountIsOnline(item, online);
  TreeModelItem tmp_item = item;
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(AccountIsOnlineAction))
  {
    PluginEvent event;
    event.system_event_type = AccountIsOnlineAction;
    event.args.append(&tmp_item);
    event.args.append(&online);
    plugin->processEvent(event);
  }
}

void PluginSystem::createChat(const TreeModelItem &item)
{
  AbstractChatLayer::instance().createChat(item);
}

void PluginSystem::getQutimVersion(quint8 &major, quint8 &minor, quint8 &secminor, quint16 &svn)
{
  major = QUTIM_BUILD_VERSION_MAJOR;
  minor = QUTIM_BUILD_VERSION_MINOR;
  secminor = QUTIM_BUILD_VERSION_SECMINOR;
  svn = QUTIM_SVN_REVISION;
}

void PluginSystem::chatWindowOpened(const TreeModelItem &item)
{
  ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  if (!protocol)
    return;

  protocol->chatWindowOpened(item.m_account_name, item.m_item_name);
}

void PluginSystem::chatWindowAboutToBeOpened(const TreeModelItem &item)
{
  ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  if (!protocol)
    return;

  protocol->chatWindowAboutToBeOpened(item.m_account_name, item.m_item_name);
}

void PluginSystem::chatWindowClosed(const TreeModelItem &item)
{
  ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
  if (!protocol)
    return;

  protocol->chatWindowClosed(item.m_account_name, item.m_item_name);
}

void PluginSystem::createConference(const QString &protocol_name,
                                    const QString &conference_name,
                                    const QString &account_name)
{
  AbstractChatLayer::instance().createConference(protocol_name,
      conference_name, account_name);
}

void PluginSystem::addMessageToConference(const QString &protocol_name,
    const QString &conference_name, const QString &account_name,
    const QString &from,
    const QString &message, const QDateTime &date, bool history)
{
  AbstractChatLayer::instance().addMessageToConference(
    protocol_name,
    conference_name,account_name,
    from, message, date, history);
}

void PluginSystem::sendMessageToConference(
  const QString &protocol_name,
  const QString &conference_name, const QString &account_name,
  const QString &message)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;

  protocol->sendMessageToConference(conference_name, account_name, message);
}

void PluginSystem::changeOwnConferenceNickName(const QString &protocol_name,
    const QString &conference_name,
    const QString &account_name,
    const QString &nickname)
{
  AbstractChatLayer::instance().changeOwnConferenceNickName(protocol_name,
      conference_name, account_name, nickname);
}

void PluginSystem::setConferenceTopic(const QString &protocol_name, const QString &conference_name,
                                      const QString &account_name, const QString &topic)
{
  AbstractChatLayer::instance().setConferenceTopic(
    protocol_name, conference_name,
    account_name, topic);
}

void PluginSystem::addSystemMessageToConference(const QString &protocol_name,
    const QString &conference_name, const QString &account_name,
    const QString &message, const QDateTime &date, bool history)
{
  AbstractChatLayer::instance().addSystemMessageToConference(
    protocol_name, conference_name,account_name,
    message, date, history);
}

void PluginSystem::leaveConference(const QString &protocol_name,
                                   const QString &conference_name, const QString &account_name)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;

  protocol->leaveConference(conference_name, account_name);
}
void PluginSystem::addConferenceItem(const QString &protocol_name, const QString &conference_name,
                                     const QString &account_name, const QString &nickname)
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.addConferenceItem(protocol_name, conference_name, account_name, nickname);
}
void PluginSystem::removeConferenceItem(const QString &protocol_name, const QString &conference_name,
                                        const QString &account_name, const QString &nickname)
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.removeConferenceItem(protocol_name, conference_name, account_name, nickname);
}
void PluginSystem::renameConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name,
                                        const QString &nickname, const QString &new_nickname)
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.renameConferenceItem(protocol_name, conference_name, account_name, nickname, new_nickname);
}
void PluginSystem::setConferenceItemStatus(const QString &protocol_name, const QString &conference_name, const QString &account_name,
    const QString &nickname, const QIcon &icon, const QString &status, int mass)
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.setConferenceItemStatus(protocol_name, conference_name, account_name, nickname, icon, status, mass);
}
void PluginSystem::setConferenceItemIcon(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname,
    const QIcon &icon, int position)
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.setConferenceItemIcon(protocol_name, conference_name, account_name, nickname, icon, position);
}
void PluginSystem::setConferenceItemRole(const QString &protocol_name, const QString &conference_name, const QString &account_name,
    const QString &nickname, const QIcon &icon, const QString &role, int mass)
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.setConferenceItemRole(protocol_name, conference_name, account_name, nickname, icon, role, mass);
}
QStringList PluginSystem::getConferenceItemsList(const QString &protocol_name,
    const QString &conference_name, const QString &account_name)
{
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  return acl.getConferenceItemsList(protocol_name, conference_name, account_name);
}
void PluginSystem::setItemVisible(const TreeModelItem &item, bool visible)
{
  AbstractContactList::instance().setItemVisible(item, visible);
}
void PluginSystem::setItemInvisible(const TreeModelItem &item, bool invisible)
{
  AbstractContactList::instance().setItemInvisible(item, invisible);
}

void PluginSystem::conferenceItemActivated(const QString &protocol_name, const QString &conference_name,
    const QString &account_name, const QString &nickname)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;

  protocol->conferenceItemActivated(conference_name, account_name, nickname);
}

void PluginSystem::conferenceItemContextMenu(const QList<QAction*> &action_list, const QString &protocol_name,
    const QString &conference_name, const QString &account_name, const QString &nickname, const QPoint &menu_point)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;

  protocol->conferenceItemContextMenu(action_list, conference_name, account_name, nickname, menu_point);
}

QString PluginSystem::getConferenceItemToolTip(const QString &protocol_name, const QString &conference_name,
    const QString &account_name, const QString &nickname)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return nickname;

  return protocol->getConferenceItemToolTip(conference_name, account_name, nickname);
}

void PluginSystem::showConferenceContactInformation(const QString &protocol_name,
    const QString &conference_name, const QString &account_name, const QString &nickname)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;

  protocol->showConferenceContactInformation(conference_name, account_name, nickname);
}

void PluginSystem::removePluginsSettingsWidget()
{
  for (int i = 0; i < m_loaders.count(); ++i)
  {
    SimplePluginInterface *plugin = getPluginByIndex(i);
    if (plugin)
    {
      plugin->removeSettingsWidget();
    }
  }
}

void PluginSystem::saveAllPluginsSettings()
{
  for (int i = 0; i < m_loaders.count(); ++i)
  {
    SimplePluginInterface *plugin = getPluginByIndex(i);
    if (plugin)
    {
      plugin->saveSettings();
    }
  }
}

void PluginSystem::registerContactMenuAction(QAction *plugin_action, SimplePluginInterface *pointer_this)
{
  AbstractContextLayer::instance().registerContactMenuAction(plugin_action);
  m_registered_events_plugins.insert(ContactContextAction, pointer_this);
}

void PluginSystem::setCurrentContextItemToPlugins(const TreeModelItem &item)
{
  TreeModelItem tmp_item = item;
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(ContactContextAction))
  {
    PluginEvent event;
    event.system_event_type = ContactContextAction;
    QList<void *> params;
    params.append(&tmp_item);
    event.args = params;
    plugin->processEvent(event);
  }
}

void PluginSystem::sendCustomMessage(const TreeModelItem &item, const QString &message, bool silent)
{
  if ( silent )
  {
    ProtocolInterface *protocol = m_protocols.value(item.m_protocol_name);
    if (!protocol)
      return;
    protocol->sendMessageTo(item.m_account_name, item.m_item_name, item.m_item_type, message, 0);
  }
  else
    AbstractChatLayer::instance().sendMessageFromPlugin(item, message);
}

void PluginSystem::registerMainMenuAction(QAction *menu_action)
{
  AbstractLayer::instance().addActionToMainMenu(menu_action);
}

void PluginSystem::showTopicConfig(const QString &protocol_name, const QString &account_name, const QString &conference)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;
  protocol->showConferenceTopicConfig(conference, account_name);
}

void PluginSystem::showConferenceMenu(const QString &protocol_name, const QString &account_name, const QString &conference_name,
                                      const QPoint &menu_point)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;
  protocol->showConferenceMenu(conference_name, account_name, menu_point);
}

void PluginSystem::redirectEventToProtocol(const QStringList &protocol_name, const QList<void*> &event)
{
  if ( !protocol_name.count() )
  {
    foreach(ProtocolInterface *protocol, m_protocols)
    {
      protocol->getMessageFromPlugins(event);
    }
  }
  else
  {
    foreach(const QString &protocol_n, protocol_name)
    {
      ProtocolInterface *protocol = m_protocols.value(protocol_n);
      if (protocol)
        protocol->getMessageFromPlugins(event);
    }
  }

}

void PluginSystem::sendMessageBeforeShowing(const TreeModelItem &item, QString &message)
{
  TreeModelItem tmp_item = item;
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(SendingMessageBeforeShowing))
  {
    PluginEvent event;
    event.system_event_type = SendingMessageBeforeShowing;
    QList<void *> params;
    params.append(&tmp_item);
    params.append(&message);
    event.args = params;
    plugin->processEvent(event);
  }
}

void PluginSystem::pointersAreInitialized()
{
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(PointersAreInitialized))
  {
    PluginEvent event;
    event.system_event_type = PointersAreInitialized;
    plugin->processEvent(event);
  }
}

void PluginSystem::playSoundByPlugin(QString path)
{
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(SoundAction))
  {
    PluginEvent event;
    event.system_event_type = SoundAction;
    QList<void *> params;
    params.append(&path);
    plugin->processEvent(event);
  }
}

void PluginSystem::playSound(NotificationType event)
{
  AbstractSoundLayer::instance().playSound(event);
}

void PluginSystem::playSound(const QString &file_name)
{
  AbstractSoundLayer::instance().playSound(file_name);
}

void PluginSystem::receivingMessageBeforeShowing(const TreeModelItem &item, QString &message)
{
  TreeModelItem tmp_item = item;
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(ReceivingMessageThirdLevel))
  {
    PluginEvent event;
    event.system_event_type = ReceivingMessageThirdLevel;
    QList<void *> params;
    params.append(&tmp_item);
    params.append(&message);
    event.args = params;
    plugin->processEvent(event);
  }
  foreach(SimplePluginInterface *plugin, m_registered_events_plugins.values(ReceivingMessageFourthLevel))
  {
    PluginEvent event;
    event.system_event_type = ReceivingMessageFourthLevel;
    QList<void *> params;
    params.append(&tmp_item);
    params.append(&message);
    event.args = params;
    plugin->processEvent(event);
  }
}

void PluginSystem::editAccount(const QString &protocol_name, const QString &account_name)
{
  ProtocolInterface *protocol = m_protocols.value(protocol_name);
  if (!protocol)
    return;

  protocol->editAccount(account_name);
}

bool PluginSystem::changeChatWindowID(const TreeModelItem &item, const QString &id)
{
  return AbstractChatLayer::instance().changeChatWindowID(item, id);
}
