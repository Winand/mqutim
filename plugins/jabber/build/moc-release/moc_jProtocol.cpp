/****************************************************************************
** Meta object code from reading C++ file 'jProtocol.h'
**
** Created: Sun Feb 15 15:01:25 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jProtocol.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jProtocol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jProtocol[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      20,   11,   10,   10, 0x05,
      42,   39,   10,   10, 0x05,
      89,   39,   10,   10, 0x05,
     143,  136,   10,   10, 0x05,
     205,  191,   10,   10, 0x05,
     257,  242,   10,   10, 0x05,
     357,  300,   10,   10, 0x05,
     443,  422,   10,   10, 0x05,
     510,  479,   10,   10, 0x05,
     576,  553,   10,   10, 0x05,
     634,  620,   10,   10, 0x05,
     687,  680,   10,   10, 0x05,
     728,  712,   10,   10, 0x05,
     783,  768,   10,   10, 0x05,
     852,  826,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     906,  894,   10,   10, 0x0a,
     934,   10,   10,   10, 0x0a,
     946,   10,   10,   10, 0x0a,
     962,   10,   10,   10, 0x0a,
     972,  826,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_jProtocol[] = {
    "jProtocol\0\0presence\0setRealStatus(int)\0"
    ",,\0setClientCustomIcon(TreeModelItem,QString,int)\0"
    "setClientCustomText(TreeModelItem,QString,int)\0"
    "item,,\0addMessageFrom(TreeModelItem,QDateTime,QString)\0"
    "item,position\0messageDelievered(TreeModelItem,int)\0"
    "item,composing\0"
    "sendTypingNotification(TreeModelItem,bool)\0"
    "protocol_name,conference_name,account_name,nickname,name\0"
    "conferenceClientVersion(QString,QString,QString,QString,QString)\0"
    "account_name,message\0"
    "systemNotification(QString,QString)\0"
    "m_account_name,vcard,avatarUrl\0"
    "onFetchVCard(QString,const VCard*,QString)\0"
    "text,jid,jabber_client\0"
    "createAcceptAuthDialog(QString,JID,Client*)\0"
    "b_list,c_list\0"
    "bookmarksHandled(BookmarkList,ConferenceList)\0"
    "xml,in\0tagHandled(QString,bool)\0"
    "from,items,type\0receiveSDisco(JID,Disco::Items,QString)\0"
    "from,info,type\0"
    "receiveSDiscoInfo(JID,Disco::Info,QString)\0"
    "room,from,reason,password\0"
    "conferenceInvite(JID,JID,QString,QString)\0"
    "from,stanza\0addMessageFrom(JID,Message)\0"
    "onSetMood()\0onSetActivity()\0onTimer()\0"
    "receiveInvite(JID,JID,QString,QString)\0"
};

const QMetaObject jProtocol::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_jProtocol,
      qt_meta_data_jProtocol, 0 }
};

const QMetaObject *jProtocol::metaObject() const
{
    return &staticMetaObject;
}

void *jProtocol::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jProtocol))
	return static_cast<void*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "RosterListener"))
	return static_cast< RosterListener*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "ConnectionListener"))
	return static_cast< ConnectionListener*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "MessageHandler"))
	return static_cast< MessageHandler*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "PresenceHandler"))
	return static_cast< PresenceHandler*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "IqHandler"))
	return static_cast< IqHandler*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "DiscoHandler"))
	return static_cast< DiscoHandler*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "VCardHandler"))
	return static_cast< VCardHandler*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "SubscriptionHandler"))
	return static_cast< SubscriptionHandler*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "BookmarkHandler"))
	return static_cast< BookmarkHandler*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "LogHandler"))
	return static_cast< LogHandler*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "PubSub::EventHandler"))
	return static_cast< PubSub::EventHandler*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "PubSub::ResultHandler"))
	return static_cast< PubSub::ResultHandler*>(const_cast< jProtocol*>(this));
    if (!strcmp(_clname, "gloox::EventHandler"))
	return static_cast< gloox::EventHandler*>(const_cast< jProtocol*>(this));
    return QThread::qt_metacast(_clname);
}

int jProtocol::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setRealStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: setClientCustomIcon((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: setClientCustomText((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: addMessageFrom((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QDateTime(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 4: messageDelievered((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: sendTypingNotification((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: conferenceClientVersion((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 7: systemNotification((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: onFetchVCard((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const VCard*(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 9: createAcceptAuthDialog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const JID(*)>(_a[2])),(*reinterpret_cast< Client*(*)>(_a[3]))); break;
        case 10: bookmarksHandled((*reinterpret_cast< const BookmarkList(*)>(_a[1])),(*reinterpret_cast< const ConferenceList(*)>(_a[2]))); break;
        case 11: tagHandled((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 12: receiveSDisco((*reinterpret_cast< const JID(*)>(_a[1])),(*reinterpret_cast< const Disco::Items(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 13: receiveSDiscoInfo((*reinterpret_cast< const JID(*)>(_a[1])),(*reinterpret_cast< const Disco::Info(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 14: conferenceInvite((*reinterpret_cast< const JID(*)>(_a[1])),(*reinterpret_cast< const JID(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 15: addMessageFrom((*reinterpret_cast< const JID(*)>(_a[1])),(*reinterpret_cast< const Message(*)>(_a[2]))); break;
        case 16: onSetMood(); break;
        case 17: onSetActivity(); break;
        case 18: onTimer(); break;
        case 19: receiveInvite((*reinterpret_cast< const JID(*)>(_a[1])),(*reinterpret_cast< const JID(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        }
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void jProtocol::setRealStatus(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void jProtocol::setClientCustomIcon(const TreeModelItem & _t1, const QString & _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void jProtocol::setClientCustomText(const TreeModelItem & _t1, const QString & _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void jProtocol::addMessageFrom(const TreeModelItem & _t1, const QDateTime & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void jProtocol::messageDelievered(const TreeModelItem & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void jProtocol::sendTypingNotification(const TreeModelItem & _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void jProtocol::conferenceClientVersion(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void jProtocol::systemNotification(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void jProtocol::onFetchVCard(const QString & _t1, const VCard * _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void jProtocol::createAcceptAuthDialog(const QString & _t1, const JID & _t2, Client * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void jProtocol::bookmarksHandled(const BookmarkList & _t1, const ConferenceList & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void jProtocol::tagHandled(const QString & _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void jProtocol::receiveSDisco(const JID & _t1, const Disco::Items & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void jProtocol::receiveSDiscoInfo(const JID & _t1, const Disco::Info & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void jProtocol::conferenceInvite(const JID & _t1, const JID & _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}
