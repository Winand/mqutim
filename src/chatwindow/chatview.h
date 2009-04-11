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
    struct MessageCache // some info about row's display
    {
      int frameObject;
      int imagePos; 
    };    
    
    void updateMessageText(int messageRow); // Fill frame at messageRow with formatted message contents
    void connectModel(); // connect model's signals
    void disconnectModel(); // disconnect model's signals
    
     // Auto-scroll to bottom if log size changed and currently at bottom
    void smartScrollBegin(); // Before contents are altered
    void smartScrollEnd(); // After contents are altered
    bool m_needToScroll; // should we scroll?
    
    QList<MessageCache> m_cache;
    QTextFrameFormat m_frameformat;
    MessageListModel *m_model;  
};

#endif // _CHATVIEW_H_

