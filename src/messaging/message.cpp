#include "message.h"

// TODO: QExplicitlySharedPointer

MessageData::MessageData()
  : type(Message::Undefined), status(Message::None), id(-1)
{
}

MessageData::MessageData(const MessageData &other)
  : QSharedData(other),
  type(other.type), is_groupchat(false),
  status(other.status), 
  author(other.author), nick(other.nick),
  time(other.time), text(other.text),
  id(-1) // This message is not registered anymore as it may differ form its parent
{
}

MessageData::~MessageData()
{
}

//---------------

uint Message::id_counter = 0;
QMutex Message::counterGuard;

Message::Message()
{
  data = new MessageData;
}

Message::~Message()
{
}

Message::Type Message::type() const
{
  return static_cast<Type>(data->type);
}

void Message::setType(Message::Type t)
{
  data->type = t;
}

bool Message::isGroupChat() const
{
  return data->is_groupchat;
}

void Message::setIsGroupChat(bool is_g)
{
  data->is_groupchat = is_g;
}

Message::DeliveryStatus Message::status() const
{
  return static_cast<DeliveryStatus>(data->status);
}

void Message::setStatus(Message::DeliveryStatus s)
{
  data->status = s;
}

const TreeModelItem &Message::author() const
{
  return data->author;
}

void Message::setAuthor(const TreeModelItem &a)
{
  data->author = a;
}

const QString &Message::nick() const
{
  return data->nick;
}

void Message::setNick(const QString &nick)
{
  data->nick = nick;
}

const QDateTime &Message::time() const
{
  return data->time;
}

void Message::setTime(const QDateTime &t)
{
  data->time = t;
}

const QString &Message::text() const
{
  return data->text;
}

void Message::setText(const QString &t)
{
  data->text = t;
}

int Message::id() const
{
  return data->id;
}

void Message::reg()
{
  if (data->id>=0)
    return; // Already registered
  QMutexLocker locker(&counterGuard);
  data->id = id_counter++;
}
