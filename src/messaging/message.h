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
    int status;
    TreeModelItem author;
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
      Undefined,
      IncomingDialog, 
      OutgoingDialog,
      Conference,
      StatusChange,
      Error
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
    
    Type type() const;
    void setType(Type t);
    
    DeliveryStatus status() const;
    void setStatus(DeliveryStatus s);
    
    const TreeModelItem &author() const;
    void setAuthor(const TreeModelItem &a);
    
    const QDateTime &time() const;
    void setTime(const QDateTime &t);
    
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
