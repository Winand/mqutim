#include <QDebug>
#include <QChildEvent>

#include "messagedispatcher.h"

MessageDispatcher::MessageDispatcher(QObject *parent)
  : QObject(parent),
    m_chatfactory(NULL)
{
}

MessageDispatcher::~MessageDispatcher()
{
}

void MessageDispatcher::dispatch(const Message &msg)
{
  ChatSession *session = sessionFor(msg.author(), msg.isGroupChat()?ChatSession::Public:ChatSession::Private);
  if (!session)
  {
    qWarning() << "No session - dropping message";
    return;
  }
  session->appendMessage(msg);
}

ChatSession *MessageDispatcher::sessionFor(const TreeModelItem &item, ChatSession::Type type)
{
  QHash<QString, ChatSession *>::iterator psession = sessions.find(userId(item));
  if (psession != sessions.end())
    return psession.value();
  // Create a new one
  if (!m_chatfactory)
  {
    qWarning() << "No chat session factory: unable to create a session";
    return NULL;
  }
  else
  {
    ChatSession *session = m_chatfactory->createChatSession(item, type);
    Q_ASSERT(session);
    session->setParent(this);
    connect(session, SIGNAL(messageComposed(QString)), SLOT(messageComposed(QString)));
    connect(session, SIGNAL(typing(bool)), SLOT(userTyping(bool)));
    return session;
  }
}

void MessageDispatcher::setChatSessionFactory(ChatSessionFactory *factory)
{
  m_chatfactory = factory;
}

void MessageDispatcher::childEvent(QChildEvent *event)
{
  ChatSession *session = qobject_cast<ChatSession *>(event->child());
  if (session)
  {
    QString uid = userId(session->contact());
    if (event->added()) 
    {
      // Register this chat session
      qDebug() << "New session for" << uid;
      if (sessions.count(uid))
        qWarning() << "Duplicate sessions for" << uid; // What should we do? :(
      sessions[uid] = session;
    }
    else if (event->removed())
    {
      qDebug() << "Session for" << uid << "deleted";
      if (!sessions.count(uid))
        qWarning() << "No session for" << uid << "found";
      else
        sessions.remove(uid);
    }
  }
}

void MessageDispatcher::userTyping(bool state)
{
  ChatSession *origin = qobject_cast<ChatSession *>(sender());
  if (!origin) // message not from a chat session
  {
    qWarning() << "Some wicked object" << sender()->objectName() << "says it is" << state << "that it is typing";
    return;
  }
  Message msg;
  msg.setType(state?Message::TypingStarted:Message::TypingStopped);
  msg.setAuthor(origin->contact());
  msg.setTime(QDateTime::currentDateTime());
  msg.reg();
  sendMessage(msg);// Send it
}

void MessageDispatcher::messageComposed(const QString &text)
{
  ChatSession *origin = qobject_cast<ChatSession *>(sender());
  if (!origin) // message not from a chat session
  {
    qWarning() << "Some wicked object" << sender()->objectName() << "tried to send a message";
    return;
  }
  Message msg;
  msg.setType(origin->type()==ChatSession::Private?Message::OutgoingDialog:Message::Conference);
  msg.setIsGroupChat(origin->type()==ChatSession::Public);
  msg.setAuthor(origin->contact());
  msg.setTime(QDateTime::currentDateTime());
  msg.setText(text);
  msg.reg();
  sendMessage(msg);// Send it
}

QString MessageDispatcher::userId(const TreeModelItem &item)
{
  return QString("%1.%2.%3").arg(item.m_protocol_name).arg(item.m_account_name).arg(item.m_item_name);
}

void MessageDispatcher::sendMessage(const Message &msg)
{
  Q_UNUSED(msg);
  qWarning() << "Sending message... Could you tell me how?";
}
