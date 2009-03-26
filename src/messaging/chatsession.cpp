#include "chatsession.h"

ChatSession::ChatSession(const TreeModelItem &contact, ChatSession::Type t, QObject *parent)
: QObject(parent), m_model(this), m_contact(contact), m_type(t)
{
}

ChatSession::~ChatSession()
{
}
    
const TreeModelItem &ChatSession::contact() const
{
  return m_contact;
}

ChatSession::Type ChatSession::type() const
{
  return m_type;
}

MessageListModel *ChatSession::model()
{
  return &m_model;
}

void ChatSession::notifyTyping(bool state)
{
}

void ChatSession::appendMessage(const Message &msg)
{
  m_model.append(msg);
}

void ChatSession::activate()
{
}

