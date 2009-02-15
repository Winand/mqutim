/****************************************************************************
** Meta object code from reading C++ file 'snacchannel.h'
**
** Created: Sat Feb 14 22:21:01 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../snacchannel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'snacchannel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_snacChannel[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      26,   12,   12,   12, 0x05,
      38,   12,   12,   12, 0x05,
      53,   12,   12,   12, 0x05,
      77,   12,   12,   12, 0x05,
     100,   12,   12,   12, 0x05,
     121,   12,   12,   12, 0x05,
     149,   12,   12,   12, 0x05,
     170,   12,   12,   12, 0x05,
     193,   12,   12,   12, 0x05,
     205,   12,   12,   12, 0x05,
     221,  219,   12,   12, 0x05,
     252,  219,   12,   12, 0x05,
     282,   12,   12,   12, 0x05,
     302,   12,   12,   12, 0x05,
     322,  219,   12,   12, 0x05,
     349,   12,   12,   12, 0x05,
     373,   12,   12,   12, 0x05,
     396,   12,   12,   12, 0x05,
     413,   12,   12,   12, 0x05,
     437,   12,   12,   12, 0x05,
     470,   12,   12,   12, 0x05,
     492,   12,   12,   12, 0x05,
     519,   12,   12,   12, 0x05,
     543,   12,   12,   12, 0x05,
     576,   12,   12,   12, 0x05,
     613,   12,   12,   12, 0x05,
     636,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     664,   12,  656,   12, 0x08,
     682,   12,   12,   12, 0x08,
     699,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_snacChannel[] = {
    "snacChannel\0\0incFlapSeq()\0incReqSeq()\0"
    "rereadSocket()\0sendAuthKey(QByteArray)\0"
    "systemMessage(QString)\0userMessage(QString)\0"
    "sendBosServer(QHostAddress)\0"
    "sendBosPort(quint16)\0sendCookie(QByteArray)\0"
    "connected()\0getList(bool)\0,\0"
    "oncomingBuddy(QString,quint16)\0"
    "offlineBuddy(QString,quint16)\0"
    "getMessage(quint16)\0getOfflineMessage()\0"
    "readMetaData(quint16,bool)\0"
    "getTypingNotif(quint16)\0readSSTserver(quint16)\0"
    "blockRateLimit()\0getStatusCheck(quint16)\0"
    "getModifyItemFromServer(quint16)\0"
    "youWereAdded(quint16)\0getUploadIconData(quint16)\0"
    "getAwayMessage(quint16)\0"
    "getAuthorizationRequest(quint16)\0"
    "authorizationAcceptedAnswer(quint16)\0"
    "getMessageAck(quint16)\0clearPrivacyLists()\0"
    "quint32\0returnSnacReqId()\0resetReqSnaqId()\0"
    "incReq()\0"
};

const QMetaObject snacChannel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_snacChannel,
      qt_meta_data_snacChannel, 0 }
};

const QMetaObject *snacChannel::metaObject() const
{
    return &staticMetaObject;
}

void *snacChannel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_snacChannel))
	return static_cast<void*>(const_cast< snacChannel*>(this));
    return QObject::qt_metacast(_clname);
}

int snacChannel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: incFlapSeq(); break;
        case 1: incReqSeq(); break;
        case 2: rereadSocket(); break;
        case 3: sendAuthKey((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: systemMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: userMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: sendBosServer((*reinterpret_cast< const QHostAddress(*)>(_a[1]))); break;
        case 7: sendBosPort((*reinterpret_cast< const quint16(*)>(_a[1]))); break;
        case 8: sendCookie((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 9: connected(); break;
        case 10: getList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: oncomingBuddy((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 12: offlineBuddy((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 13: getMessage((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 14: getOfflineMessage(); break;
        case 15: readMetaData((*reinterpret_cast< quint16(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 16: getTypingNotif((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 17: readSSTserver((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 18: blockRateLimit(); break;
        case 19: getStatusCheck((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 20: getModifyItemFromServer((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 21: youWereAdded((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 22: getUploadIconData((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 23: getAwayMessage((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 24: getAuthorizationRequest((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 25: authorizationAcceptedAnswer((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 26: getMessageAck((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 27: clearPrivacyLists(); break;
        case 28: { quint32 _r = returnSnacReqId();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = _r; }  break;
        case 29: resetReqSnaqId(); break;
        case 30: incReq(); break;
        }
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void snacChannel::incFlapSeq()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void snacChannel::incReqSeq()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void snacChannel::rereadSocket()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void snacChannel::sendAuthKey(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void snacChannel::systemMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void snacChannel::userMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void snacChannel::sendBosServer(const QHostAddress & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void snacChannel::sendBosPort(const quint16 & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void snacChannel::sendCookie(const QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void snacChannel::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void snacChannel::getList(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void snacChannel::oncomingBuddy(const QString & _t1, quint16 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void snacChannel::offlineBuddy(const QString & _t1, quint16 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void snacChannel::getMessage(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void snacChannel::getOfflineMessage()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}

// SIGNAL 15
void snacChannel::readMetaData(quint16 _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void snacChannel::getTypingNotif(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void snacChannel::readSSTserver(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void snacChannel::blockRateLimit()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}

// SIGNAL 19
void snacChannel::getStatusCheck(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void snacChannel::getModifyItemFromServer(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void snacChannel::youWereAdded(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void snacChannel::getUploadIconData(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void snacChannel::getAwayMessage(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void snacChannel::getAuthorizationRequest(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void snacChannel::authorizationAcceptedAnswer(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void snacChannel::getMessageAck(quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void snacChannel::clearPrivacyLists()
{
    QMetaObject::activate(this, &staticMetaObject, 27, 0);
}
