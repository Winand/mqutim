/****************************************************************************
** Meta object code from reading C++ file 'closeconnection.h'
**
** Created: Mon Feb 9 20:46:55 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../closeconnection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'closeconnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_closeConnection[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      40,   16,   16,   16, 0x05,
      63,   16,   16,   16, 0x05,
      91,   16,   16,   16, 0x05,
     112,   16,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_closeConnection[] = {
    "closeConnection\0\0systemMessage(QString)\0"
    "sendCookie(QByteArray)\0"
    "sendBosServer(QHostAddress)\0"
    "sendBosPort(quint16)\0blockRateLimit()\0"
};

const QMetaObject closeConnection::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_closeConnection,
      qt_meta_data_closeConnection, 0 }
};

const QMetaObject *closeConnection::metaObject() const
{
    return &staticMetaObject;
}

void *closeConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_closeConnection))
	return static_cast<void*>(const_cast< closeConnection*>(this));
    return QObject::qt_metacast(_clname);
}

int closeConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: systemMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: sendCookie((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: sendBosServer((*reinterpret_cast< const QHostAddress(*)>(_a[1]))); break;
        case 3: sendBosPort((*reinterpret_cast< const quint16(*)>(_a[1]))); break;
        case 4: blockRateLimit(); break;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void closeConnection::systemMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void closeConnection::sendCookie(const QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void closeConnection::sendBosServer(const QHostAddress & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void closeConnection::sendBosPort(const quint16 & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void closeConnection::blockRateLimit()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
