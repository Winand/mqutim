#ifndef CHATSESSION_QTOPIA_H_
#define CHATSESSION_QTOPIA_H_

#include "chatsession.h"
#include "chatsessionfactory.h"

class QtopiaChatSessionFactory: public QObject, public ChatSessionFactory
{
  Q_OBJECT
  public:
    QtopiaChatSessionFactory(QObject *parent = NULL);
    virtual ~QtopiaChatSessionFactory();
    ChatSession *createChatSession(const TreeModelItem &contact, ChatSession::Type type);
};

class QtopiaChatSession: public ChatSession
{
  Q_OBJECT
  public:
    QtopiaChatSession(const TreeModelItem &contact, Type t, QObject *parent = NULL);
    virtual ~QtopiaChatSession();
    
    // Reimplemented from ChatSession
    void notifyTyping(bool state);
    void appendMessage(const Message &msg);
    void activate();
  private:
    QWidget ui;
};

#endif // CHATSESSION_QTOPIA_H_

