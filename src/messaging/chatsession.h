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
    enum Type
    {
      Public, // Multi-User chat
      Private // Private chat
    };
    
    ChatSession(const TreeModelItem &contact, Type t, QObject *parent = NULL);
    virtual ~ChatSession();
    
    /**
      Partner's address
    **/
    const TreeModelItem &contact() const;    
    /**
      Chat type
    **/
    Type type() const;
    /**
      Get message list model for this chat session
    **/
    MessageListModel *model();
    /**
      Set your partner's typing notification status
      
      Default: does nothing.
      Reimplement to show something to the user.
    **/
    virtual void notifyTyping(bool state);
    /**
      Process a message for this chat session
      
      Default: appends the message to the model. 
      Reimplement it to filter messages.
    **/
    virtual void appendMessage(const Message &msg);
    /**
      Activate this session somehow.
      
      Default: does nothing.
    **/
    virtual void activate();
    
  signals:
    /**
      Notify if user starts or stops typing 
    **/
    void typing(bool state);
    /**
      A message should be sent
    **/
    void messageComposed(const QString &text);
  private:
    ChatSession(const ChatSession &): QObject(NULL) {}; // copy prevention
    
    MessageListModel m_model;
    TreeModelItem m_contact;
    Type m_type;
};

#endif // CHATSESSION_H_
