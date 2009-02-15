/****************************************************************************
** Meta object code from reading C++ file 'jAccount.h'
**
** Created: Sun Feb 15 15:01:05 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jAccount.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jAccount.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jAccount[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   10,    9,    9, 0x0a,
      56,   36,    9,    9, 0x0a,
     100,   36,    9,    9, 0x0a,
     153,  144,    9,    9, 0x0a,
     188,  184,    9,    9, 0x2a,
     211,    9,    9,    9, 0x2a,
     227,    9,    9,    9, 0x08,
     251,    9,    9,    9, 0x08,
     278,    9,    9,    9, 0x08,
     290,  184,    9,    9, 0x08,
     320,  314,    9,    9, 0x08,
     340,  184,    9,    9, 0x08,
     362,    9,    9,    9, 0x08,
     401,  389,    9,    9, 0x08,
     463,  438,    9,    9, 0x08,
     504,  184,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_jAccount[] = {
    "jAccount\0\0status\0setRealStatus(int)\0"
    "server,node,handler\0"
    "getDiskoItem(QString,QString,DiscoHandler*)\0"
    "getDiskoInfo(QString,QString,DiscoHandler*)\0"
    "jid,nick\0showAddDialog(QString,QString)\0"
    "jid\0showAddDialog(QString)\0showAddDialog()\0"
    "showTransportsBrowser()\0"
    "showOtherServicesBrowser()\0setStatus()\0"
    "s_requestVCard(QString)\0vcard\0"
    "s_saveVCard(VCard*)\0s_closeVCard(QString)\0"
    "showChangeResourceDialog()\0server,node\0"
    "s_requestDiscoItems(QString,QString)\0"
    "jid,name,group,authorize\0"
    "addContact(QString,QString,QString,bool)\0"
    "showInformation(QString)\0"
};

const QMetaObject jAccount::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_jAccount,
      qt_meta_data_jAccount, 0 }
};

const QMetaObject *jAccount::metaObject() const
{
    return &staticMetaObject;
}

void *jAccount::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jAccount))
	return static_cast<void*>(const_cast< jAccount*>(this));
    return QObject::qt_metacast(_clname);
}

int jAccount::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setRealStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: getDiskoItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< DiscoHandler*(*)>(_a[3]))); break;
        case 2: getDiskoInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< DiscoHandler*(*)>(_a[3]))); break;
        case 3: showAddDialog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: showAddDialog((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: showAddDialog(); break;
        case 6: showTransportsBrowser(); break;
        case 7: showOtherServicesBrowser(); break;
        case 8: setStatus(); break;
        case 9: s_requestVCard((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: s_saveVCard((*reinterpret_cast< VCard*(*)>(_a[1]))); break;
        case 11: s_closeVCard((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: showChangeResourceDialog(); break;
        case 13: s_requestDiscoItems((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 14: addContact((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 15: showInformation((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 16;
    }
    return _id;
}
