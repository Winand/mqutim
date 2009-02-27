#ifndef MESSAGELISTMODEL_H_
#define MESSAGELISTMODEL_H_

#include <QAbstractListModel>

#include "message.h"

/**
  A model to access a message list.
  This model can be either fixed size when it preserves <i>n</i> last items
  or infinite-sized when it can preserve <i>[theoretically]</i> infinite number of items
**/
class MessageListModel: public QAbstractListModel
{
  Q_OBJECT
  public:
    /**
      Creates an infinite-sized model
    **/
    MessageListModel(QObject *parent = NULL); 
    virtual ~MessageListModel();
    
    // Specific interface
    void append(const Message &msg);
    bool isFixedSize() const;
    uint fixedSize() const; // Returns 0 for infinite sized model
    void setFixedSize(uint size);
    void setInfiniteSize();
    const Message &message(const QModelIndex &index) const;
    
    // AbstractItemModel implementation
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
  private:
    QList<Message> m_messages;
    bool m_isFixedSize; // Preserve only last m_fixedSize messages
    uint m_fixedSize; 
};

#endif // MESSAGELISTMODEL_H_
