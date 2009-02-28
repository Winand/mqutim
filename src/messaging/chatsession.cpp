#include "chatsession.h"

ChatSession::ChatSession(const TreeModelItem &contact, QObject *parent = NULL)
: QObject(parent), m_model(this), m_contact(contact)
{
}

ChatSession::~ChatSession()
{
}
    
const TreeModelItem &ChatSession::contact()
{
  return m_contact;
}

MessageListModel *ChatSession::model()
{
  return &m_model;
}

ChatSession::notifyTyping(bool state)
{
}