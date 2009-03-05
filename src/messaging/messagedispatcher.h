#ifndef MESSAGEDISPATCHER_H_
#define MESSAGEDISPATCHER_H_

#include <QObject>
#include <QString>
#include <QHash>

#include "chatsession.h"
#include "chatsessionfactory.h"
#include "message.h"

class MessageDispatcher: public QObject
{
  Q_OBJECT
  public:
    MessageDispatcher(QObject *parent = NULL);
    virtual ~MessageDispatcher();
    
    /**
      Process and route incoming message
    **/
    void dispatch(const Message &);
    /** 
      Install a chat session factory.
      By default there's no factory, all incoming messages are dropped.
    **/
    void setChatSessionFactory(ChatSessionFactory *factory);
    /**
      Return a session for specified item.
      If no corresponding sessions exist, a new one is created.
    **/
    ChatSession *sessionFor(const TreeModelItem &item, ChatSession::Type type);
    
  protected:
    void childEvent(QChildEvent *event);
    
  private slots:
    void userTyping(bool state);
    void messageComposed(const QString &message);
  
  private:
    QString userId(const TreeModelItem &item); // Provide a userId 
    void sendMessage(const Message &msg);
    
    QHash<QString, ChatSession *> sessions;
    ChatSessionFactory *m_chatfactory;
};

#endif // MESSAGEDISPATCHER_H_
