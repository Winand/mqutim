/*****************************************************************************
    Tree Contact List Model

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

#include "treecontactlistmodel.h"
#include "abstractsoundlayer.h"
#include "abstractnotificationlayer.h"
#include "abstractchatlayer.h"
#include <QtGui>

TreeContactListModel::TreeContactListModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
  QString nil_group("");
  nil_group.append(QChar(0));
  m_nil_group=nil_group;
  m_root_item = new TreeItem(headers[0]);
  m_icon_timer = new QTimer(this);
  m_icon_timer->setInterval(500);
  m_icon_timer->setObjectName("qutIM::TreeContactListModel::iconTimer");
  connect(m_icon_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
  //m_icon_timer->start();
  m_show_special_status=false;
  m_status_to_tr.insert("online",tr("Online"));
  m_status_to_tr.insert("ffc",tr("Free for chat"));
  m_status_to_tr.insert("away",tr("Away"));
  m_status_to_tr.insert("na",tr("Not available"));
  m_status_to_tr.insert("occupied",tr("Occupied"));
  m_status_to_tr.insert("dnd",tr("Do not disturb"));
  m_status_to_tr.insert("invisible",tr("Invisible"));
  m_status_to_tr.insert("offline",tr("Offline"));
  m_status_to_tr.insert("athome",tr("At home"));
  m_status_to_tr.insert("atwork",tr("At work"));
  m_status_to_tr.insert("lunch",tr("Having lunch"));
  m_status_to_tr.insert("evil",tr("Evil"));
  m_status_to_tr.insert("depression","Depression");
  m_status_to_tr.insert("noauth",tr("Without authorization"));
}
void TreeContactListModel::loadSettings(const QString & profile_name)
{
  m_profile_name = profile_name;
}
void TreeContactListModel::saveSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("contactlist");
  QList<QString> protocols = m_item_list.keys();
  foreach(const QString &protocol, protocols)
  {
    QHash<QString, TreeItem *> *v_protocol = m_item_list.value(protocol);
    QList<QString> accounts = v_protocol->keys();
    foreach(const QString &account, accounts)
    {
      settings.beginGroup(protocol+"."+account);
      TreeItem *v_account = v_protocol->value(account);
      settings.setValue("expanded",v_account->isExpanded());
      settings.setValue("grouporder",v_account->getChildOrder());
      for (int i=0;i<v_account->childCount();i++)
      {
        TreeItem *v_group = v_account->child(i);
        QString group = v_group->getName();
        if (group.isEmpty())
          settings.beginGroup(m_nil_group);
        else
          settings.beginGroup(group);
        settings.setValue("expanded",v_group->isExpanded());
        settings.endGroup();
      }
      settings.endGroup();
    }
  }
  settings.endGroup();
}

TreeContactListModel::~TreeContactListModel()
{
  delete m_root_item;
}

int TreeContactListModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant TreeContactListModel::data(const QModelIndex &index, int role) const
{
  if (index.column()!=0)
    return QVariant();
  if (!index.isValid())
    return QVariant();

  TreeItem *item = getItem(index);

  return item->data(role);
}

Qt::ItemFlags TreeContactListModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;
  if (index.data(AbstractContactList::ContactTypeRole).toInt()<2)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

TreeItem *TreeContactListModel::getItem(const QModelIndex &index) const
{
  if (index.isValid())
  {
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (item)
      return item;
  }
  return m_root_item;
}

QVariant TreeContactListModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{
  if (section!=0)
    return QVariant();
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section==0)
    return m_root_item->data(role);

  return QVariant();
}

QModelIndex TreeContactListModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() && parent.column() != 0)
    return QModelIndex();

  TreeItem *parentItem = getItem(parent);

  TreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

bool TreeContactListModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
  return false;
}

bool TreeContactListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
  TreeItem *parentItem = getItem(parent);
  bool success;

  beginInsertRows(parent, position, position + rows - 1);
  success = parentItem->insertChildren(position, rows);//m_root_item->columnCount());
  endInsertRows();
  return success;
}

QModelIndex TreeContactListModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  TreeItem *childItem = getItem(index);
  TreeItem *parentItem = childItem->parent();

  if (parentItem == m_root_item)
    return QModelIndex();

  return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeContactListModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
  return false;
}

bool TreeContactListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
  TreeItem *parent_item = getItem(parent);
  if (position<0 || position+rows>parent_item->childCount())
    return false;
  bool success = true;
  beginRemoveRows(parent, position, position + rows - 1);
  success = parent_item->removeChildren(position, rows);
  endRemoveRows();

  return success;
}

int TreeContactListModel::rowCount(const QModelIndex &parent) const
{
  TreeItem *parentItem = getItem(parent);

  return parentItem->childCount();
}

bool TreeContactListModel::setData(const QModelIndex &index, const QVariant &value,
                                   int role)
{
  if (index.column()!=0)
    return false;
  /*if (role != Qt::EditRole)
  	return false;*/

  TreeItem *item = getItem(index);

  return item->setData(value, role);
}

bool TreeContactListModel::setHeaderData(int section, Qt::Orientation orientation,
    const QVariant &value, int role)
{
  if (section!=0)
    return false;
  if (role != Qt::EditRole || orientation != Qt::Horizontal)
    return false;

  return m_root_item->setData(value, role);
}

TreeItem *TreeContactListModel::findItem(const TreeModelItem & Item)
{
  if (Item.m_item_type!=TreeModelItem::Buddy &&
      Item.m_item_type!=TreeModelItem::Group &&
      Item.m_item_type!=TreeModelItem::Account)
    return 0;
  QHash<QString, TreeItem *> *protocol = m_item_list.value(Item.m_protocol_name);
  if (!protocol)
    return 0;
  TreeItem *parent = protocol->value(Item.m_account_name);
  if (!parent)
    return 0;
  if (Item.m_item_type==TreeModelItem::Account)
    return parent;
  else if (Item.m_item_type==TreeModelItem::Buddy)
    parent = parent->find(Item.m_parent_name);
  if (!parent)
    return 0;
  TreeItem *item = parent->find(Item.m_item_name);
  if (!item)
    return 0;
  return item;
}
TreeItem *TreeContactListModel::findItemNoParent(const TreeModelItem & Item)
{
  if (Item.m_item_type!=TreeModelItem::Buddy && Item.m_item_type!=TreeModelItem::Account)
    return 0;
  QHash<QString, TreeItem *> *protocol = m_item_list.value(Item.m_protocol_name);
  if (!protocol)
    return 0;
  TreeItem *parent = protocol->value(Item.m_account_name);
  if (!parent)
    return 0;
  if (Item.m_item_type==TreeModelItem::Account)
    return parent;
  else if (Item.m_item_type==TreeModelItem::Buddy)
  {
    int num = parent->childCount();
    for (int i=0;i<num;i++)
    {
      TreeItem *group = parent->child(i);
      if (group)
      {
        TreeItem *item = group->find(Item.m_item_name);
        if (item)
          return item;
      }
    }
    return 0;
  }
  return 0;
}
QModelIndex TreeContactListModel::findIndex(const TreeModelItem & Item)
{
  return createIndex(0,0,findItem(Item));
}
bool TreeContactListModel::addAccount(const TreeModelItem & Item, QString name)
{
  //qWarning() << 0 << Item.m_account_name;
  QHash<QString, TreeItem *> *protocol = m_item_list.value(Item.m_protocol_name);
  if (!protocol)
  {
    protocol = new QHash<QString, TreeItem *>();
    m_item_list.insert(Item.m_protocol_name, protocol);
  }
  if (protocol->contains(Item.m_account_name))
    return false;
  insertRows(m_root_item->childCount(),1,QModelIndex());
  TreeItem *item = m_root_item->child(m_root_item->childCount()-1);
  item->setStructure(Item);
  item->setData(name,Qt::DisplayRole);
  item->setData(QVariant(Item.m_item_type),AbstractContactList::ContactTypeRole);
  item->setData(0, AbstractContactList::ContactMassRole);
  protocol->insert(Item.m_account_name, item);
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("contactlist");
  settings.beginGroup(Item.m_protocol_name+"."+Item.m_account_name);
  item->setExpanded(settings.value("expanded",true).toBool());
  item->setChildOrder(settings.value("grouporder",QStringList()).toStringList());
  settings.endGroup();
  settings.endGroup();
  emit itemInserted(createIndex(0,0,findItem(Item)));
  TreeModelItem double_item = Item;
  double_item.m_item_name="";
  double_item.m_item_type=TreeModelItem::Group;
  addGroup(double_item, "");
  return true;
}
bool TreeContactListModel::addGroup(const TreeModelItem & Item, QString name)
{
  //qWarning() << Item.m_item_type << Item.m_protocol_name << Item.m_account_name << Item.m_parent_name << Item.m_item_name;
  QHash<QString, TreeItem *> *protocol = m_item_list.value(Item.m_protocol_name);
  if (!protocol)
    return false;
  TreeItem *parent = protocol->value(Item.m_account_name);
  if (!parent)
    return false;
  if (parent->hasHash(Item.m_item_name))
    return false;
  insertRows(parent->childCount(),1,createIndex(0,0,parent));
  TreeItem *item = parent->child(parent->childCount()-1);
  item->setData(name,Qt::DisplayRole);
  item->setData(QVariant(Item.m_item_type),AbstractContactList::ContactTypeRole);
  item->setStructure(Item);
  int mass=0;
  if (Item.m_item_name=="")
    mass=1;
  item->setData(0, AbstractContactList::ContactMassRole);
  parent->setHash(Item.m_item_name, item);
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("contactlist");
  settings.beginGroup(Item.m_protocol_name+"."+Item.m_account_name);
  settings.beginGroup(Item.m_item_name.isEmpty()?m_nil_group:Item.m_item_name);
  item->setExpanded(settings.value("expanded",true).toBool());
  settings.endGroup();
  settings.endGroup();
  settings.endGroup();
  emit itemInserted(createIndex(0,0,findItem(Item)));
  return true;
}
bool TreeContactListModel::addBuddy(const TreeModelItem & Item, QString name)
{
  //qWarning() << 0 << Item.m_account_name << Item.m_parent_name << Item.m_item_name;
  QHash<QString, TreeItem *> *protocol = m_item_list.value(Item.m_protocol_name);
  if (!protocol)
    return false;
  TreeItem *account = protocol->value(Item.m_account_name);
  if (!account)
    return false;
  TreeItem *parent = account->find(Item.m_parent_name);
  if (!parent)
    return false;
  if (parent->hasHash(Item.m_item_name))
    return false;
  insertRows(parent->childCount(),1,createIndex(0,0,parent));
  TreeItem *item = parent->child(parent->childCount()-1);
  item->setData(name,Qt::DisplayRole);
  item->setData(QVariant(Item.m_item_type),AbstractContactList::ContactTypeRole);
  item->setData(1000, AbstractContactList::ContactMassRole);
  item->setStructure(Item);
  parent->setHash(Item.m_item_name, item);
//	emit itemInserted(createIndex(0,0,findItem(Item)));
  return true;
}
void TreeContactListModel::reinsertAllItems(TreeItem *item)
{
  if (item==0)
    item=m_root_item;
  for (int i=0;i<item->childCount();i++)
  {
    emit itemInserted(createIndex(0,0,item->child(i)));
    reinsertAllItems(item->child(i));
  }
}
void TreeContactListModel::setItemHasUnviewedContent(const TreeModelItem & Item, bool has_content)
{
  TreeItem *item = findItemNoParent(Item);
  if (item)
  {
    bool should_check = has_content?!item->hasContent():item->getContent()==1;
    item->setContent(1,has_content);
    if (should_check)
      emit checkItem(createIndex(0,0,item));
    m_mutex.lock();
    if (has_content)
      m_has_unviewed_content.insert(item,true);
    else
    {
      item->setData(item->getImage(0),AbstractContactList::ContactStatusIconRole);
      QModelIndex index = createIndex(0,0,item);
      emit dataChanged(index,index);
      m_has_unviewed_content.insert(item,false);
    }
    m_mutex.unlock();
  }
}
bool TreeContactListModel::getItemHasUnviewedContent(const TreeModelItem & Item)
{
  TreeItem *item = findItemNoParent(Item);
  if (item)
    return m_has_unviewed_content.value(item,false);
  return false;
}
void TreeContactListModel::setItemIsTyping(const TreeModelItem & Item, bool has_content)
{
  TreeItem *item = findItem(Item);
  if (item)
  {
    bool should_check = has_content?!item->hasContent():item->getContent()==2;
    item->setContent(2,has_content);
    if (should_check)
      emit checkItem(createIndex(0,0,item));
    m_mutex.lock();
    if (has_content)
      m_is_typing.insert(item,true);
    else
    {
      item->setData(item->getImage(0),AbstractContactList::ContactStatusIconRole);
      QModelIndex index = createIndex(0,0,item);
      emit dataChanged(index,index);
      m_is_typing.insert(item,false);
    }
    m_mutex.unlock();
  }
}
bool TreeContactListModel::getItemIsTyping(const TreeModelItem & Item)
{
  TreeItem *item = findItem(Item);
  if (item)
    return m_is_typing.value(item,false);
  return false;
}
void TreeContactListModel::onTimerTimeout()
{
  m_mutex.lock();
  QList<TreeItem *> keys = m_has_unviewed_content.keys();
  foreach(TreeItem *key, keys)
  {
    if (!key)
      m_has_unviewed_content.remove(key);
    else
    {
      bool special =  m_has_unviewed_content.value(key,false);
      QIcon icon(":/icons/crystal_project/message.png");
      if (m_show_special_status && special)
        key->setData(icon,AbstractContactList::ContactStatusIconRole);
      else
      {
        QPixmap pixmap(icon.actualSize(QSize(65535,65535)));
        pixmap.fill(Qt::transparent);
        icon = QIcon(pixmap);
        key->setData(icon,AbstractContactList::ContactStatusIconRole);
      }
      QModelIndex index = createIndex(0,0,key);
      if (!special)
      {
        key->setData(key->getImage(0),AbstractContactList::ContactStatusIconRole);
        m_has_unviewed_content.remove(key);
        bool should_check = key->getContent()==1;
        key->setContent(1,false);
        if (should_check)
          emit checkItem(index);
      }
      emit dataChanged(index,index);
    }
  }
  m_mutex.unlock();
  m_mutex.lock();
  keys = m_is_typing.keys();
  foreach(TreeItem *key, keys)
  {
    if (!key)
      m_is_typing.remove(key);
    else if (!m_has_unviewed_content.value(key,false))
    {
      bool special =  m_is_typing.value(key,false);
      QIcon icon(":/icons/typing.png");
      if (m_show_special_status && special)
        key->setData(icon,AbstractContactList::ContactStatusIconRole);
      else
      {
        QPixmap pixmap(icon.actualSize(QSize(65535,65535)));
        pixmap.fill(Qt::transparent);
        icon = QIcon(pixmap);
        key->setData(icon,AbstractContactList::ContactStatusIconRole);
      }
      QModelIndex index = createIndex(0,0,key);
      if (!special)
      {
        key->setData(key->getImage(0),AbstractContactList::ContactStatusIconRole);
        m_is_typing.remove(key);
        bool should_check = key->getContent()==2;
        key->setContent(2,false);
        if (should_check)
          emit checkItem(index);
      }
      emit dataChanged(index,index);
    }
  }
  m_mutex.unlock();
  m_mutex.lock();
  keys = m_changed_status.keys();
  foreach(TreeItem *key, keys)
  {
    if (!key)
      m_changed_status.remove(key);
    else
    {
      int special =  m_changed_status.value(key,-1);
      if (!m_has_unviewed_content.value(key,false) && !m_is_typing.value(key,false))
      {
        QIcon icon = key->getImage(0);
        if (m_show_special_status && special>0)
        {
          icon = QIcon(icon.pixmap(QSize(65535,65535),QIcon::Disabled));
          key->setData(icon,AbstractContactList::ContactStatusIconRole);
        }
        else
          key->setData(icon,AbstractContactList::ContactStatusIconRole);
        QModelIndex index = createIndex(0,0,key);
        emit dataChanged(index,index);
      }
      if (special<=0)
      {
        m_changed_status.remove(key);
        bool should_check = key->getContent()==4;
        key->setContent(4,false);
        if (should_check)
          emit checkItem(createIndex(0,0,key));
      }
      else
        m_changed_status.insert(key,special-m_icon_timer->interval());
    }
  }
  m_mutex.unlock();
  m_mutex.lock();
  keys = m_block_status.keys();
  foreach(TreeItem *key, keys)
  {
    if (!key)
      m_block_status.remove(key);
    else
    {
      int special =  m_block_status.value(key,-1);
      if (special<=0)
        m_block_status.remove(key);
      else
        m_block_status.insert(key,special-m_icon_timer->interval());
    }
  }
  m_mutex.unlock();
  m_show_special_status=!m_show_special_status;
}
bool TreeContactListModel::removeAccount(const TreeModelItem & Item)
{
  QHash<QString, TreeItem *> *protocol = m_item_list.value(Item.m_protocol_name);
  if (!protocol)
    return false;
  TreeItem *account = protocol->value(Item.m_account_name);
  if (!account)
    return false;
  int row = m_root_item->indexOf(account);
  if (row<0)
    return false;
  emit itemRemoved(createIndex(0,0,account));
  removeRows(row,1,QModelIndex());
  protocol->remove(Item.m_account_name);
  return true;
}
bool TreeContactListModel::removeGroup(const TreeModelItem & Item)
{
  QHash<QString, TreeItem *> *protocol = m_item_list.value(Item.m_protocol_name);
  if (!protocol)
    return false;
  TreeItem *account = protocol->value(Item.m_account_name);
  if (!account)
    return false;
  TreeItem *group = account->find(Item.m_item_name);
  if (!group)
    return false;
  int row = account->indexOf(group);
  if (row<0)
    return false;
  emit itemRemoved(createIndex(0,0,group));
  removeRows(row,1,createIndex(0,0,account));
  account->removeHash(Item.m_item_name);
  return true;
}
bool TreeContactListModel::removeBuddy(const TreeModelItem & Item)
{
  QHash<QString, TreeItem *> *protocol = m_item_list.value(Item.m_protocol_name);
  if (!protocol)
    return false;
  TreeItem *account = protocol->value(Item.m_account_name);
  if (!account)
    return false;
  TreeItem *group = account->find(Item.m_parent_name);
  if (!group)
    return false;
  TreeItem *contact = group->find(Item.m_item_name);
  m_has_unviewed_content.remove(contact);
  m_is_typing.remove(contact);
  m_changed_status.remove(contact);
  int row = group->indexOf(contact);
  if (row<0)
    return false;
  emit itemRemoved(createIndex(0,0,contact));
  removeRows(row,1,createIndex(0,0,group));
  group->removeHash(Item.m_item_name);
  return true;
}
bool TreeContactListModel::moveBuddy(const TreeModelItem & oldItem, const TreeModelItem & newItem)
{
  TreeItem *old_item = findItem(oldItem);
  QString status = old_item->data(AbstractContactList::ContactStatusRole).toString();

  return true;
}
bool TreeContactListModel::setItemName(const TreeModelItem & Item, QString name)
{
  if (name.isEmpty())
    return false;
  TreeItem *item = findItem(Item);
  if (item==0)
    return false;
  item->setData(name,Qt::DisplayRole);
  emit itemNameChanged(createIndex(0,0,findItem(Item)), name);
  QModelIndex index = createIndex(0,0,item);
  emit dataChanged(index,index);
  return true;
}
bool TreeContactListModel::setItemIcon(const TreeModelItem & Item, QIcon icon, int position)
{
  TreeItem *item = findItem(Item);
  if (item==0 || (position==0 && Item.m_item_type==TreeModelItem::Buddy))
    return false;
  item->setImage(icon,position);
  QModelIndex index = createIndex(0,0,item);
  emit dataChanged(index,index);
  return true;
}
bool TreeContactListModel::setItemRow(const TreeModelItem & Item, QList<QVariant> var, int row)
{
  TreeItem *item = findItem(Item);
  if (item==0)
    return false;
  item->setRow(QVariant(var), row);
  QModelIndex index = createIndex(0,0,item);
  emit dataChanged(index,index);
  return true;
}
bool TreeContactListModel::setItemStatus(const TreeModelItem & Item, QIcon icon, QString status, int mass)
{
  TreeItem *item = findItem(Item);
  if (item==0 || Item.m_item_type!=TreeModelItem::Buddy)
    return false;
  bool new_item = item->data(AbstractContactList::ContactStatusRole).toString().isEmpty();
  int old_mass = item->data(AbstractContactList::ContactMassRole).toInt();
  item->setStatus(status, icon, mass);
  QModelIndex index = createIndex(0,0,item);
  if (Item.m_item_type==TreeModelItem::Buddy)
  {
    if (old_mass==1000 && mass<1000)
      item->parent()->m_visible++;
    else if (old_mass<1000 && mass==1000)
      item->parent()->m_visible--;
  }
  if (!new_item && Item.m_item_type==TreeModelItem::Buddy)
  {
    if (m_block_status.value(item->parent()->parent(),0)<1)
    {
      AbstractSoundLayer &asl = AbstractSoundLayer::instance();
      AbstractNotificationLayer &anl = AbstractNotificationLayer::instance();
      AbstractChatLayer &acl = AbstractChatLayer::instance();
      QString tr_status = m_status_to_tr.value(status,status);
      if (old_mass==1000 && mass<1000)
      {
        anl.userMessage(Item,tr_status, NotifyOnline);
        acl.addServiceMessage(Item,tr_status);
        asl.playSound(NotifyOnline);
      }
      else if (old_mass<1000 && mass==1000)
      {
        anl.userMessage(Item,tr_status, NotifyOffline);
        acl.addServiceMessage(Item,tr_status);
        asl.playSound(NotifyOffline);
      }
      else if (old_mass!=mass)
      {
        anl.userMessage(Item,tr_status, NotifyStatusChange);
        asl.playSound(NotifyStatusChange);
      }
      m_mutex.lock();
      m_changed_status.insert(item,5000);
      bool should_check = !item->hasContent();
      item->setContent(4,true);
      if (should_check)
        emit checkItem(index);
      m_mutex.unlock();
    }
  }
  //qWarning() << item->parent()->m_visible;
  emit itemStatusChanged(createIndex(0,0,findItem(Item)), icon, status, mass);
  emit dataChanged(index,index);
  return true;
}
void TreeContactListModel::setItemIsOnline(const TreeModelItem & Item, bool online)
{
  TreeItem *item = findItem(Item);
  if (item==0 || Item.m_item_type!=TreeModelItem::Account)
    return;
  if (item->getOnline()!=online)
  {
    m_mutex.lock();
    m_block_status.insert(item,5000);
    m_mutex.unlock();
    item->setOnline(online);
  }
}
void TreeContactListModel::setItemVisible(const TreeModelItem &Item, bool visible)
{
  TreeItem *item = findItem(Item);
  if (item==0 || Item.m_item_type==TreeModelItem::Account)
    return;
  item->setAlwaysVisible(visible);
  QModelIndex index = createIndex(0,0,item);
//	emit itemRemoved(index,index);
//	emit itemInserted(index,index);
}
void TreeContactListModel::setItemInvisible(const TreeModelItem &Item, bool invisible)
{
  TreeItem *item = findItem(Item);
  if (item==0 || Item.m_item_type==TreeModelItem::Account)
    return;
  item->setAlwaysInvisible(invisible);
  QModelIndex index = createIndex(0,0,item);
//	emit itemRemoved(index,index);
//	emit itemInserted(index,index);
}
