/****************************************************************************
** Meta object code from reading C++ file 'oscarprotocol.h'
**
** Created: Mon Feb 9 20:47:15 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../oscarprotocol.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'oscarprotocol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_oscarProtocol[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      29,   15,   14,   14, 0x05,
      58,   14,   14,   14, 0x05,
      86,   81,   14,   14, 0x05,
     144,   14,   14,   14, 0x05,
     160,   14,   14,   14, 0x05,
     178,   14,   14,   14, 0x05,
     199,   14,   14,   14, 0x05,
     219,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     242,   14,   14,   14, 0x0a,
     268,   14,   14,   14, 0x08,
     283,   14,   14,   14, 0x08,
     295,   14,   14,   14, 0x08,
     338,   14,   14,   14, 0x08,
     359,   14,   14,   14, 0x08,
     373,   14,   14,   14, 0x08,
     389,  387,   14,   14, 0x08,
     427,  416,   14,   14, 0x08,
     494,  489,   14,   14, 0x08,
     522,  517,   14,   14, 0x08,
     542,   14,   14,   14, 0x08,
     558,   14,   14,   14, 0x08,
     570,   14,   14,   14, 0x08,
     593,   14,   14,   14, 0x08,
     620,   14,   14,   14, 0x08,
     638,   14,   14,   14, 0x08,
     653,   14,   14,   14, 0x08,
     671,   14,   14,   14, 0x08,
     695,  693,   14,   14, 0x08,
     725,   14,   14,   14, 0x08,
     742,   14,   14,   14, 0x08,
     769,  761,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_oscarProtocol[] = {
    "oscarProtocol\0\0currentStatus\0"
    "statusChanged(accountStatus)\0"
    "systemMessage(QString)\0,,,,\0"
    "userMessage(QString,QString,QString,userMessageType,bool)\0"
    "getNewMessage()\0readAllMessages()\0"
    "addToEventList(bool)\0updateTranslation()\0"
    "accountConnected(bool)\0onReloadGeneralSettings()\0"
    "disconnected()\0connected()\0"
    "displayError(QAbstractSocket::SocketError)\0"
    "readDataFromSocket()\0clearSocket()\0"
    "sendIdentif()\0m\0sendSystemMessage(QString)\0"
    "fu,f,m,t,l\0"
    "sendUserMessage(QString,QString,QString,userMessageType,bool)\0"
    "addr\0getBosIp(QHostAddress)\0port\0"
    "getBosPort(quint16)\0incFlapSeqNum()\0"
    "incReqSeq()\0getAuthKey(QByteArray)\0"
    "reconnectToBos(QByteArray)\0connectingToBos()\0"
    "rereadSocket()\0sendAlivePacket()\0"
    "updateChangedStatus()\0,\0"
    "restartAutoAway(bool,quint32)\0"
    "blockRateLimit()\0proxyDeleteTimer()\0"
    "seconds\0onSecondIdle(int)\0"
};

const QMetaObject oscarProtocol::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_oscarProtocol,
      qt_meta_data_oscarProtocol, 0 }
};

const QMetaObject *oscarProtocol::metaObject() const
{
    return &staticMetaObject;
}

void *oscarProtocol::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_oscarProtocol))
	return static_cast<void*>(const_cast< oscarProtocol*>(this));
    return QObject::qt_metacast(_clname);
}

int oscarProtocol::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: statusChanged((*reinterpret_cast< accountStatus(*)>(_a[1]))); break;
        case 1: systemMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: userMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< userMessageType(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 3: getNewMessage(); break;
        case 4: readAllMessages(); break;
        case 5: addToEventList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: updateTranslation(); break;
        case 7: accountConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: onReloadGeneralSettings(); break;
        case 9: disconnected(); break;
        case 10: connected(); break;
        case 11: displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 12: readDataFromSocket(); break;
        case 13: clearSocket(); break;
        case 14: sendIdentif(); break;
        case 15: sendSystemMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: sendUserMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< userMessageType(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 17: getBosIp((*reinterpret_cast< const QHostAddress(*)>(_a[1]))); break;
        case 18: getBosPort((*reinterpret_cast< const quint16(*)>(_a[1]))); break;
        case 19: incFlapSeqNum(); break;
        case 20: incReqSeq(); break;
        case 21: getAuthKey((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 22: reconnectToBos((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 23: connectingToBos(); break;
        case 24: rereadSocket(); break;
        case 25: sendAlivePacket(); break;
        case 26: updateChangedStatus(); break;
        case 27: restartAutoAway((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2]))); break;
        case 28: blockRateLimit(); break;
        case 29: proxyDeleteTimer(); break;
        case 30: onSecondIdle((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void oscarProtocol::statusChanged(accountStatus _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void oscarProtocol::systemMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void oscarProtocol::userMessage(const QString & _t1, const QString & _t2, const QString & _t3, userMessageType _t4, bool _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void oscarProtocol::getNewMessage()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void oscarProtocol::readAllMessages()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void oscarProtocol::addToEventList(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void oscarProtocol::updateTranslation()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void oscarProtocol::accountConnected(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
