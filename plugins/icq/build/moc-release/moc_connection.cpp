/****************************************************************************
** Meta object code from reading C++ file 'connection.h'
**
** Created: Mon Feb 9 20:46:56 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../connection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_connection[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      24,   11,   11,   11, 0x05,
      36,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      74,   54,   11,   11, 0x08,
     133,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_connection[] = {
    "connection\0\0connected()\0sendLogin()\0"
    "connectingToBos()\0proxy,authenticator\0"
    "proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)\0"
    "dnsResults(QHostInfo)\0"
};

const QMetaObject connection::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_connection,
      qt_meta_data_connection, 0 }
};

const QMetaObject *connection::metaObject() const
{
    return &staticMetaObject;
}

void *connection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_connection))
	return static_cast<void*>(const_cast< connection*>(this));
    return QObject::qt_metacast(_clname);
}

int connection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connected(); break;
        case 1: sendLogin(); break;
        case 2: connectingToBos(); break;
        case 3: proxyAuthenticationRequired((*reinterpret_cast< const QNetworkProxy(*)>(_a[1])),(*reinterpret_cast< QAuthenticator*(*)>(_a[2]))); break;
        case 4: dnsResults((*reinterpret_cast< QHostInfo(*)>(_a[1]))); break;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void connection::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void connection::sendLogin()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void connection::connectingToBos()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
