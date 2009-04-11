
#include <QListView>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QFrame>

#include <QtopiaItemDelegate>

#include "qutim.h"
#include "chatsession_qtopia.h"
#include "chatview.h"

QtopiaChatSession::QtopiaChatSession(const TreeModelItem &contact, Type t, QObject *parent)
  : ChatSession(contact, t, parent), ui(NULL)
{
  qutIM::getInstance()->addTab(&ui, contact.m_item_name);
  
  QVBoxLayout *layout = new QVBoxLayout(&ui);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
#if 0
  QListView *chatlog = new QListView(&ui);
  chatlog->setFrameStyle(QFrame::NoFrame);
  chatlog->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  chatlog->setModel(model());
  chatlog->setItemDelegate(new QtopiaItemDelegate(chatlog));
#else
  ChatView *chatlog = new ChatView(&ui);
  chatlog->setModel(model());
  chatlog->setFrameStyle(QFrame::NoFrame);
#endif
  chatlog->setFocusPolicy(Qt::NoFocus);
  QFrame *delimiter = new QFrame(&ui);
  delimiter->setFixedHeight(1);
  delimiter->setFrameStyle(QFrame::HLine | QFrame::Plain);
  QTextEdit *input = new QTextEdit(&ui);
  input->setFrameStyle(QFrame::NoFrame);
  input->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  
  layout->addWidget(chatlog);
  layout->addWidget(delimiter);
  layout->addWidget(input);
  layout->setStretchFactor(chatlog, 5);
  layout->setStretchFactor(input, 1);
  
  ui.setFocusProxy(input);
  chatlog->setFocusProxy(input);
}

QtopiaChatSession::~QtopiaChatSession()
{
}

// Reimplemented from ChatSession
void QtopiaChatSession::notifyTyping(bool state)
{
}

void QtopiaChatSession::appendMessage(const Message &msg)
{
  ChatSession::appendMessage(msg);
}

void QtopiaChatSession::activate()
{
  qutIM::getInstance()->setCurrentWidget(&ui);
}

// Factory
ChatSession *QtopiaChatSessionFactory::
              createChatSession(const TreeModelItem &contact, ChatSession::Type type)
{
  return new QtopiaChatSession(contact, type, this);
}

QtopiaChatSessionFactory::QtopiaChatSessionFactory(QObject *parent)
  : QObject(parent)
{
}

QtopiaChatSessionFactory::~QtopiaChatSessionFactory()
{
}
