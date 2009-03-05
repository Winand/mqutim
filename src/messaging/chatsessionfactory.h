#ifndef CHATSESSIONFACTORY_H_
#define CHATSESSIONFACTORY_H_

#include <qutim/plugininterface.h>
#include "chatsession.h"

/**
  Abstract interface used to create chat sessions
**/
class ChatSessionFactory
{
  public:
    virtual ChatSession *createChatSession(const TreeModelItem &contact, ChatSession::Type type) = 0;
};

#endif // CHATSESSIONFACTORY_H_
