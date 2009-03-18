/*****************************************************************************
    Abstract Contact List

    Copyright (c) 2008 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "abstractcontactlist.h"
#include "pluginsystem.h"
#include "abstractchatlayer.h"
#include "qutim.h"
#include "abstractlayer.h"
#include "abstractnotificationlayer.h"
#include "abstractcontextlayer.h"
#include "abstracthistorylayer.h"
#include "contactlistitemdelegate_qtopia.h"


AbstractContactList::AbstractContactList()
{
  m_first_load=true;
  m_has_tree_view=false;
}

AbstractContactList::~AbstractContactList()
{
  delete m_background_file;
}

AbstractContactList &AbstractContactList::instance()
{
  static AbstractContactList acs;
  return acs;
}

void AbstractContactList::setTreeView(QTreeView *TreeView)
{
  m_has_tree_view = true;
  m_tree_view = TreeView;
  QStringList headers;
  headers<<"1";
  m_item_model = new TreeContactListModel (headers);
  m_proxy_model = new ContactListProxyModel();
  QObject::connect(m_item_model, SIGNAL(itemInserted(QModelIndex)), m_proxy_model, SLOT(insertItem(QModelIndex)));
  QObject::connect(m_item_model, SIGNAL(itemRemoved(QModelIndex)), m_proxy_model, SLOT(removeItem(QModelIndex)));
  QObject::connect(m_item_model, SIGNAL(checkItem(QModelIndex)), m_proxy_model, SLOT(checkItem(QModelIndex)));
  QObject::connect(m_item_model, SIGNAL(itemNameChanged(QModelIndex,QString)), m_proxy_model, SLOT(setName(QModelIndex,QString)));
  QObject::connect(m_item_model, SIGNAL(itemStatusChanged(QModelIndex,QIcon,QString,int)), m_proxy_model, SLOT(setStatus(QModelIndex,QIcon,QString,int)));
  m_tree_view->setModel(m_proxy_model);
  m_proxy_model->setModel(m_item_model);
  m_proxy_model->setTreeView(m_tree_view);

  QObject::connect(m_item_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), m_proxy_model, SLOT(newData(QModelIndex,QModelIndex)));

  m_item_model->loadSettings(m_profile_name);
  m_proxy_model->loadProfile(m_profile_name);
  m_first_load=false;
  loadGuiSettings();
  loadSettings();
}
bool AbstractContactList::addItem(const TreeModelItem & Item, QString name)
{
  if (!m_has_tree_view)
    return false;
  //qWarning() << "addItem		" << Item.m_item_type << Item.m_protocol_name << Item.m_account_name << Item.m_parent_name << Item.m_item_name;
  if (name.isEmpty())
    name=Item.m_item_type==TreeModelItem::Account?Item.m_account_name:Item.m_item_name;
  bool result=false;
  m_tree_view->setUpdatesEnabled(false);
  QModelIndex parent_index = m_proxy_model->mapFromSource(m_item_model->findIndex(Item)).parent();
  switch (Item.m_item_type)
  {
  case TreeModelItem::Buddy:
    result = m_item_model->addBuddy(Item, name);
    break;
  case TreeModelItem::Group:
    result = m_item_model->addGroup(Item, name);
    break;
  case TreeModelItem::Account:
    result = m_item_model->addAccount(Item, name);
    break;
  }
  if (parent_index.isValid() && result)
    if (m_tree_view->isExpanded(parent_index))
    {
      m_tree_view->setExpanded(parent_index,false);
      m_tree_view->setExpanded(parent_index,true);
    }
  m_tree_view->setUpdatesEnabled(true);
  AbstractHistoryLayer::instance().setContactHistoryName(Item);
  return result;
}
bool AbstractContactList::removeItem(const TreeModelItem & Item)
{
  if (!m_has_tree_view)
    return false;
  //qWarning() << "removeItem		" << Item.m_item_type << Item.m_protocol_name << Item.m_account_name << Item.m_parent_name << Item.m_item_name;
  //m_tree_view->setUpdatesEnabled(false);
  bool result=false;
  QModelIndex parent_index = m_proxy_model->mapFromSource(m_item_model->findIndex(Item)).parent();
  m_tree_view->setUpdatesEnabled(false);
  switch (Item.m_item_type)
  {
  case TreeModelItem::Buddy:
    result = m_item_model->removeBuddy(Item);
    break;
  case TreeModelItem::Group:
    result = m_item_model->removeGroup(Item);
    break;
  case TreeModelItem::Account:
    result = m_item_model->removeAccount(Item);
    break;
  }
  if (parent_index.isValid() && result)
    if (m_tree_view->isExpanded(parent_index))
    {
      m_tree_view->setExpanded(parent_index,false);
      m_tree_view->setExpanded(parent_index,true);
    }
  m_tree_view->setUpdatesEnabled(true);
  return result;
}
bool AbstractContactList::moveItem(const TreeModelItem & OldItem, const TreeModelItem & NewItem)
{
  if (!m_has_tree_view)
    return false;
  if (OldItem.m_item_type!=TreeModelItem::Buddy)
    return false;
  TreeItem *item = m_item_model->findItem(OldItem);
  if (item==0)
    return false;
  QVariant display = item->data(Qt::DisplayRole);
  QList<QVariant> decoration = *reinterpret_cast<QList<QVariant> *>(item->data(ContactIconsRole).value<qptrdiff>());
  int mass = item->data(ContactMassRole).toInt();
  QList<QVariant> rows = *reinterpret_cast<QList<QVariant> *>(item->data(ContactTextRole).value<qptrdiff>());
  QString status = item->data(ContactStatusRole).toString();
  bool always_visible = item->getAlwaysVisible();
  bool always_invisible = item->getAlwaysInvisible();
  if (removeItem(OldItem))
  {
    if (addItem(NewItem,display.toString()))
    {
      m_tree_view->setUpdatesEnabled(false);
      item = m_item_model->findItem(NewItem);
      item->setData(decoration,ContactIconsRole);
      for (int i = 0; i < rows.size(); i++)
        setItemRow(NewItem,rows[i].toList(),i);
      setItemVisible(NewItem,always_visible);
      setItemInvisible(NewItem,always_invisible);
      setItemStatus(NewItem,qvariant_cast<QIcon>(decoration[0]),status,mass);
      return true;
    }
    return false;
  }
  //return m_item_model->moveBuddy(OldItem,NewItem);
  else
    return false;
}

bool AbstractContactList::setItemName(const TreeModelItem & Item, QString name)
{
  if (!m_has_tree_view)
    return false;
  m_tree_view->setUpdatesEnabled(false);
  //qWarning() << "setItemName		" << Item.m_item_type << Item.m_protocol_name << Item.m_account_name << Item.m_parent_name << Item.m_item_name;
  bool result = m_item_model->setItemName(Item, name);
  m_tree_view->setUpdatesEnabled(true);
  return result;
}

bool AbstractContactList::setItemIcon(const TreeModelItem & Item, QIcon icon, int position)
{
  if (!m_has_tree_view)
    return false;
  //m_tree_view->setUpdatesEnabled(false);
  //qWarning() << "setItemIcon		" << Item.m_item_type << Item.m_protocol_name << Item.m_account_name << Item.m_parent_name << Item.m_item_name << position;
  bool result = m_item_model->setItemIcon(Item, icon, position);
  if (position==12)
    AbstractChatLayer::instance().contactChangeCLientIcon(Item);
  //m_tree_view->setUpdatesEnabled(true);
  return result;
}
bool AbstractContactList::setItemRow(const TreeModelItem & Item, QList<QVariant> row, int position)
{
  if (!m_has_tree_view)
    return false;
  m_tree_view->setUpdatesEnabled(false);
  //qWarning() << "setItemRow		" << Item.m_item_type << Item.m_protocol_name << Item.m_account_name << Item.m_parent_name << Item.m_item_name << position;
  bool result = m_item_model->setItemRow(Item, row, position);
  m_tree_view->setUpdatesEnabled(true);
  return result;
}
bool AbstractContactList::setItemStatus(const TreeModelItem & Item, QIcon icon, QString text, int mass)
{
  if (!m_has_tree_view)
    return false;
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.contactChageStatusIcon(Item,icon);
  m_tree_view->setUpdatesEnabled(false);
  //qWarning() << "setItemStatus	" << Item.m_item_type << Item.m_protocol_name << Item.m_account_name << Item.m_parent_name << Item.m_item_name << mass;
  bool result = m_item_model->setItemStatus(Item, icon, text, mass);
  m_tree_view->setUpdatesEnabled(true);
  return result;
}
void AbstractContactList::sendEventActivated(const QModelIndex & index)
{
  if (!m_has_tree_view)
    return;
  QModelIndex source_index = static_cast<ProxyModelItem*>(index.internalPointer())->getSourceIndex();
  if (!source_index.isValid())
    return;
  TreeModelItem item = static_cast<TreeItem*>(source_index.internalPointer())->getStructure();
  if (item.m_item_type==TreeModelItem::Buddy)
    itemActivated(item);
}
void AbstractContactList::sendEventActivated(const QPoint &pos)
{
  if (!m_has_tree_view)
    return;
  QPoint point = m_tree_view->mapFromGlobal(pos);
  if (!m_tree_view->rect().contains(point))
    return;
  QModelIndex index = m_tree_view->indexAt(point);
  if (index.isValid())
    sendEventActivated(index);
}
void AbstractContactList::sendEventClicked(const QModelIndex & index, const QPoint &point)
{
  if (!m_has_tree_view)
    return;
  QModelIndex source_index = static_cast<ProxyModelItem*>(index.internalPointer())->getSourceIndex();
  if (!source_index.isValid())
    return;
  TreeModelItem item = static_cast<TreeItem*>(source_index.internalPointer())->getStructure();
  contactMenu(item,point);
}
void AbstractContactList::sendEventHelp(const QModelIndex & /*index*/, const QPoint &/*point*/)
{
  if (!m_has_tree_view)
    return;
//	QModelIndex source_index = static_cast<ProxyModelItem*>(index.internalPointer())->getSourceIndex();
//	if(!source_index.isValid())
//		return;
//	TreeModelItem item = static_cast<TreeItem*>(source_index.internalPointer())->getStructure();
//	if(item.m_item_type==0)
//	{
//		//TODO: Draw tooltip. Use TreeItemModel item, QPoint point
//		PluginSystem &ps = PluginSystem::instance();
//		QString tooltip = ps.getItemToolTip(item);
//		if(!tooltip.isNull())
//			static_cast<TreeItem*>(source_index.internalPointer())->setData(tooltip,Qt::ToolTipRole);
//		m_tree_view->setToolTip("-----");
//		m_tree_view->setToolTip("Hello, world? o_o");
//		QToolTip::showText(point,"Hello!");//,m_tree_view);
//	}
}
void AbstractContactList::itemActivated(const TreeModelItem & item)
{
  if (!m_has_tree_view)
    return;
//	PluginSystem &ps = PluginSystem::instance();
//	ps.itemActivated(item);
  AbstractChatLayer &acl = AbstractChatLayer::instance();
  acl.createChat(item);
  setItemHasUnviewedContent(item, false);
}
void AbstractContactList::contactMenu(const TreeModelItem & item, QPoint point)
{
  if (!m_has_tree_view)
    return;
  AbstractContextLayer::instance().itemContextMenu(item, point);
}
void AbstractContactList::itemMoved(const TreeModelItem & OldItem, const TreeModelItem & NewItem)
{
  if (!m_has_tree_view)
    return;
  PluginSystem::instance().moveItemSignalFromCL(OldItem, NewItem);
}
void AbstractContactList::setAccountIsOnline(const TreeModelItem &Item, bool online)
{
  if (!m_has_tree_view)
    return;
  m_item_model->setItemIsOnline(Item,online);
}
QString AbstractContactList::getItemStatus(const TreeModelItem &Item)
{
  if (!m_has_tree_view)
    return "";
  TreeItem *item = m_item_model->findItemNoParent(Item);
  if (item==0)
    return "";
  else
    return item->data(ContactStatusRole).toString();
}
void AbstractContactList::setItemHasUnviewedContent(const TreeModelItem & Item, bool has_content)
{
  if (!m_has_tree_view)
    return;
//	qDebug() << "content" << Item.m_protocol_name << Item.m_account_name << Item.m_item_name << has_content;
  m_item_model->setItemHasUnviewedContent(Item,has_content);
}
bool AbstractContactList::getItemHasUnviewedContent(const TreeModelItem & item)
{
  if (!m_has_tree_view)
    return false;
  return m_item_model->getItemHasUnviewedContent(item);
}
void AbstractContactList::setItemIsTyping(const TreeModelItem & Item, bool is_typing)
{
  if (!m_has_tree_view)
    return;
//	qDebug() << "typing" << Item.m_protocol_name << Item.m_account_name << Item.m_item_name << is_typing;
  m_item_model->setItemIsTyping(Item,is_typing);
}
bool AbstractContactList::getItemIsTyping(const TreeModelItem & Item)
{
  if (!m_has_tree_view)
    return false;
  return m_item_model->getItemIsTyping(Item);
}
bool AbstractContactList::itemExists(const TreeModelItem & Item)
{
  if (!m_has_tree_view)
    return false;
  TreeItem *item = m_item_model->findItemNoParent(Item);
  if (item==0)
    return false;
  return true;
}

QIcon AbstractContactList::getItemStatusIcon(const TreeModelItem & Item)
{
  if (!m_has_tree_view)
    return QIcon();
  TreeItem *item = m_item_model->findItemNoParent(Item);
  if (item==0)
  {
    return QIcon();
  }
  else
    return item->getImage(0);
}
QPixmap AbstractContactList::getItemClientIcon(const TreeModelItem & Item)
{
  if (!m_has_tree_view)
    return QPixmap();
  TreeItem *item = m_item_model->findItemNoParent(Item);
  if (item==0)
  {
    return QPixmap();
  }
  else
    return item->getImage(12).pixmap(65535,65535);
}
void AbstractContactList::setShowOffline(bool show)
{
  if (!m_has_tree_view)
    return;
  m_tree_view->setUpdatesEnabled(false);
  m_proxy_model->setShowOffline(show);
  m_tree_view->setUpdatesEnabled(true);
}
void AbstractContactList::setShowEmptyGroup(bool show)
{
  if (!m_has_tree_view)
    return;
  m_tree_view->setUpdatesEnabled(false);
  m_proxy_model->setShowEmptyGroup(show);
  m_tree_view->setUpdatesEnabled(true);
}
void AbstractContactList::setSortStatus(bool sort)
{
  if (!m_has_tree_view)
    return;
  m_tree_view->setUpdatesEnabled(false);
  m_proxy_model->setSortStatus(sort);
  m_tree_view->setUpdatesEnabled(true);
}
void AbstractContactList::setShowSeparator(bool show)
{
  if (!m_has_tree_view)
    return;
  m_tree_view->setUpdatesEnabled(false);
  m_proxy_model->setShowSeparator(show);
  m_tree_view->setUpdatesEnabled(true);
}
void AbstractContactList::setModelType(int type)
{
  if (!m_has_tree_view)
    return;
  m_tree_view->setUpdatesEnabled(false);
  m_proxy_model->setModelType(type);
  m_tree_view->setUpdatesEnabled(true);
}
bool AbstractContactList::getShowOffline()
{
  if (!m_has_tree_view)
    return false;
  return m_proxy_model->getShowOffline();
}
bool AbstractContactList::getShowEmptyGroup()
{
  if (!m_has_tree_view)
    return false;
  return m_proxy_model->getShowEmptyGroup();
}
bool AbstractContactList::getSortStatus()
{
  if (!m_has_tree_view)
    return false;
  return m_proxy_model->getSortStatus();
}
bool AbstractContactList::getShowSeparator()
{
  if (!m_has_tree_view)
    return false;
  return m_proxy_model->getShowSeparator();
}
int AbstractContactList::getModelType()
{
  if (!m_has_tree_view)
    return 0;
  return m_proxy_model->getModelType();
}
void AbstractContactList::selectedItemDeleted()
{
  if (!m_has_tree_view)
    return;
  QModelIndexList list = m_tree_view->selectionModel()->selectedIndexes();
  foreach(QModelIndex index, list)
  {
    QModelIndex source_index = static_cast<ProxyModelItem*>(index.internalPointer())->getSourceIndex();
    if (!source_index.isValid())
      return;
    TreeModelItem item = static_cast<TreeItem*>(source_index.internalPointer())->getStructure();
    if (item.m_item_type==TreeModelItem::Buddy||item.m_item_type==TreeModelItem::Group)
    {
      PluginSystem::instance().deleteItemSignalFromCL(item);

    }
  }
}
void AbstractContactList::selectedItemRenamed()
{
  if (!m_has_tree_view)
    return;
  QModelIndexList list = m_tree_view->selectionModel()->selectedIndexes();
  foreach(QModelIndex index, list)
  {
    QModelIndex source_index = static_cast<ProxyModelItem*>(index.internalPointer())->getSourceIndex();
    if (!source_index.isValid())
      return;
    TreeModelItem item = static_cast<TreeItem*>(source_index.internalPointer())->getStructure();
    if (item.m_item_type==TreeModelItem::Buddy||item.m_item_type==TreeModelItem::Group)
    {
      //TODO: Say plugin to show rename window for item
      m_tree_view->edit(index);
      qWarning() << (index.flags()&Qt::ItemIsEditable) << m_tree_view->editTriggers();
    }
  }
}
void AbstractContactList::loadProfile(const QString &profile_name)
{
  m_profile_name = profile_name;
  //loadSettings();
}
void AbstractContactList::loadGuiSettings()
{
}
void AbstractContactList::loadSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  m_tree_view->setUpdatesEnabled(false);

  settings.beginGroup("contactlist");
  m_tree_view->setAlternatingRowColors(settings.value("alternatingrc",false).toBool());
  QList<bool> show_icons;
  show_icons.append(settings.value("showicon0",true).toBool());
  show_icons.append(false /*settings.value("showicon1",false).toBool()*/); //Отображение аватара
  for (int i=2;i<12;i++)
    show_icons.append(settings.value("showicon"+QString::number(i),true).toBool());
  show_icons.append(settings.value("showicon12",false).toBool());
  int model_type = settings.value("modeltype",0).toInt();
  bool show_offline=settings.value("showoffline",true).toBool();
  bool show_empty_group=settings.value("showempty",true).toBool();
  bool sort_status=settings.value("sortstatus",false).toBool();
  bool show_separator=settings.value("showseparator",true).toBool();
  QString family=settings.value("accountfontfml",QFont().family()).toString();
  int size=settings.value("accountfontpnt",QFont().pointSize()).toInt();
  size=qBound(6, size, 24);
  QVariant account_font=settings.value("useaccountfont",false).toBool()?QFont(family,size):QVariant();
  family=settings.value("groupfontfml",QFont().family()).toString();
  size=settings.value("groupfontpnt",QFont().pointSize()).toInt();
  size=qBound(6, size, 24);
  QVariant group_font=settings.value("usegroupfont",false).toBool()?QFont(family,size):QVariant();
  family=settings.value("onlinefontfml",QFont().family()).toString();
  size=settings.value("onlinefontpnt",QFont().pointSize()).toInt();
  size=qBound(6, size, 24);
  QVariant online_font=settings.value("useonlinefont",false).toBool()?QFont(family,size):QVariant();
  family=settings.value("offlinefontfml",QFont().family()).toString();
  size=settings.value("offlinefontpnt",QFont().pointSize()).toInt();
  size=qBound(6, size, 24);
  QVariant offline_font=settings.value("useofflinefont",false).toBool()?QFont(family,size):QVariant();
  family=settings.value("separatorfontfml",QFont().family()).toString();
  size=settings.value("separatorfontpnt",QFont().pointSize()).toInt();
  size=qBound(6, size, 24);
  QVariant separator_font=settings.value("useseparatorfont",false).toBool()?QFont(family,size):QVariant();
  QVariant account_color=settings.value("useaccountfont",false).toBool()?settings.value("accountcolor",QVariant()):QVariant();
  QVariant group_color=settings.value("usegroupfont",false).toBool()?settings.value("groupcolor",QVariant()):QVariant();
  QVariant online_color=settings.value("useonlinefont",false).toBool()?settings.value("onlinecolor",QVariant()):QVariant();
  QVariant offline_color=settings.value("useofflinefont",false).toBool()?settings.value("offlinecolor",QVariant()):QVariant();
  QVariant separator_color=settings.value("useseparatorfont",false).toBool()?settings.value("separatorcolor",QVariant()):QVariant();
  m_proxy_model->setSettings(model_type, show_offline, show_empty_group, sort_status, show_separator, account_font, group_font, online_font, offline_font, separator_font, account_color, group_color, online_color, offline_color, separator_color);
  settings.endGroup();
  m_tree_view->setUpdatesEnabled(true);
}
void AbstractContactList::setExpanded(const QModelIndex & index, bool expanded)
{
  m_proxy_model->setExpanded(index,expanded);
}
void AbstractContactList::saveSettings()
{
  if (!m_has_tree_view)
    return;
  m_item_model->saveSettings();
  m_proxy_model->saveSettings();
}
void AbstractContactList::setItemVisible(const TreeModelItem &item, bool visible)
{
  if (!m_has_tree_view)
    return;
  m_item_model->setItemVisible(item, visible);
}

void AbstractContactList::setItemInvisible(const TreeModelItem &item, bool invisible)
{
  if (!m_has_tree_view)
    return;
  m_item_model->setItemInvisible(item, invisible);
}
