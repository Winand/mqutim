#include <QDebug>

#include "messagelistmodel.h"

MessageListModel::MessageListModel(QObject *parent)
  : QAbstractListModel(parent),
    m_isFixedSize(false), m_fixedSize(0)
{
}

MessageListModel::~MessageListModel()
{
}

QVariant MessageListModel::data(const QModelIndex &index, int role) const
{
  switch (role)
  {
    case Qt::DisplayRole:
      return m_messages[index.row()].text();
    default:
      return QVariant();
  }
}

Qt::ItemFlags MessageListModel::flags(const QModelIndex &) const
{
  return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

QModelIndex MessageListModel::index(int row, int column, const QModelIndex &) const
{
  return createIndex(row, column);
  /* // FIXME: Do we need it?
  if (row>=rowCount())
  {
    qWarning() << __PRETTY_FUNCTION__ << "Index out of range (" << row << ">=" << rowCount() << ")";
    return createIndex(row, column);
  }
  const Message &msg = m_messages[row];
  return createIndex(row, column, (void *)(&msg));
  */
}

int MessageListModel::rowCount(const QModelIndex &) const
{
  return m_messages.size();
}

bool MessageListModel::removeRows(int row, int count, const QModelIndex &parent)
{
  beginRemoveRows(parent, row, row+count-1);
  m_messages.erase(m_messages.begin()+row, m_messages.begin()+row+count);
  endRemoveRows();
  return true;
}

bool MessageListModel::insertRows(int row, int count, const QModelIndex &parent)
{
  beginInsertRows(parent, row, row+count-1);
  QList<Message>::iterator pos = m_messages.begin()+row;
  Message emptyMessage;
  for (int i=0; i<count; i++)
    pos = m_messages.insert(pos, emptyMessage);
  endInsertRows();
  return true;
}

//=======================

void MessageListModel::append(const Message &msg)
{
  insertRow(rowCount());
  m_messages.last() = msg;
  QModelIndex i = index(rowCount()-1, 0);
  emit dataChanged(i, i);
  if (m_isFixedSize)
  {
    int rowsToBeDeleted = rowCount()-m_fixedSize;
    if (rowsToBeDeleted<=0)
      return; // nothing to delete
    removeRows(0, rowsToBeDeleted);
  }
}

bool MessageListModel::isFixedSize() const
{
  return m_isFixedSize;
}

uint MessageListModel::fixedSize() const
{
  return m_fixedSize;
}

void MessageListModel::setFixedSize(uint size)
{
  m_isFixedSize = true;
  m_fixedSize = size;
}

void MessageListModel::setInfiniteSize()
{
  m_isFixedSize = false;
  m_fixedSize = 0;
}

const Message &MessageListModel::message(const QModelIndex &index) const
{
  //return *static_cast<const Message *>(index.internalPointer());
  if (index.row()>=rowCount())
  {
    qWarning() << "MessageListModel::message(): index out of range";
    return Message();
  }
  return m_messages[index.row()];
}

