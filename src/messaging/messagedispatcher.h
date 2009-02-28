#ifndef MESSAGEDISPATCHER_H_
#define MESSAGEDISPATCHER_H_

#include <QObject>

#include "chatsession.h"
#include "chatsessionfactory.h"
#include "message.h"

class MessageDispatcher: public QObject
{
  Q_OBJECT
  public:
    MessageDispatcher(QObject *parent);
    virtual ~MessageDispatcher();
    
    /**
      Process and route incoming message
    **/
    void dispatch(const Message &);
    void setChatSessionFactory(ChatSessionFactory *factory);
    
  protected:
    void childEvent(QChildEvent *event);
    
  private slots:
    void userTyping(bool state);
    void messageComposed(const QString &message);
};

#endif // MESSAGEDISPATCHER_H_
