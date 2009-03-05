#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <QSharedData>
#include <QSharedDataPointer>
#include <QString>
#include <QDateTime>
#include <QMutex>

#include <qutim/plugininterface.h> // for TreeModelItem

using qutim_sdk_0_2::TreeModelItem;

class MessageData: public QSharedData
{
  public:
    MessageData();
    MessageData(const MessageData &other);
    ~MessageData();
    
    int type;
    bool is_groupchat;
    int status;
    TreeModelItem author;
    QString nick;
    QDateTime time;
    QString text;
    int id;
};

/**
  \brief Message abstraction
  
  Provides unified access to chat log entries.
  Implicit sharing used. All members are reentrant.
**/
class Message
{
  public:
    enum Type
    {
      Undefined, // Only makes sense for uninitialized messages. Normally such message will be ignored.
      IncomingDialog, // Incoming regular chat message
      OutgoingDialog, // Outgoing regular chat message
      Conference, // Regular conference message
      StatusChange, // A participant has changed his status
      Action, // A participant has done some action
      Error, // Some technical error
      Notification, // Some synthetic notification
      TypingStarted, // User started typing a message
      TypingStopped // User stopped typing a message
    };
    enum DeliveryStatus
    {
      None,
      Received,
      Sending,
      Server,
      Recipient
    };
    
    Message();
    virtual ~Message();
    
    /**
      Message type
    **/
    Type type() const;
    void setType(Type t);
    
    /**
      Is this a groupchat message?
    **/
    bool isGroupChat() const;
    void setIsGroupChat(bool is_g);
    
    /**
      Message delivery status
    **/
    DeliveryStatus status() const;
    void setStatus(DeliveryStatus s);
    
    /**
      Message author id. Used to find a corresponding chat session.
      For conference public messages, this should be conference's id,
      real author is determined by 'nick' property.
    **/
    const TreeModelItem &author() const;
    void setAuthor(const TreeModelItem &a);
    
    /**
      Specify person's nick. Actual [only] for conferences.
    **/
    const QString &nick() const;
    void setNick(const QString &nick);
    
    /**
      Message timestamp.
    **/
    const QDateTime &time() const;
    void setTime(const QDateTime &t);
    
    /**
      Message contents
    **/
    const QString &text() const;
    void setText(const QString &t);
    
    /**
      Unique id that is assigned to each registered message to provide callback from protocols.
    **/
    int id() const;
    
    /**
      Assign a unique id to this message. A registered message won't be registered second time.
    **/
    void reg();
    
  protected:    
    QSharedDataPointer<MessageData> data;
    
    static uint id_counter;
    static QMutex counterGuard;
};

#endif // MESSAGE_H_
