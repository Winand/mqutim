/****************************************************************************
** Meta object code from reading C++ file 'jConnection.h'
**
** Created: Sun Feb 15 15:01:14 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jConnection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jConnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jConnection[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,   12,   12,   12, 0x0a,
      43,   12,   12,   12, 0x0a,
      70,   58,   12,   12, 0x0a,
     106,   12,   12,   12, 0x0a,
     130,  118,   12,   12, 0x0a,
     173,   12,   12,   12, 0x0a,
     180,   12,   12,   12, 0x0a,
     200,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_jConnection[] = {
    "jConnection\0\0startConnection()\0"
    "connected()\0disconnected()\0socketError\0"
    "error(QAbstractSocket::SocketError)\0"
    "hostFound()\0socketState\0"
    "stateChanged(QAbstractSocket::SocketState)\0"
    "read()\0atStartConnection()\0processMessages()\0"
};

const QMetaObject jConnection::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_jConnection,
      qt_meta_data_jConnection, 0 }
};

const QMetaObject *jConnection::metaObject() const
{
    return &staticMetaObject;
}

void *jConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jConnection))
	return static_cast<void*>(const_cast< jConnection*>(this));
    if (!strcmp(_clname, "ConnectionBase"))
	return static_cast< ConnectionBase*>(const_cast< jConnection*>(this));
    return QObject::qt_metacast(_clname);
}

int jConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: startConnection(); break;
        case 1: connected(); break;
        case 2: disconnected(); break;
        case 3: error((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 4: hostFound(); break;
        case 5: stateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 6: read(); break;
        case 7: atStartConnection(); break;
        case 8: processMessages(); break;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void jConnection::startConnection()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
