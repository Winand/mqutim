#ifndef _CHATVIEW_H_
#define _CHATVIEW_H_

#include <QTextEdit>
#include "message.h"
#include "messagelistmodel.h"

class MessageListModel;

class ChatView: public QTextEdit
{
  Q_OBJECT
  public:
    ChatView(QWidget *parent = NULL);
    virtual ~ChatView();
    
    void setModel(MessageListModel *model);
    MessageListModel *model() const
    {
      return m_model;
    }
    
  private slots:
    void modelRowsInserted(const QModelIndex &parent, int start, int end);
    void modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void modelRowsAboutToBeRemoved(const QModelIndex & parent, int start, int end);
    void modelReset();
  private:
    struct MessageCache
    {
      int frameObject;
      int imagePos; 
    };    
    
    void insertMessage(const Message &message, QTextFrame *frame, MessageCache &mcache);
    void connectModel();
    void disconnectModel();
    
    QList<MessageCache> m_cache;
    QTextFrameFormat m_frameformat;
    MessageListModel *m_model;  
};

#endif // _CHATVIEW_H_

