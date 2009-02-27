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

Qt::ItemFlags MessageListModel::flags(const QModelIndex &index) const
{
  return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

QModelIndex MessageListModel::index(int row, int column, const QModelIndex &parent) const
{
  const Message &msg = m_messages[row];
  return createIndex(row, column, (void *)(&msg));
}

int MessageListModel::rowCount(const QModelIndex &parent) const
{
  return m_messages.size();
}


bool MessageListModel::removeRows(int row, int count, const QModelIndex &parent)
{
  beginRemoveRows(parent, row, row+count-1);
  m_messages.erase(m_messages.begin()+row, m_messages.begin()+row+count);
  endRemoveRows();
}

bool MessageListModel::insertRows(int row, int count, const QModelIndex &parent)
{
  beginInsertRows(parent, row, row+count-1);
  QList<Message>::iterator pos = m_messages.begin()+row;
  Message emptyMessage;
  for (int i=0; i<count; i++)
    pos = m_messages.insert(pos, emptyMessage);
  endInsertRows();
}

//=======================

void MessageListModel::append(const Message &msg)
{
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
  return *static_cast<const Message *>(index.internalPointer());
}

