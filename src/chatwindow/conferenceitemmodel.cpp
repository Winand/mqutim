/*
    Conference Item Model

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

#include "conferenceitemmodel.h"

ConferenceItemModel::ConferenceItemModel(ConferenceContactList *contact_list, QObject *parent)
    : QAbstractItemModel(parent)
{
    qDebug("ConferenceItemModel::ConferenceItemModel1");
  m_contact_list = contact_list;
      qDebug("ConferenceItemModel::ConferenceItemModel2");
}

ConferenceItemModel::~ConferenceItemModel()
{
}

QVariant ConferenceItemModel::data(const QModelIndex &index, int role) const
{
  if (index.column()!=0)
    return QVariant();
  if (!index.isValid())
    return QVariant();

  ConferenceItem *item = getItem(index);

  return item->data(role);
}

QVariant ConferenceItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (section!=0)
    return QVariant();
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section==0)
    return QString("test");

  return QVariant();
}

QModelIndex ConferenceItemModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() || m_item_list.size()<=row || row<0)
    return QModelIndex();
  else
    return createIndex(row, column, m_item_list[row]);
}

QModelIndex ConferenceItemModel::parent(const QModelIndex &index) const
{
  return QModelIndex();
}

int ConferenceItemModel::rowCount(const QModelIndex &parent) const
{
  return m_item_list.size();
}

int ConferenceItemModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

Qt::ItemFlags ConferenceItemModel::flags(const QModelIndex &index) const
{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool ConferenceItemModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
  return false;
}

bool ConferenceItemModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
  return false;
}

bool ConferenceItemModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
  return false;
}

bool ConferenceItemModel::insertRows(int position, int rows, const QModelIndex &parent)
{
qDebug("11");  if (position>m_item_list.size()+1)
    return false;
qDebug("12");  beginInsertRows(parent, position, position + rows - 1);
  for (int i=position;i<position+rows;i++) {
    qDebug("inserting");
    m_item_list.insert(i,new ConferenceItem("",m_contact_list));}
qDebug("13");  endInsertRows();
qDebug("14");  return true;
}

bool ConferenceItemModel::removeRows(int position, int rows, const QModelIndex &parent)
{
  if (position<0 || position+rows>m_item_list.size())
    return false;

  beginRemoveRows(parent, position, position + rows - 1);
  for (int i=0;i<rows;i++)
  {
    m_item_hash.remove(m_item_list[position]->data(Qt::DisplayRole).toString());
    delete m_item_list[position];
    m_item_list.removeAt(position);
  }
  endRemoveRows();
  return true;
}

bool ConferenceItemModel::addBuddy(const QString & name)
{
  if (!insertRows(0,1,QModelIndex()))
    return false;
  if (m_item_hash.contains(name))
    return false;
  ConferenceItem *item = m_item_list[0];
  item->setData(name, Qt::DisplayRole);
  m_item_hash[name] = item;
qDebug("0");  //position(item);
qDebug("1");  return true;
}

bool ConferenceItemModel::removeBuddy(const QString & name)
{
  ConferenceItem *item = m_item_hash.value(name,0);
  if (!item)
    return false;
  return removeRows(m_item_list.indexOf(item),1,QModelIndex());
}

bool ConferenceItemModel::renameBuddy(const QString & name, const QString & new_name)
{
  ConferenceItem *item = m_item_hash.value(name,0);
  if (!item)
    return false;
  m_item_hash.remove(name);
  item->setData(new_name,Qt::DisplayRole);
  m_item_hash.insert(new_name,item);
  position(item);
  return true;
}

bool ConferenceItemModel::setItemIcon(const QString & name, const QIcon & icon, int position)
{
  ConferenceItem *item = m_item_hash.value(name,0);
  if (!item)
    return false;
  item->setImage(icon,position);
  QModelIndex index = createIndex(m_item_list.indexOf(item),0,item);
  emit dataChanged(index,index);
  return true;
}

bool ConferenceItemModel::setItemRow(const QString & name, const QList<QVariant> & var, int row)
{
  ConferenceItem *item = m_item_hash.value(name,0);
  if (!item)
    return false;
  item->setRow(QVariant(var), row);
  QModelIndex index = createIndex(m_item_list.indexOf(item),0,item);
  emit dataChanged(index,index);
  return true;
}

bool ConferenceItemModel::setItemStatus(const QString & name, const QIcon & icon, const QString & status, int mass)
{
  ConferenceItem *item = m_item_hash.value(name,0);
  if (!item)
    return false;
  item->setStatus(status, icon, mass);
  position(item);
  QModelIndex index = createIndex(m_item_list.indexOf(item),0,item);
  emit dataChanged(index,index);
  return true;
}

bool ConferenceItemModel::setItemRole(const QString & name, const QIcon & icon, const QString & role, int mass)
{
  ConferenceItem *item = m_item_hash.value(name,0);
  if (!item)
    return false;
  item->setRole(role, icon, mass);
  position(item);
  QModelIndex index = createIndex(m_item_list.indexOf(item),0,item);
  emit dataChanged(index,index);
  return true;
}

bool ConferenceItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.column()!=0)
    return false;

  ConferenceItem *item = getItem(index);

  return item->setData(value, role);
}

ConferenceItem *ConferenceItemModel::getItem(const QModelIndex &index) const
{
  if (index.isValid())
  {
    ConferenceItem *item = static_cast<ConferenceItem*>(index.internalPointer());
    if (item)
      return item;
  }
  return 0;
}

void ConferenceItemModel::position(ConferenceItem *item)
{
  emit layoutAboutToBeChanged();
  int mass = item->getMass();
  m_item_list.removeAll(item);
  QString string = item->data(Qt::DisplayRole).toString();
  for (int i=0;i<m_item_list.size();i++)
  {
    int item_mass=m_item_list[i]->getMass();
    if (item_mass>mass)
    {
      m_item_list.insert(i,item);
      emit layoutChanged();
      return;
    }
    else if (item_mass==mass && m_item_list[i]->data(Qt::DisplayRole).toString().compare(string,Qt::CaseInsensitive)>0)
    {
      m_item_list.insert(i,item);
      emit layoutChanged();
      return;
    }
  }
  m_item_list.append(item);
  emit layoutChanged();
}

QStringList ConferenceItemModel::getUsers()
{
  return m_item_hash.keys();
}
