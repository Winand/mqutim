#ifndef CHATSESSION_H_
#define CHATSESSION_H_

#include <QObject>

#include <qutim/plugininterface.h>
#include "messagelistmodel.h"

/**
  Abstract chat session interface.
  Incapsulated methods a real chat session must implement.
**/
class ChatSession: public QObject
{
  Q_OBJECT
  public:
    ChatSession(const TreeModelItem &contact, QObject *parent = NULL);
    virtual ~ChatSession();
    
    /**
      Partner's address
    **/
    const TreeModelItem &contact();    
    /**
      Get message list model for this chat session
    **/
    MessageListModel *model();
    /**
      Set your partner's typing notification status
    **/
    virtual notifyTyping(bool state);
    
  signals:
    /**
      Notify if user starts or stops typing 
    **/
    typing(bool state);
    /**
      A message should be sent
    **/
    message(const QString &text);
  private:
    ChatSession(const ChatSession &) {}; // copy prevention
    
    MessageListModel m_model;
    TreeModelItem m_contact;
};

#endif // CHATSESSION_H_
