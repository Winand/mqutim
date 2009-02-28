#ifndef CHATSESSIONFACTORY_H_
#define CHATSESSIONFACTORY_H_

#include <qutim/plugininterface.h>
#include "chatsession.h"

/**
  Abstract interface used to create chat sessions
**/
class ChatSessionFactory
{
  virtual ChatSession *createChatSession(const TreeModelItem *contact) = 0;
};

#endif // CHATSESSIONFACTORY_H_
